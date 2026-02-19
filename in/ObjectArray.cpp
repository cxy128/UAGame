#include "ObjectArray.h"

int32 ObjectArray::Num() {

	return *reinterpret_cast<int32*>(GObjects + Offset::FChunkedFixedUObjectArray::NumElements);
}

void ObjectArray::DumpObjects(const std::filesystem::path& Path, bool WithPathName) {

	std::ofstream DumpStream(Path / "GObjects-Dump.txt");

	DumpStream << "Object dump\n\n\n";

	DumpStream << "Count: " << Num() << "\n\n\n";

	for (auto Object : ObjectArray()) {

		if (!WithPathName) {

			DumpStream << std::format("[{:08X}] {{{}}} {}\n", Object.GetIndex(), Object.GetAddress(), Object.GetFullName());

		} else {

			DumpStream << std::format("[{:08X}] {{{}}} {}\n", Object.GetIndex(), Object.GetAddress(), Object.GetPathName());
		}
	}

	DumpStream.close();
}

void ObjectArray::Init() {

	InitGObjects();

	uint8** Objects = reinterpret_cast<uint8**>(GObjects + Offset::FChunkedFixedUObjectArray::Objects);

	InitFUObjectItem(*reinterpret_cast<uint8**>(*Objects));

	InitNumElementsPerChunk(reinterpret_cast<uint8*>(Objects));

	InitUObject();

	InitName();

	InitUStruct();

	InitUClass();

	InitUFunction();

	InitFField();

	// TODO

	InitProcessEvent();

	InitGWorld();
}

void ObjectArray::InitGObjects(bool ScanAllMemory) {

	auto [ImageBase, ImageSize] = GetImageBaseAndSize();

	uint64 SearchBase = ImageBase;
	uint32 SearchRange = ImageSize;

	if (!ScanAllMemory) {

		const auto [DataSection, DataSize] = GetSectionByName(ImageBase, ".data");

		if (DataSection != 0x0 && DataSize != 0x0) {

			SearchBase = DataSection;

			SearchRange = DataSize;

		} else {

			ScanAllMemory = true;
		}
	}

	for (uint32 i = 0; i < SearchRange; i += 0x4) {

		const uint64 CurrentAddress = SearchBase + i;

		if (IsAddressValidGObjects(CurrentAddress)) {

			GObjects = reinterpret_cast<uint8*>(SearchBase + i);

			ByIndex = [](void* Objects, int32 Index, uint32 ObjectItemSize, uint32 ObjectItemObjectBaseOffset, uint32 NumElements) -> void* {

				if (Index < 0 || Index > Num()) {
					return nullptr;
				}

				const uint32 ChunkIndex = Index / NumElements;
				const uint32 ChunkOffset = Index % NumElements;

				uint8* ChunkPtr = *reinterpret_cast<uint8**>(Objects);

				uint8* Chunk = reinterpret_cast<uint8**>(ChunkPtr)[ChunkIndex];
				uint8* ItemPtr = Chunk + (ChunkOffset * ObjectItemSize);

				return *reinterpret_cast<void**>(ItemPtr + ObjectItemObjectBaseOffset);
			};

			Print("--> UAGame.exe GObjects %p \n", GObjects);

			return;
		}
	}

	if (!ScanAllMemory) {

		ObjectArray::InitGObjects(true);

		return;
	}
}

void ObjectArray::InitFUObjectItem(uint8* FirstItemPtr) {

	for (int i = 0x0; i < 0x10; i += 4) {

		if (IsUserAddressValid(*reinterpret_cast<uint8**>(FirstItemPtr + i))) {

			Offset::FUObjectItem::Object = i;

			break;
		}
	}

	for (int i = Offset::FUObjectItem::Object + 0x8; i <= 0x38; i += 4) {

		void* SecondObject = *reinterpret_cast<uint8**>(FirstItemPtr + i);

		void* ThirdObject = *reinterpret_cast<uint8**>(FirstItemPtr + (i * 2) - Offset::FUObjectItem::Object);

		if (IsUserAddressValid(SecondObject) && IsUserAddressValid(*reinterpret_cast<void**>(SecondObject)) &&
			IsUserAddressValid(ThirdObject) && IsUserAddressValid(*reinterpret_cast<void**>(ThirdObject))) {

			Offset::FUObjectItem::SizeOf = i - Offset::FUObjectItem::Object;

			break;
		}
	}

	Print("--> UAGame.exe FUObjectItem 0x%lX 0x%lX \n", Offset::FUObjectItem::Object, Offset::FUObjectItem::SizeOf);
}

