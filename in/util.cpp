#include "util.h"

TEB* GetTeb() {

	return reinterpret_cast<TEB*>(__readgsqword(FIELD_OFFSET(NT_TIB, Self)));
}

PEB* GetPeb() {

	return GetTeb()->ProcessEnvironmentBlock;
}

uint64 GetImageBase() {

	return reinterpret_cast<uint64>(GetPeb()->ImageBaseAddress);
}

uint32_t GetImageSize(uint64 ImageBase) {

	auto DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(ImageBase);

	auto NtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(ImageBase + DosHeader->e_lfanew);

	return NtHeader->OptionalHeader.SizeOfImage;
}

std::pair<uint64_t, uint32_t> GetImageBaseAndSize() {

	auto ImageBase = GetImageBase();

	return {ImageBase, GetImageSize(ImageBase)};
}

bool IsInProcessRange(uint64 Address) {

	auto [ImageBase, ImageSize] = GetImageBaseAndSize();

	return Address >= ImageBase && Address < (ImageBase + ImageSize);
}

uint64 ResolveRipRelativeIndirectAddress(uint64 Address) {

	auto Offset = *reinterpret_cast<int32*>(Address + 3);

	return Address + 7 + Offset;
}

uint64 ResolveRipRelativeNearCall(uint64 Address) {

	auto Offset = *reinterpret_cast<int32*>(Address + 1);

	return Address + 5 + Offset;
}

uint64 ResolveRipRelativeIndirectCall(uint64 Address) {

	auto Offset = *reinterpret_cast<int32*>(Address + 2);

	return Address + 6 + Offset;
}

uint32 GetOffset(const uint64 Address) {

	uint64 ImageBase = GetImageBase();

	auto Offset = Address > ImageBase ? (Address - ImageBase) : 0x0;

	return static_cast<uint32>(Offset);
}

uint32 GetOffset(const PVOID Address) {

	return GetOffset(reinterpret_cast<const uint64>(Address));
}

std::pair<uint64, uint32> GetSectionByName(uint64 ImageBase, const std::string& SectionName) {

	if (!ImageBase) {
		return {};
	}

	IMAGE_DOS_HEADER* DosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(ImageBase);
	IMAGE_NT_HEADERS* NtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(ImageBase + DosHeader->e_lfanew);

	PIMAGE_SECTION_HEADER Sections = IMAGE_FIRST_SECTION(NtHeaders);

	DWORD TextSize = 0;

	for (uint32 i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++) {

		IMAGE_SECTION_HEADER& CurrentSection = Sections[i];

		std::string CurrentSectionName = reinterpret_cast<const char*>(CurrentSection.Name);

		if (!SectionName.compare(CurrentSectionName)) {

			return {ImageBase + CurrentSection.VirtualAddress, CurrentSection.Misc.VirtualSize};
		}
	}

	return {};
}

bool WideCharToString(wchar_t* WideChar, std::string& MultiByteString) {

	char byte[1024 * sizeof(wchar_t)] = "";

	int result = WideCharToMultiByte(CP_UTF8, 0, WideChar, -1, byte, 1024 * sizeof(wchar_t), nullptr, nullptr);

	MultiByteString.assign(byte);

	return result > 0;
}

std::vector<uint32> PatternToBytes(const char* Pattern) {

	std::vector<uint32> Bytes = {};

	const auto Start = const_cast<char*>(Pattern);

	const auto End = const_cast<char*>(Pattern) + strlen(Pattern);

	for (auto Current = Start; Current < End; ++Current) {

		if (*Current == '?') {

			++Current;

			if (*Current == '?') {

				++Current;
			}

			Bytes.push_back(0xFFFF'FFFF);

		} else {

			Bytes.push_back(strtoul(Current, &Current, 16));
		}
	}

	return Bytes;
}

PVOID FindPatternInRange(const char* Signature, const void* Start, const uint64 Range, const bool Relative, const int32 Offset) {

	return FindPatternInRange(PatternToBytes(Signature), Start, Range, Relative, Offset);
}

PVOID FindPatternInRange(const char* Signature, const uint64 Start, const uint64 Range, const bool Relative, const int32 Offset) {

	return FindPatternInRange(Signature, reinterpret_cast<const void*>(Start), Range, Relative, Offset);
}

PVOID FindPatternInRange(std::vector<uint32>&& Signature, const void* Start, const uint64 Range, const bool Relative, int32 Offset, const uint32 SkipFoundCount) {

	const auto PatternBytes = Signature.data();

	const auto PatternLength = Signature.size();

	for (uint64 i = 0; i < (Range - PatternLength); i++) {

		bool IsFound = true;

		int CurrentSkips = 0;

		for (uint64 j = 0; j < PatternLength; ++j) {

			if (static_cast<const uint8*>(Start)[i + j] != PatternBytes[j] && PatternBytes[j] != 0xFFFF'FFFF) {

				IsFound = false;

				break;
			}
		}

		if (IsFound) {

			if (CurrentSkips != SkipFoundCount) {

				CurrentSkips++;

				continue;
			}

			uint64 Address = reinterpret_cast<uint64>(Start) + i;

			if (Relative) {

				if (Offset == -1) {

					Offset = static_cast<int32>(PatternLength);
				}

				Address = Address + Offset + 4 + *reinterpret_cast<int32*>(Address + Offset);
			}

			return reinterpret_cast<PVOID>(Address);
		}
	}

	return nullptr;
}

bool IsUserAddressValid(const void* VirtualAddress) {

	MEMORY_BASIC_INFORMATION MemoryBasicInformation = {};
	if (!VirtualQuery(VirtualAddress, &MemoryBasicInformation, sizeof(MEMORY_BASIC_INFORMATION))) {
		return false;
	}

	if (MemoryBasicInformation.State != MEM_COMMIT) {
		return false;
	}

	if (MemoryBasicInformation.Protect & (PAGE_NOACCESS | PAGE_GUARD)) {
		return false;
	}

	return MemoryBasicInformation.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
}