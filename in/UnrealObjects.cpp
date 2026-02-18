#include "UnrealObjects.h"

UEObject::operator bool() const {

	return Object != nullptr && reinterpret_cast<void*>(Object + Offset::UObject::Class) != nullptr;
}

UEObject::operator uint8* () {

	return Object;
}

bool UEObject::operator==(const UEObject& Other) const {

	return Object == Other.Object;
}

bool UEObject::operator!=(const UEObject& Other) const {

	return Object != Other.Object;
}

void* UEObject::GetAddress() {

	return Object;
}

uint64 UEObject::GetValue() {

	return reinterpret_cast<uint64>(Object);
}

FName UEObject::GetFName() const {

	return FName(Object + Offset::UObject::Name);
}

int32 UEObject::GetIndex() const {

	return *reinterpret_cast<int32*>(Object + Offset::UObject::Index);
}

UEObject UEObject::GetOuter() const {

	return UEObject(*reinterpret_cast<void**>(Object + Offset::UObject::Outer));
}

std::string UEObject::GetName() const {

	return Object ? GetFName().ToString() : "None";
}

std::string UEObject::GetNameWithPath() const {

	return Object ? GetFName().ToRawString() : "None";
}

UEClass UEObject::GetClass() const {

	return UEClass(*reinterpret_cast<void**>(Object + Offset::UObject::Class));
}

std::string UEObject::GetFullName() const {

	if (*this) {

		std::string Temp;

		for (UEObject Outer = GetOuter(); Outer; Outer = Outer.GetOuter()) {
			Temp = Outer.GetName() + "." + Temp;
		}

		std::string Name = GetClass().GetName();
		Name += " ";
		Name += Temp;
		Name += GetName();

		return Name;
	}

	return "None";
}

std::string UEObject::GetPathName() const {

	if (*this) {

		std::string Temp;

		for (UEObject Outer = GetOuter(); Outer; Outer = Outer.GetOuter()) {
			Temp = Outer.GetNameWithPath() + "." + Temp;
		}

		std::string Name = GetClass().GetNameWithPath();
		Name += " ";
		Name += Temp;
		Name += GetNameWithPath();

		return Name;
	}

	return "None";
}

EObjectFlags UEObject::GetFlags() const {

	return Object ? *reinterpret_cast<EObjectFlags*>(Object + Offset::UObject::Flags) : EObjectFlags();
}

bool UEObject::IsA(EClassCastFlags TypeFlags) const {

	return (TypeFlags != EClassCastFlags::None ? GetClass().IsType(TypeFlags) : true);
}

bool UEObject::IsA(UEClass Class) const {

	if (!Class) {

		return false;
	}

	for (UEClass Clss = GetClass(); Clss; Clss = Clss.GetSuper().Cast<UEClass>()) {

		if (Clss == Class) {

			return true;
		}
	}

	return false;
}

bool UEObject::HasAnyFlags(EObjectFlags Flags) const {

	return GetFlags() & Flags;
}

UEStruct UEStruct::GetSuper() const {

	return UEStruct(*reinterpret_cast<void**>(Object + Offset::UStruct::SuperStruct));
}

EClassCastFlags UEClass::GetCastFlags() const {

	return *reinterpret_cast<EClassCastFlags*>(Object + Offset::UClass::CastFlags);
}

bool UEClass::IsType(EClassCastFlags TypeFlag) const {

	return (TypeFlag != EClassCastFlags::None ? (GetCastFlags() & TypeFlag) : true);
}

class UEGameViewportClient UEEngine::GetGameViewport() const {

	return UEGameViewportClient(*reinterpret_cast<void**>(Object + Offset::UEngine::GameViewport));
}

class UEWorld UEGameViewportClient::GerWorld() const {

	return UEWorld(*reinterpret_cast<void**>(Object + Offset::UGameViewportClient::World));
}

class UEUameInstance UEWorld::GetOwningGameInstance() const {

	return UEUameInstance(*reinterpret_cast<void**>(Object + Offset::UWorld::OwningGameInstance));
}

class ASGCharacter UEUameInstance::GetPawn() const {

	auto PlayerController = GetPlayerController();
	if (!PlayerController.GetAddress()) {
		return ASGCharacter();
	}

	return ASGCharacter(*reinterpret_cast<void**>(reinterpret_cast<uint64>(PlayerController.GetAddress()) + Offset::APlayerController::Pawn));
}

class APlayerController UEUameInstance::GetPlayerController() const {

	auto LocalPlayers = reinterpret_cast<TArray<uint64>*>(Object + Offset::UGameInstance::LocalPlayers);
	if (!LocalPlayers->Data) {
		return APlayerController();
	}

	return APlayerController(*reinterpret_cast<void**>(LocalPlayers->Data[0] + Offset::ULocalPlayer::PlayerController));
}