void ObjectArray::InitNumElementsPerChunk(uint8* ChunksPtr) {

	int IndexOffset = 0x0;

	uint8* ObjAtIdx374 = (uint8*)ByIndex(ChunksPtr, 0x374, Offset::FUObjectItem::SizeOf, Offset::FUObjectItem::Object, 0x10000);

	uint8* ObjAtIdx106 = (uint8*)ByIndex(ChunksPtr, 0x106, Offset::FUObjectItem::SizeOf, Offset::FUObjectItem::Object, 0x10000);

	for (int i = 0x8; i < 0x20; i++) {

		if (*reinterpret_cast<int32*>(ObjAtIdx374 + i) == 0x374 && *reinterpret_cast<int32*>(ObjAtIdx106 + i) == 0x106) {

			IndexOffset = i;

			Offset::FChunkedFixedUObjectArray::NumElementsPerChunk = 0x10000;

			break;
		}
	}

	int IndexToCheck = 0x10400;

	while (ObjectArray::Num() > IndexToCheck) {

		if (void* Obj = ByIndex(ChunksPtr, IndexToCheck, Offset::FUObjectItem::SizeOf, Offset::FUObjectItem::Object, 0x10000)) {

			const bool bHasBiggerChunkSize = (*reinterpret_cast<int32*>((uint8*)Obj + IndexOffset) != IndexToCheck);

			Offset::FChunkedFixedUObjectArray::NumElementsPerChunk = bHasBiggerChunkSize ? 0x10400 : 0x10000;

			break;
		}

		IndexToCheck += 0x10400;
	}

	Print("--> UAGame.exe NumElementsPerChunk 0x%lX \n", Offset::FChunkedFixedUObjectArray::NumElementsPerChunk);
}

void ObjectArray::InitUObject() {

	uint8* ObjA = static_cast<uint8*>(ObjectArray::GetByIndex(0x055).GetAddress());

	uint8* ObjB = static_cast<uint8*>(ObjectArray::GetByIndex(0x123).GetAddress());

	auto GetIndexOffset = [ObjA, ObjB]() -> int32 {

		std::vector<std::pair<void*, int32>> Infos = {};

		Infos.emplace_back(ObjectArray::GetByIndex(0x055).GetAddress(), 0x055);

		Infos.emplace_back(ObjectArray::GetByIndex(0x123).GetAddress(), 0x123);

		return Offset::Find<4>(Infos, 0x0);
	};

	auto GetNameOffset = [ObjA, ObjB](int32 ClassOffset, int32 IndexOffset) -> int32 {

		const auto ObjAValueAfterClassAsPtr = *reinterpret_cast<uint64**>(ObjA + ClassOffset + sizeof(void*));
		const auto ObjBValueAfterClassAsPtr = *reinterpret_cast<uint64**>(ObjB + ClassOffset + sizeof(void*));

		const bool bIsObjAOuterRightAfterClass = IsUserAddressValid(ObjAValueAfterClassAsPtr) || ObjAValueAfterClassAsPtr == nullptr;
		const bool bIsObjBOuterRightAfterClass = IsUserAddressValid(ObjBValueAfterClassAsPtr) || ObjBValueAfterClassAsPtr == nullptr;

		if (bIsObjAOuterRightAfterClass && bIsObjBOuterRightAfterClass) {

			if ((ClassOffset - IndexOffset) >= 0x4) {

				Offset::Setting::IsObjectNameBeforeClass = true;

				return IndexOffset + 0x4;
			}

			return ClassOffset + 0x10;
		}

		return ClassOffset + sizeof(void*);
	};

	Offset::UObject::Vft = 0x00;
	Offset::UObject::Flags = sizeof(void*);
	Offset::UObject::Index = GetIndexOffset();
	Offset::UObject::Class = Offset::GetValidPointer(ObjA, ObjB, Offset::UObject::Index + sizeof(int), 0x40);
	Offset::UObject::Name = GetNameOffset(Offset::UObject::Class, Offset::UObject::Index);
	Offset::UObject::Outer = Offset::NotFound;

	const int32 OuterSearchBase = std::max(Offset::UObject::Name, Offset::UObject::Class) + 0x8;

	while (Offset::UObject::Outer == Offset::NotFound) {

		Offset::UObject::Outer = Offset::GetValidPointer(ObjA, ObjB, OuterSearchBase, 0x40);

		ObjA = static_cast<uint8*>(ObjectArray::GetByIndex(rand() % 0x400).GetAddress());
		ObjB = static_cast<uint8*>(ObjectArray::GetByIndex(rand() % 0x400).GetAddress());
	}

	Print("--> UAGame.exe UObject 0x%lX 0x%lX 0x%lX 0x%lX 0x%lX \n", Offset::UObject::Vft, Offset::UObject::Flags, Offset::UObject::Index, Offset::UObject::Class, Offset::UObject::Name, Offset::UObject::Outer);
}

