#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include "UnrealContainers.h"
#include "util.h"

class FName {

private:

	inline static void(*AppendString)(const void*, FString&) = nullptr;

	inline static std::string(*ToStr)(const void* NameAddress) = nullptr;

private:

	const uint8* Address;

public:

	FName() = default;

	FName(const void* Ptr) : Address(static_cast<const uint8*>(Ptr)) {

	}

	FName(const uint64 Ptr) : Address(reinterpret_cast<const uint8*>(Ptr)) {

	}

public:

	const void* GetAddress() const;

	std::string ToString() const;

	std::string ToRawString() const;

public:

	static bool Init();
};