bool UEGameplayStatics::ProjectWorldToScreen(uint64 PlayerController, FVector& WorldPosition, FVector2D* ScreenPosition, bool bPlayerViewportRelative) {

	struct GameplayStatics_ProjectWorldToScreen final {
	public:
		uint64 PlayerController;
		FVector WorldPosition;
		FVector2D ScreenPosition;
		bool bPlayerViewportRelative;
		bool ReturnValue;
		uint8 Pad_1E[0x2];
	};

	GameplayStatics_ProjectWorldToScreen Parms{};

	Parms.PlayerController = PlayerController;
	Parms.WorldPosition = std::move(WorldPosition);
	Parms.bPlayerViewportRelative = bPlayerViewportRelative;

	ProcessEvent(ProjectWorldToScreen_Function, &Parms);

	if (ScreenPosition != nullptr) {
		*ScreenPosition = std::move(Parms.ScreenPosition);
	}

	return Parms.ReturnValue;
}

TArray<uint64> UESceneComponent::GetAllSocketNames() const {

	struct SceneComponent_GetAllSocketNames final {
	public:
		TArray<uint64> ReturnValue;
	};

	SceneComponent_GetAllSocketNames Parms{};

	ProcessEvent(GetAllSocketNames_Function, &Parms);

	return Parms.ReturnValue;
}

FTransform UESceneComponent::GetSocketTransform(uint64 InSocketName, ERelativeTransformSpace TransformSpace) const {

	struct SceneComponent_GetSocketTransform final {
	public:
		uint64 InSocketName;
		ERelativeTransformSpace TransformSpace;
		uint8 Pad_9[0x7];
		FTransform ReturnValue;
	};

	SceneComponent_GetSocketTransform Parms{};

	Parms.InSocketName = InSocketName;
	Parms.TransformSpace = TransformSpace;

	ProcessEvent(GetSocketTransform_Function, &Parms);

	return Parms.ReturnValue;
}

void UECanvas::K2_DrawBox(const FVector2D& ScreenPosition, const FVector2D& ScreenSize, float Thickness, const FLinearColor& RenderColor) {

	struct Canvas_K2_DrawBox final {
	public:
		FVector2D ScreenPosition;
		FVector2D ScreenSize;
		float Thickness;
		FLinearColor RenderColor;
	};

	Canvas_K2_DrawBox Parms{};

	Parms.ScreenPosition = ScreenPosition;
	Parms.ScreenSize = ScreenSize;
	Parms.Thickness = Thickness;
	Parms.RenderColor = RenderColor;

	ProcessEvent(K2_DrawBox_Function, &Parms);
}

void UECanvas::K2_DrawLine(FVector2D& ScreenPositionA, FVector2D& ScreenPositionB, float Thickness, FLinearColor& RenderColor) {

	struct Canvas_K2_DrawLine final {

	public:

		struct FVector2D ScreenPositionA;
		struct FVector2D ScreenPositionB;
		float Thickness;
		struct FLinearColor RenderColor;
	};

	Canvas_K2_DrawLine Parms{};

	Parms.ScreenPositionA = std::move(ScreenPositionA);
	Parms.ScreenPositionB = std::move(ScreenPositionB);
	Parms.Thickness = Thickness;
	Parms.RenderColor = std::move(RenderColor);

	ProcessEvent(K2_DrawLine_Function, &Parms);
}

void UECanvas::K2_DrawText(UEFont* RenderFont, const FString& RenderText, FVector2D& ScreenPosition, FVector2D& Scale, FLinearColor& RenderColor, float Kerning, FLinearColor& ShadowColor, FVector2D& ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor& OutlineColor) {

	struct Canvas_K2_DrawText final {

	public:
		class UEFont* RenderFont;
		class FString                                 RenderText;
		struct FVector2D                              ScreenPosition;
		struct FVector2D                              Scale;
		struct FLinearColor                           RenderColor;
		float                                         Kerning;
		struct FLinearColor                           ShadowColor;
		struct FVector2D                              ShadowOffset;
		bool                                          bCentreX;
		bool                                          bCentreY;
		bool                                          bOutlined;
		uint8                                         Pad_57[0x1];
		struct FLinearColor                           OutlineColor;
	};

	Canvas_K2_DrawText Parms{};

	Parms.RenderFont = RenderFont;
	Parms.RenderText = RenderText;
	Parms.ScreenPosition = std::move(ScreenPosition);
	Parms.Scale = std::move(Scale);
	Parms.RenderColor = std::move(RenderColor);
	Parms.Kerning = Kerning;
	Parms.ShadowColor = std::move(ShadowColor);
	Parms.ShadowOffset = std::move(ShadowOffset);
	Parms.bCentreX = bCentreX;
	Parms.bCentreY = bCentreY;
	Parms.bOutlined = bOutlined;
	Parms.OutlineColor = std::move(OutlineColor);

	ProcessEvent(K2_DrawText_Function, &Parms);
}

int32 UESGActorStatics::GetHealth(uint64 Actor) {

	struct SGActorStatics_GetHealth final {
	public:
		uint64 Actor;
		int32 ReturnValue;
		uint8 Pad_C[0x4];
	};

	SGActorStatics_GetHealth Parms{};

	Parms.Actor = Actor;

	ProcessEvent(GetHealth_Function, &Parms);

	return Parms.ReturnValue;
}