void ObjectArray::InitName() {

	UEObject FirstObject = ObjectArray::GetByIndex(0);

	const uint8* NameAddress = static_cast<const uint8*>(FirstObject.GetFName().GetAddress());

	const int32 FNameFirstInt = *reinterpret_cast<const int32*>(NameAddress);

	const int32 FNameSecondInt = *reinterpret_cast<const int32*>(NameAddress + 0x4);

	const int32 FNameSize = !Offset::Setting::IsObjectNameBeforeClass ? (Offset::UObject::Outer - Offset::UObject::Name) : (Offset::UObject::Class - Offset::UObject::Name);

	Offset::FName::ComparisonIndex = 0x0;
	Offset::FName::Number = 0x4;

	if (FNameSize == 0x8 && FNameFirstInt == FNameSecondInt) {

		Offset::FName::Number = -0x1;
		Offset::FName::SizeOf = 0x8;

	} else if (FNameSize == 0x10) {

		Offset::FName::Number = FNameFirstInt == FNameSecondInt ? 0x8 : 0x4;

		Offset::FName::SizeOf = 0xC;

	} else {

		Offset::FName::Number = 0x4;

		Offset::FName::SizeOf = 0x8;
	}

	Print("--> UAGame.exe Name 0x%lX 0x%lX 0x%lX \n", Offset::FName::ComparisonIndex, Offset::FName::Number, Offset::FName::SizeOf);
}

void ObjectArray::InitUClass() {

	auto FindCastFlagsOffset = []() -> int32 {

		std::vector<std::pair<void*, EClassCastFlags>> Infos = {};

		Infos.push_back({ObjectArray::FindObjectFast("Actor").GetAddress(), EClassCastFlags::Actor});
		Infos.push_back({ObjectArray::FindObjectFast("Class").GetAddress(), EClassCastFlags::Field | EClassCastFlags::Struct | EClassCastFlags::Class});

		return Offset::Find(Infos);
	};

	auto FindDefaultObjectOffset = []() -> int32 {

		std::vector<std::pair<void*, void*>> Infos = {};

		Infos.push_back({ObjectArray::FindObjectFast("Object").GetAddress(), ObjectArray::FindObjectFast("Default__Object").GetAddress()});
		Infos.push_back({ObjectArray::FindObjectFast("Field").GetAddress(), ObjectArray::FindObjectFast("Default__Field").GetAddress()});

		return Offset::Find(Infos, 0x28, 0x200);
	};

	Offset::UClass::CastFlags = FindCastFlagsOffset();

	Offset::UClass::ClassDefaultObject = FindDefaultObjectOffset();

	Print("--> UAGame.exe UClass 0x%lX [ClassDefaultObject: 0x%lX] \n", Offset::UClass::CastFlags, Offset::UClass::ClassDefaultObject);
}

