#include "UnrealTypes.h"
#include "NameArray.h"
#include "Offsets.h"

bool FName::Init() {

	auto ImageBase = GetImageBase();

	auto [SectionAddress, SectionSize] = GetSectionByName(ImageBase, ".text");

	auto StringAddress = StringSearch("ForwardShadingQuality_", SectionAddress, SectionSize);
	if (!StringAddress) {
		return false;
	}

	AppendString = reinterpret_cast<void(*)(const void*, FString&)>(FindPatternInRange("48 8D ? ? 48 8D ? ? E8", StringAddress, 0x50, true, -1));
	if (!AppendString) {
		return false;
	}

	Offset::FName::AppendString = GetOffset(AppendString);

	if (!NameArray::FindNamePool()) {
		return false;
	}

	ToStr = [](const void* NameAddress) -> std::string {

		wchar_t buffer[1024]{};

		FString TempSting = {};
		TempSting.Data = buffer;
		TempSting.MaxElements = 1024;
		TempSting.NumElements = 0;

		AppendString(NameAddress, TempSting);

		return TempSting.ToString();
	};

	Print("--> UAGame.exe AppendString: %p 0x%lX \n", AppendString, Offset::FName::AppendString);

	return true;
}

const void* FName::GetAddress() const {

	return Address;
}

std::string FName::ToString() const {

	if (!Address) {
		return "None";
	}

	std::string OutputString = ToStr(Address);

	size_t Pos = OutputString.rfind('/');

	if (Pos == std::string::npos) {
		return OutputString;
	}

	return OutputString.substr(Pos + 1);
}

std::string FName::ToRawString() const {

	if (!Address) {
		return "None";
	}

	return ToStr(Address);
}