int32 UESGActorStatics::GetHealthMax(uint64 Actor) {

	struct SGActorStatics_GetHealthMax final {
	public:
		uint64 Actor;
		int32 ReturnValue;
		uint8 Pad_C[0x4];
	};

	SGActorStatics_GetHealthMax Parms{};

	Parms.Actor = Actor;

	ProcessEvent(GetHealthMax_Function, &Parms);

	return Parms.ReturnValue;
}

bool UESGActorStatics::IsDead(uint64 Actor) {

	struct SGActorStatics_IsDead final {
	public:
		uint64 Actor;
		bool ReturnValue;
		uint8 Pad_9[0x7];
	};

	SGActorStatics_IsDead Parms{};

	Parms.Actor = Actor;

	ProcessEvent(IsDead_Function, &Parms);

	return Parms.ReturnValue;
}

bool UESGActorStatics::IsInBattle(uint64 World) {

	struct SGActorStatics_IsInBattle final {
	public:
		uint64 World;
		bool ReturnValue;
		uint8 Pad_9[0x7];
	};

	SGActorStatics_IsInBattle Parms{};

	Parms.World = World;

	ProcessEvent(IsInBattle_Function, &Parms);

	return Parms.ReturnValue;
}

ECharacterType UESGCharacterStatics::GetActorCharacterType(uint64 Actor) {

	struct SGCharacterStatics_GetActorCharacterType final {
	public:
		uint64 Actor;
		ECharacterType ReturnValue;
		uint8 Pad_9[0x7];
	};

	SGCharacterStatics_GetActorCharacterType Parms{};

	Parms.Actor = Actor;

	ProcessEvent(GetActorCharacterType_Function, &Parms);

	return Parms.ReturnValue;
}

bool APlayerController::ProjectWorldLocationToScreen(FVector& WorldLocation, FVector2D* ScreenLocation, bool bPlayerViewportRelative) const {

	struct PlayerController_ProjectWorldLocationToScreen final {
	public:
		struct FVector                                WorldLocation;
		struct FVector2D                              ScreenLocation;
		bool                                          bPlayerViewportRelative;
		bool                                          ReturnValue;
		uint8                                         Pad_16[0x2];
	};

	PlayerController_ProjectWorldLocationToScreen Parms{};

	Parms.WorldLocation = std::move(WorldLocation);
	Parms.bPlayerViewportRelative = bPlayerViewportRelative;

	ProcessEvent(ProjectWorldLocationToScreen_Function, &Parms);

	if (ScreenLocation != nullptr) {
		*ScreenLocation = std::move(Parms.ScreenLocation);
	}

	return Parms.ReturnValue;
}

bool APlayerController::LineOfSightTo(uint64 Actor, const  FVector& ViewPoint, bool bAlternateChecks) const {

	struct Controller_LineOfSightTo final {
	public:
		uint64 Actor;
		FVector ViewPoint;
		bool bAlternateChecks;
		bool ReturnValue;
		uint8 Pad_16[0x2];
	};

	Controller_LineOfSightTo Parms{};

	Parms.Actor = Actor;
	Parms.ViewPoint = std::move(ViewPoint);
	Parms.bAlternateChecks = bAlternateChecks;

	ProcessEvent(LineOfSightTo_Function, &Parms);

	return Parms.ReturnValue;
}

void ASGCharacter::GetAllActorsOfClass(void* WorldContextObject, void* ActorClass, TArray<uint64>* OutActors) {

	struct GameplayStatics_GetAllActorsOfClass final {
	public:
		void* WorldContextObject;
		void* ActorClass;
		TArray<uint64> OutActors;
	};

	GameplayStatics_GetAllActorsOfClass Parms{};

	Parms.WorldContextObject = WorldContextObject;
	Parms.ActorClass = ActorClass;

	ProcessEvent(GetAllActorsOfClass_Function, &Parms);

	if (OutActors != nullptr) {
		*OutActors = std::move(Parms.OutActors);
	}
}

FVector APlayerCameraManager::GetCameraLocation() const {

	struct PlayerCameraManager_GetCameraLocation final {
	public:
		struct FVector ReturnValue;
	};

	PlayerCameraManager_GetCameraLocation Parms{};

	ProcessEvent(GetCameraLocation_Function, &Parms);

	return Parms.ReturnValue;
}

uint64 APawn::GetController() const {

	struct Pawn_GetController final {

	public:

		uint64 ReturnValue;
	};

	Pawn_GetController Parms{};

	ProcessEvent(GetController_Function, &Parms);

	return Parms.ReturnValue;
}

bool APawn::IsBotControlled() const {

	struct Pawn_IsBotControlled final {
	public:
		bool ReturnValue;
	};

	Pawn_IsBotControlled Parms{};

	ProcessEvent(IsBotControlled_Function, &Parms);

	return Parms.ReturnValue;
}