void ObjectArray::InitUStruct() {

	auto FindChildOffset = []() -> int32 {

		std::vector<std::pair<void*, void*>> Infos = {};

		Infos.push_back({ObjectArray::FindObjectFast("PlayerController").GetAddress(), ObjectArray::FindObjectFastInOuter("WasInputKeyJustReleased", "PlayerController").GetAddress()});
		Infos.push_back({ObjectArray::FindObjectFast("Controller").GetAddress(), ObjectArray::FindObjectFastInOuter("UnPossess", "Controller").GetAddress()});

		if (Offset::Find(Infos) == Offset::NotFound) {

			Infos.clear();

			Infos.push_back({ObjectArray::FindObjectFast("Vector").GetAddress(), ObjectArray::FindObjectFastInOuter("X", "Vector").GetAddress()});
			Infos.push_back({ObjectArray::FindObjectFast("Vector4").GetAddress(), ObjectArray::FindObjectFastInOuter("X", "Vector4").GetAddress()});
			Infos.push_back({ObjectArray::FindObjectFast("Vector2D").GetAddress(), ObjectArray::FindObjectFastInOuter("X", "Vector2D").GetAddress()});
			Infos.push_back({ObjectArray::FindObjectFast("Guid").GetAddress(), ObjectArray::FindObjectFastInOuter("A","Guid").GetAddress()});

			return Offset::Find(Infos);
		}

		Offset::Setting::UseFProperty = true;

		return Offset::Find(Infos);
	};

	auto FindSuperOffset = []() -> int32 {

		std::vector<std::pair<void*, void*>> Infos = {};

		Infos.push_back({ObjectArray::FindObjectFast("Struct").GetAddress(), ObjectArray::FindObjectFast("Field").GetAddress()});
		Infos.push_back({ObjectArray::FindObjectFast("Class").GetAddress(), ObjectArray::FindObjectFast("Struct").GetAddress()});

		if (Infos[0].first == nullptr) {
			Infos[0].first = Infos[1].second = ObjectArray::FindObjectFast("struct").GetAddress();
		}

		return Offset::Find(Infos);
	};

	auto FindStructSizeOffset = []() -> int32 {

		std::vector<std::pair<void*, int32_t>> Infos = {};

		Infos.push_back({ObjectArray::FindObjectFast("Color").GetAddress(), 0x04});
		Infos.push_back({ObjectArray::FindObjectFast("Guid").GetAddress(), 0x10});

		return Offset::Find(Infos);
	};

	auto FindMinAlignmentOffset = []() -> int32 {

		std::vector<std::pair<void*, int32_t>> Infos = {};

		Infos.push_back({ObjectArray::FindObjectFast("Transform").GetAddress(), 0x10});
		Infos.push_back({ObjectArray::FindObjectFast("PlayerController").GetAddress(), 0x8});

		return Offset::Find(Infos);
	};

	auto FindChildPropertiesOffset = []() -> int32 {

		uint8* ObjA = (uint8*)ObjectArray::FindObjectFast("Color").GetAddress();
		uint8* ObjB = (uint8*)ObjectArray::FindObjectFast("Guid").GetAddress();

		return Offset::GetValidPointer(ObjA, ObjB, Offset::UStruct::Children + 0x08, 0x80);
	};

	Offset::UStruct::Children = FindChildOffset();

	Offset::UStruct::SuperStruct = FindSuperOffset();

	Offset::UStruct::Size = FindStructSizeOffset();

	Offset::UStruct::MinAlignment = FindMinAlignmentOffset();

	if (Offset::Setting::UseFProperty) {

		Offset::UStruct::ChildProperties = FindChildPropertiesOffset();
	}

	Print("--> UAGame.exe UStruct 0x%lX 0x%lX 0x%lX 0x%lX [ChildProperties: 0x%lX] \n", Offset::UStruct::Children, Offset::UStruct::SuperStruct, Offset::UStruct::Size, Offset::UStruct::MinAlignment, Offset::UStruct::ChildProperties);
}

