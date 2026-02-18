#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <format>
#include <vector>
#include <algorithm>
#include "util.h"
#include "Offsets.h"
#include "Enums.h"
#include "UnrealObjects.h"

class ObjectArray {

private:

	static inline uint8* GObjects = nullptr;

private:

	static inline void* (*ByIndex)(void* Objects, int32 Index, uint32 ObjectItemSize, uint32 ObjectItemObjectBaseOffset, uint32 NumElements) = nullptr;

public:

	static int32 Num();

	static void DumpObjects(const std::filesystem::path& Path = std::filesystem::current_path(), bool WithPathName = false);

public:

	static void Init();

	static void InitGObjects(bool ScanAllMemory = false);

	static void InitFUObjectItem(uint8* FirstItemPtr);

	static void InitNumElementsPerChunk(uint8* ChunksPtr);

	static void InitUObject();

	static void InitName();

	static void InitUStruct();

	static void InitUClass();

	static void InitGWorld();

	static bool IsAddressValidGObjects(uint64 Address);

public:

	template<typename UEType = UEObject> static UEType GetByIndex(int32 Index);

	template<typename UEType = UEObject> static UEType FindObjectFast(std::string Name, EClassCastFlags RequiredType = EClassCastFlags::None) {

		auto ObjArray = ObjectArray();

		for (UEObject Object : ObjArray) {

			if (Object.IsA(RequiredType) && Object.GetName() == Name) {

				return Object.Cast<UEType>();
			}
		}

		return UEType();
	}

	static UEClass FindClassFast(std::string Name);

	template<typename UEType = UEObject> static UEType FindObjectFastInOuter(std::string Name, std::string Outer);

	//template<typename UEType> inline UEType FindObjectFast__inline(std::string Name, EClassCastFlags RequiredType) {

	//	auto ObjArray = ObjectArray();

	//	for (UEObject Object : ObjArray) {

	//		if (Object.IsA(RequiredType) && Object.GetName() == Name) {

	//			return Object.Cast<UEType>();
	//		}
	//	}

	//	return UEType();
	//}

public:

	class ObjectsIterator {

		ObjectArray& IteratedArray;

		UEObject CurrentObject;

		int32 CurrentIndex;

	public:

		ObjectsIterator(ObjectArray& Array, int32 StartIndex = 0);

		UEObject operator*();

		ObjectsIterator& operator++();

		bool operator!=(const ObjectsIterator& Other);

		int32 GetIndex() const;
	};

	ObjectsIterator begin();

	ObjectsIterator end();
};