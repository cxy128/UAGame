#pragma once

#include <Windows.h>
#include <intrin.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <format>
#include <vector>
#include "Common.h"

#define OUTPUT_DEBUG TRUE

inline auto Print = []<typename ...E>(const char* Format, E ...Arguments) -> void {

#if OUTPUT_DEBUG == TRUE

	char buf[256] = "";
	sprintf_s(buf, 256, Format, Arguments...);

	OutputDebugStringA(buf);

#endif
};

TEB* GetTeb();

PEB* GetPeb();

uint64 GetImageBase();

uint32 GetImageSize(uint64 ImageBase);

std::pair<uint64, uint32> GetImageBaseAndSize();

bool IsInProcessRange(uint64 Address);

uint64 ResolveRipRelativeIndirectAddress(uint64 Address);

uint64 ResolveRipRelativeNearCall(uint64 Address);

uint64 ResolveRipRelativeIndirectCall(uint64 Address);

uint32 GetOffset(const uint64 Address);

uint32 GetOffset(const PVOID Address);

std::pair<uint64, uint32> GetSectionByName(uint64 ImageBase, const std::string& SectionName);

bool WideCharToString(wchar_t* WideChar, std::string& MultiByteString);

std::vector<uint32> PatternToBytes(const char* Pattern);

PVOID FindPatternInRange(const char* Signature, const void* Start, const uint64 Range, const bool Relative = false, const int32 Offset = 0);

PVOID FindPatternInRange(const char* Signature, const uint64 Start, const uint64 Range, const bool Relative = false, const int32 Offset = 0);

PVOID FindPatternInRange(std::vector<uint32>&& Signature, const void* Start, const uint64 Range, const bool Relative = false, int32 Offset = 0, const uint32 SkipFoundCount = 0);

bool IsUserAddressValid(const void* VirtualAddress);

template<typename T> inline uint64 StringLength(const T* Str) {

	if constexpr (std::is_same<T, char>()) {

		return std::string(Str).length();

	} else if constexpr (std::is_same<T, char8_t>()) {

		return std::u8string(Str).length();

	} else if constexpr (std::is_same<T, wchar_t>()) {

		return std::wstring(Str).length();
	}

	return 0;
}

template<typename T> inline bool StringEqual(const T* Left, const T* Right) {

	if constexpr (std::is_same<T, char>()) {

		return !std::string(Left).compare(Right);

	} else if constexpr (std::is_same<T, char8_t>()) {

		return !std::u8string(Left).compare(Right);

	} else if constexpr (std::is_same<T, wchar_t>()) {

		return !std::wstring(Left).compare(Right);
	}

	return false;
}

template<typename T> inline uint64 StringSearch(const T* str, uint64 StartAddress, uint64 Range) {

	auto SearchAddress = reinterpret_cast<uint8*>(StartAddress);

	auto Length = StringLength(str);

	for (uint64 i = 0; i < Range; i++) {

		if ((SearchAddress[i] == static_cast<uint8>(0x4C) || SearchAddress[i] == static_cast<uint8>(0x48)) && SearchAddress[i + 1] == static_cast<uint8>(0x8D)) {

			auto Address = reinterpret_cast<uint64>(SearchAddress + i);

			uint64 StringAddress = ResolveRipRelativeIndirectAddress(Address);

			if (!IsInProcessRange(StringAddress)) {

				continue;
			}

			if (StringEqual(str, reinterpret_cast<T*>(StringAddress))) {

				return Address;
			}
		}
	}

	return 0;
}

template<typename T> inline T* FindAlignedValueInProcessInRange(T Value, int32 Alignment, uint64 StartAddress, uint32 Range) {

	constexpr int32 ElementSize = sizeof(T);

	for (uint32 i = 0x0; i < Range; i += Alignment) {

		T* TypedPtr = reinterpret_cast<T*>(StartAddress + i);

		if (*TypedPtr == Value) {

			return TypedPtr;
		}
	}

	return nullptr;
}

template<typename T> inline T* FindAlignedValueInProcess(T Value, bool SearchAllSections = false, int32 Alignment = alignof(T), const std::string& Sectionname = ".data") {

	const auto [ImageBase, ImageSize] = GetImageBaseAndSize();

	uint64 SearchStart = ImageBase;
	uint32 SearchRange = ImageSize;

	if (!SearchAllSections) {

		const auto [SectionStart, SectionSize] = GetSectionByName(ImageBase, Sectionname);

		if (SectionStart != 0x0 && SectionSize != 0x0) {

			SearchStart = SectionStart;

			SearchRange = SectionSize;
		}
	}

	return FindAlignedValueInProcessInRange(Value, Alignment, SearchStart, SearchRange);
}