void ObjectArray::InitUFunction() {

	{
		std::vector<std::pair<void*, EFunctionFlags>> Infos;

		Infos.push_back({ObjectArray::FindObjectFast("WasInputKeyJustPressed", EClassCastFlags::Function).GetAddress(), EFunctionFlags::Final | EFunctionFlags::Native | EFunctionFlags::Public | EFunctionFlags::BlueprintCallable | EFunctionFlags::BlueprintPure | EFunctionFlags::Const});
		Infos.push_back({ObjectArray::FindObjectFast("ToggleSpeaking", EClassCastFlags::Function).GetAddress(), EFunctionFlags::Exec | EFunctionFlags::Native | EFunctionFlags::Public});
		Infos.push_back({ObjectArray::FindObjectFast("SwitchLevel", EClassCastFlags::Function).GetAddress(), EFunctionFlags::Exec | EFunctionFlags::Native | EFunctionFlags::Public});

		if (Infos[2].first == nullptr) {
			Infos[2].first = ObjectArray::FindObjectFast("FOV", EClassCastFlags::Function).GetAddress();
		}

		int32 FunctionFlags = Offset::Find(Infos);

		if (FunctionFlags == Offset::NotFound) {

			for (auto& [_, Flags] : Infos) {

				Flags |= EFunctionFlags::RequiredAPI;
			}

			FunctionFlags = Offset::Find(Infos);
		}

		Offset::UFunction::FunctionFlags = FunctionFlags;
	}

	{
		std::vector<std::pair<void*, EFunctionFlags>> Infos;

		uint64 WasInputKeyJustPressed = reinterpret_cast<uint64>(ObjectArray::FindObjectFast("WasInputKeyJustPressed", EClassCastFlags::Function).GetAddress());
		uint64 ToggleSpeaking = reinterpret_cast<uint64>(ObjectArray::FindObjectFast("ToggleSpeaking", EClassCastFlags::Function).GetAddress());
		uint64 SwitchLevel_Or_FOV = reinterpret_cast<uint64>(ObjectArray::FindObjectFast("SwitchLevel", EClassCastFlags::Function).GetAddress());

		if (!SwitchLevel_Or_FOV) {
			SwitchLevel_Or_FOV = reinterpret_cast<uint64>(ObjectArray::FindObjectFast("FOV", EClassCastFlags::Function).GetAddress());
		}

		for (int i = 0x30; i < 0x140; i += sizeof(void*)) {

			if (IsInProcessRange(*reinterpret_cast<uintptr_t*>(WasInputKeyJustPressed + i)) && IsInProcessRange(*reinterpret_cast<uintptr_t*>(ToggleSpeaking + i)) && IsInProcessRange(*reinterpret_cast<uintptr_t*>(SwitchLevel_Or_FOV + i))) {

				Offset::UFunction::ExecFunction = i;

				break;
			}
		}
	}

	Print("--> UAGame.exe UFunction 0x%lX 0x%lX \n", Offset::UFunction::FunctionFlags, Offset::UFunction::ExecFunction);
}

