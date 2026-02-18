#pragma once

#include <Windows.h>
#include <string>
#include "util.h"

struct FVector2D final {

public:

	using UnderlayingType = float;

	float X;
	float Y;
};

struct FVector {

public:
	using UnderlayingType = float;

	float X;
	float Y;
	float Z;

	bool IsZero() const {
		return X == 0.0f && Y == 0.0f && Z == 0.0f;
	}

	float Distance(const FVector& other) const {
		float dx = other.X - X;
		float dy = other.Y - Y;
		float dz = other.Z - Z;
		return sqrtf(dx * dx + dy * dy + dz * dz);
	}
};

struct alignas(0x10) FPlane final : public FVector{
public:
	float W;
};

struct FMatrix final {
public:
	struct FPlane XPlane;
	struct FPlane YPlane;
	struct FPlane ZPlane;
	struct FPlane WPlane;
};

struct FLinearColor final {

public:

	float R;
	float G;
	float B;
	float A;
};

struct alignas(0x10) FQuat final {
public:
	float X;
	float Y;
	float Z;
	float W;
};

struct FTransform final {
public:
	FQuat Rotation;
	FVector Translation;
	uint8 Pad_1C[0x4];
	FVector Scale3D;
	uint8 Pad_2C[0x4];
};

template<class T> class TArray {

public:

	T* Data;
	int NumElements;
	int MaxElements;

	inline bool IsValid() const {

		return Data != nullptr;
	}
};

class FString : public TArray<wchar_t> {

public:

	FString() = default;

	FString(const wchar_t* InBuffer) {

		Data = const_cast<wchar_t*>(InBuffer);

		NumElements = InBuffer ? static_cast<int>(wcslen(InBuffer)) + 1 : 0;

		MaxElements = NumElements;
	}

	inline std::string ToString() {

		if (IsValid()) {

			std::string TempName = {};

			WideCharToString(Data, TempName);

			return TempName;
		}

		return "";
	}
};

struct FRotator {

	float Pitch;
	float Yaw;
	float Roll;
};

struct FMinimalViewInfo {

	FVector Location;
	FRotator Rotation;
	float FOV;
};
