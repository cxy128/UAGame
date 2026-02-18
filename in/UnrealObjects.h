#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <functional>
#include <type_traits>
#include "util.h"
#include "UnrealTypes.h"
#include "Enums.h"
#include "Offsets.h"

template<typename FuncType> inline FuncType GetVirtualFunction(void* ObjectInstance, int32 Index) {

	void** VTable = reinterpret_cast<void**>(*reinterpret_cast<void**>(ObjectInstance));

	return reinterpret_cast<FuncType>(VTable[Index]);
}

template<typename FuncType, typename... ParamTypes>	requires std::invocable<FuncType, ParamTypes...> inline auto CallGameFunction(FuncType Function, ParamTypes&&... Args) {

	return Function(std::forward<ParamTypes>(Args)...);
}

class UEObject {

protected:

	uint8* Object;

public:

	UEObject() = default;

	UEObject(void* NewObject) : Object(reinterpret_cast<uint8*>(NewObject)) {

	}

	UEObject(uint64 NewObject) : Object(reinterpret_cast<uint8*>(NewObject)) {

	}

	UEObject(const UEObject&) = default;

	explicit operator bool() const;

	explicit operator uint8* ();

	bool operator==(const UEObject& Other) const;

	bool operator!=(const UEObject& Other) const;

public:

	void ProcessEvent(void* Function, void* Parms) const {

		CallGameFunction(GetVirtualFunction<void(*)(void* Object, void* Function, void* Parms)>(this->Object, Offset::ProcessEventIdx), this->Object, Function, Parms);
	}

public:

	void* GetAddress();

	uint64 GetValue();

	FName GetFName() const;

	int32 GetIndex() const;

	UEObject GetOuter() const;

	std::string GetName() const;

	std::string GetNameWithPath() const;

	class UEClass GetClass() const;

	std::string GetFullName() const;

	std::string GetPathName() const;

	EObjectFlags GetFlags() const;

public:

	bool IsA(EClassCastFlags TypeFlags) const;

	bool IsA(UEClass Class) const;

	bool HasAnyFlags(EObjectFlags Flags) const;

public:

	template<typename UEType> inline UEType Cast() {

		return UEType(Object);
	}

	template<typename UEType> inline const UEType Cast() const {

		return UEType(Object);
	}
};

class UEField : public UEObject {

	using UEObject::UEObject;
};

class UEStruct : public UEField {

	using UEField::UEField;

public:

	UEStruct GetSuper() const;
};

class UEClass : public UEStruct {

	using UEStruct::UEStruct;

public:

	EClassCastFlags GetCastFlags() const;

	bool IsType(EClassCastFlags TypeFlag) const;
};

class UEEngine : public UEObject {

	using UEObject::UEObject;

public:

	class UEGameViewportClient GetGameViewport() const;
};

class UEGameViewportClient : public UEObject {

	using UEObject::UEObject;

public:

	class UEWorld GerWorld() const;
};

class UEWorld : public UEObject {

	using UEObject::UEObject;

public:

	class UEUameInstance GetOwningGameInstance() const;
};

class UEUameInstance : public UEObject {

	using UEObject::UEObject;

public:

	class ASGCharacter GetPawn() const;

	class APlayerController GetPlayerController() const;
};

class UEFont : public UEObject {

	using UEObject::UEObject;

public:

	inline static UEFont* DefaultRegularFont = nullptr;
};

class UEGameplayStatics : public UEObject {

	using UEObject::UEObject;

public:

	inline static void* ProjectWorldToScreen_Function = nullptr;

public:

	bool ProjectWorldToScreen(uint64 PlayerController, FVector& WorldPosition, FVector2D* ScreenPosition, bool bPlayerViewportRelative);
};

class UESceneComponent : public UEObject {

	using UEObject::UEObject;

public:

	inline static void* GetAllSocketNames_Function = nullptr;

	inline static void* GetSocketTransform_Function = nullptr;

public:

	TArray<uint64> GetAllSocketNames() const;

	FTransform GetSocketTransform(uint64 InSocketName, ERelativeTransformSpace TransformSpace) const;
};

class UECanvas :UEObject {

	using UEObject::UEObject;

public:

	inline static void* K2_DrawBox_Function = nullptr;

	inline static void* K2_DrawLine_Function = nullptr;

	inline static void* K2_DrawText_Function = nullptr;

public:

	void K2_DrawBox(const FVector2D& ScreenPosition, const FVector2D& ScreenSize, float Thickness, const FLinearColor& RenderColor);

	void K2_DrawLine(FVector2D& ScreenPositionA, FVector2D& ScreenPositionB, float Thickness, FLinearColor& RenderColor);

	void K2_DrawText(UEFont* RenderFont, const FString& RenderText, FVector2D& ScreenPosition, FVector2D& Scale, FLinearColor& RenderColor, float Kerning, FLinearColor& ShadowColor, FVector2D& ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor& OutlineColor);
};

class UESGActorStatics : public UEObject {

	using UEObject::UEObject;

public:

	inline static void* GetHealth_Function = nullptr;

	inline static void* GetHealthMax_Function = nullptr;

	inline static void* IsDead_Function = nullptr;

	inline static void* IsInBattle_Function = nullptr;

public:

	int32 GetHealth(uint64 Actor);

	int32 GetHealthMax(uint64 Actor);

	bool IsDead(uint64 Actor);

	bool IsInBattle(uint64 World);
};

class UESGCharacterStatics : public UEObject {

	using UEObject::UEObject;

public:

	inline static void* GetActorCharacterType_Function = nullptr;

public:

	ECharacterType GetActorCharacterType(uint64 Actor);
};

class APlayerController : public UEObject {

	using UEObject::UEObject;

public:

	inline static void* ProjectWorldLocationToScreen_Function = nullptr;

	inline static void* LineOfSightTo_Function = nullptr;

public:

	bool ProjectWorldLocationToScreen(FVector& WorldLocation, FVector2D* ScreenLocation, bool bPlayerViewportRelative) const;

	bool LineOfSightTo(uint64 Actor, const  FVector& ViewPoint, bool bAlternateChecks) const;
};

class ASGCharacter : public UEObject {

	using UEObject::UEObject;

public:

	inline static void* GetAllActorsOfClass_Function = nullptr;

public:

	void GetAllActorsOfClass(void* WorldContextObject, void* ActorClass, TArray<uint64>* OutActors);
};

class APlayerCameraManager : public UEObject {

	using UEObject::UEObject;

public:

	inline static void* GetCameraLocation_Function = nullptr;

public:

	FVector GetCameraLocation() const;
};

class APawn : public UEObject {

	using UEObject::UEObject;

public:

	inline static void* GetController_Function = nullptr;

	inline static void* IsBotControlled_Function = nullptr;

public:

	uint64 GetController() const;

	bool IsBotControlled() const;
};