void ObjectArray::InitFField() {

	auto GuidChildProperties = ObjectArray::FindStructFast("Guid").GetChildProperties();

	auto VectorChildProperties = ObjectArray::FindStructFast("Vector").GetChildProperties();

	{
		Offset::FField::Next = Offset::GetValidPointer(GuidChildProperties.GetAddress(), VectorChildProperties.GetAddress(), Offset::FField::Owner + 0x8, 0x48);
	}

	{
		Offset::FField::Class = Offset::GetValidPointer<false>(GuidChildProperties.GetAddress(), VectorChildProperties.GetAddress(), 0x8, 0x30, true);
	}

	{
		auto FindFFieldNameOffset = [&]() -> uint32 {

			std::string GuidChildPropertiesName = GuidChildProperties.GetName();

			std::string VectorChildPropertiesName = VectorChildProperties.GetName();

			if ((GuidChildPropertiesName == "A" || GuidChildPropertiesName == "D") && (VectorChildPropertiesName == "X" || VectorChildPropertiesName == "Z")) {

				return Offset::FField::Name;
			}

			for (Offset::FField::Name = Offset::FField::Owner; Offset::FField::Name < 0x40; Offset::FField::Name += 4) {

				GuidChildPropertiesName = GuidChildProperties.GetName();
				VectorChildPropertiesName = VectorChildProperties.GetName();

				if ((GuidChildPropertiesName == "A" || GuidChildPropertiesName == "D") && (VectorChildPropertiesName == "X" || VectorChildPropertiesName == "Z")) {

					return Offset::FField::Name;
				}
			}

			return Offset::NotFound;
		};

		// TODO

		Offset::FField::Name = FindFFieldNameOffset();
	}

	{
		Offset::FField::Flags = Offset::FField::Name + Offset::FName::SizeOf;
	}

	Print("--> UAGame.exe FField 0x%lX 0x%lX 0x%lX 0x%lX \n", Offset::FField::Next, Offset::FField::Class, Offset::FField::Name, Offset::FField::Flags);
}

bool ObjectArray::InitProcessEvent() {

	PVOID* VTable = *(PVOID**)ObjectArray::GetByIndex(0).GetAddress();

	for (int i = 0; i < 0x150; i++) {

		uint64 CurrentFuncAddress = reinterpret_cast<uint64>(VTable[i]);

		if (!CurrentFuncAddress || !IsInProcessRange(CurrentFuncAddress)) {
			break;
		}

		if (*reinterpret_cast<uint8*>(CurrentFuncAddress) == static_cast<uint8>(0xE9)) {

			CurrentFuncAddress = CurrentFuncAddress + 5 + *reinterpret_cast<const int32*>(CurrentFuncAddress + 1);

			if (!IsInProcessRange(CurrentFuncAddress)) {
				break;
			}
		}

		if (!FindPatternInRange({0xF7, 0xFFFF'FFFF , Offset::UFunction::FunctionFlags, 0x0, 0x0, 0x0, 0x0, 0x04, 0x0, 0x0}, reinterpret_cast<uint8*>(CurrentFuncAddress), 0x400)) {
			continue;
		}

		if (!FindPatternInRange({0xF7, 0xFFFF'FFFF, Offset::UFunction::FunctionFlags, 0x0, 0x0, 0x0, 0x0, 0x04, 0x0, 0x0}, reinterpret_cast<uint8*>(CurrentFuncAddress), 0xF00)) {
			continue;
		}

		Offset::ProcessEventIndex = i;
		Offset::ProcessEvent = GetOffset(CurrentFuncAddress);

		Print("--> UAGame.exe FindProcessEvent %lX %lX \n", Offset::ProcessEventIndex, Offset::ProcessEvent);

		return true;
	}

	return false;
}

void ObjectArray::InitGWorld() {

	UEClass UWorld = ObjectArray::FindClassFast("World");

	for (UEObject Obj : ObjectArray()) {

		if (!Obj.GetAddress()) {

			continue;
		}

		if (Obj.HasAnyFlags(EObjectFlags::ClassDefaultObject)) {

			continue;
		}

		if (!Obj.IsA(UWorld)) {

			continue;
		}

		void* Result = FindAlignedValueInProcess(Obj.GetAddress(), true);

		if (Result) {

			Offset::GWorld = GetOffset(Result);

			break;
		}
	}

	Print("--> UAGame.exe GWorld 0x%lX \n", Offset::GWorld);
}

bool ObjectArray::IsAddressValidGObjects(uint64 Address) {

	void** Objects = reinterpret_cast<void**>(*reinterpret_cast<uint64*>(Address + Offset::FChunkedFixedUObjectArray::Objects));

	int32 MaxElements = *reinterpret_cast<int32*>(Address + Offset::FChunkedFixedUObjectArray::MaxElements);
	int32 NumElements = *reinterpret_cast<int32*>(Address + Offset::FChunkedFixedUObjectArray::NumElements);

	int32 MaxChunks = *reinterpret_cast<int32*>(Address + Offset::FChunkedFixedUObjectArray::MaxChunks);
	int32 NumChunks = *reinterpret_cast<int32*>(Address + Offset::FChunkedFixedUObjectArray::NumChunks);

	if (NumChunks > 0x14 || NumChunks < 0x1) {
		return false;
	}

	if (MaxChunks > 0x22F || MaxChunks < 0x6) {
		return false;
	}

	if (NumElements > MaxElements || NumChunks > MaxChunks) {
		return false;
	}

	if (((NumElements / 0x10000) + 1) != NumChunks && ((NumElements / 0x10400) + 1) != NumChunks) {
		return false;
	}

	if ((MaxElements / 0x10000) != MaxChunks && (MaxElements / 0x10400) != MaxChunks) {
		return false;
	}

	if (!Objects || !IsUserAddressValid(Objects))
		return false;

	for (int i = 0; i < NumChunks; i++) {

		if (!Objects[i] || !IsUserAddressValid(Objects[i])) {

			return false;
		}
	}

	return true;
}

template<typename UEType>  UEType ObjectArray::GetByIndex(int32 Index) {

	return UEType(ByIndex(GObjects + Offset::FChunkedFixedUObjectArray::Objects, Index, Offset::FUObjectItem::SizeOf, Offset::FUObjectItem::Object, Offset::FChunkedFixedUObjectArray::NumElementsPerChunk));
}

UEClass ObjectArray::FindClassFast(std::string Name) {

	return FindObjectFast<UEClass>(Name, EClassCastFlags::Class);
}

UEStruct ObjectArray::FindStructFast(const std::string& Name) {

	return FindObjectFast<UEClass>(Name, EClassCastFlags::Struct);
}

template<typename UEType>  UEType ObjectArray::FindObjectFastInOuter(std::string Name, std::string Outer) {

	auto ObjArray = ObjectArray();

	for (UEObject Object : ObjArray) {

		if (Object.GetName() == Name && Object.GetOuter().GetName() == Outer) {

			return Object.Cast<UEType>();
		}
	}

	return UEType();
}

ObjectArray::ObjectsIterator ObjectArray::begin() {

	return ObjectsIterator(*this);
}

ObjectArray::ObjectsIterator ObjectArray::end() {

	return ObjectsIterator(*this, Num());
}

ObjectArray::ObjectsIterator::ObjectsIterator(ObjectArray& Array, int32 StartIndex) : IteratedArray(Array), CurrentIndex(StartIndex), CurrentObject(ObjectArray::GetByIndex(StartIndex)) {

}

UEObject ObjectArray::ObjectsIterator::operator*() {

	return CurrentObject;
}

ObjectArray::ObjectsIterator& ObjectArray::ObjectsIterator::operator++() {

	CurrentObject = ObjectArray::GetByIndex(++CurrentIndex);

	while (!CurrentObject && CurrentIndex < (ObjectArray::Num() - 1)) {
		CurrentObject = ObjectArray::GetByIndex(++CurrentIndex);
	}

	if (!CurrentObject && CurrentIndex == (ObjectArray::Num() - 1)) [[unlikely]]
		CurrentIndex++;

	return *this;
}

bool ObjectArray::ObjectsIterator::operator!=(const ObjectsIterator& Other) {

	return CurrentIndex != Other.CurrentIndex;
}

int32 ObjectArray::ObjectsIterator::GetIndex() const {

	return CurrentIndex;
}
