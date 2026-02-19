#include "Player.h"

namespace Player {

	Render::Render(uint64 PawnValue, uint64 ActorValue) {

		Pawn = PawnValue;

		Actor = ActorValue;

		UESceneComponent Mesh(*reinterpret_cast<uint64*>(Actor + Offset::ACharacter::Mesh));

		auto Names = Mesh.GetAllSocketNames();

		SocketNum = Names.NumElements;

		WorldToScreen.resize(SocketNum);
		SocketTranslation.resize(SocketNum);
		SocketScreen.resize(SocketNum);

		for (uint32 n = 0; n < SocketNum; n++) {

			auto Name = Names.Data[n];
			if (!Name) {
				continue;
			}

			FTransform Transform = Mesh.GetSocketTransform(Name, ERelativeTransformSpace::RTS_World);

			FVector2D Screen = {};
			bool IsProjectedScreen = Engine::Default__GameplayStatics.ProjectWorldToScreen(APawn(Pawn).GetController(), Transform.Translation, &Screen, false);

			WorldToScreen[n] = IsProjectedScreen;
			SocketTranslation[n] = Transform.Translation;
			SocketScreen[n] = Screen;
		}
	}

	void Render::DrawSocket(FLinearColor Color) {

		auto SocketIndexItem = GetSocketIndex(SocketNum);

		if (SocketIndexItem.SocketNum) {

			DrawPartBone(0, 6, SocketIndexItem, Color);
			DrawPartBone(6, 8, SocketIndexItem, Color);
			DrawPartBone(14, 6, SocketIndexItem, Color);
		}
	}

	bool Render::DrawPartBone(uint32 Start, uint32 End, SocketIndex SocketIndexItem, FLinearColor Color) const {

		FVector2D Point = {};
		FVector2D PrePoint = {};

		uint32 Index = 0;
		uint32 PreBoneIdx = 0;

		if (DataSize < Start + End) {

			return false;
		}

		for (uint32 i = Start; i < Start + End; i++) {

			Index = SocketIndexItem.data[i];

			if (Index != 0) {

				Point = SocketScreen[Index];

				if (i != Start && WorldToScreen[Index] && WorldToScreen[PreBoneIdx]) {

					Engine::K2_StrokeLine(PrePoint, Point, Color);
				}
			}

			PrePoint = Point;

			PreBoneIdx = Index;
		}

		return true;
	}

	SocketIndex Render::GetSocketIndex(uint32 Num) {

		for (size_t i = 0; i < SocketIndexList.size(); i++) {

			if (SocketIndexList[i].SocketNum == Num) {

				return SocketIndexList[i];
			}
		}

		UESceneComponent Mesh(*reinterpret_cast<uint64*>(Actor + Offset::ACharacter::Mesh));

		TArray<uint64> SocketNames = Mesh.GetAllSocketNames();

		SocketIndex SocketIndexItem = {};

		SocketIndexItem.SocketNum = SocketNames.NumElements;

		for (int i = 0; i < SocketNames.NumElements; i++) {

			auto SocketName = FName(&SocketNames.Data[i]).ToString();

			if (SocketName == "head") SocketIndexItem.data[Head] = i;
			else if (SocketName == "neck_01") SocketIndexItem.data[Neck_01] = i;

			else if (SocketName == "spine_03") SocketIndexItem.data[Spine_03] = i;
			else if (SocketName == "spine_02") SocketIndexItem.data[Spine_02] = i;
			else if (SocketName == "spine_01") SocketIndexItem.data[Spine_01] = i;
			else if (SocketName == "pelvis") SocketIndexItem.data[Pelvis] = i;

			else if (SocketName == "clavicle_l") SocketIndexItem.data[Clavicle_L] = i;
			else if (SocketName == "upperarm_l") SocketIndexItem.data[UpperArm_L] = i;
			else if (SocketName == "lowerarm_l") SocketIndexItem.data[LowerArm_L] = i;
			else if (SocketName == "hand_l") SocketIndexItem.data[Hand_L] = i;

			else if (SocketName == "clavicle_r") SocketIndexItem.data[Clavicle_R] = i;
			else if (SocketName == "upperarm_r") SocketIndexItem.data[UpperArm_R] = i;
			else if (SocketName == "lowerarm_r") SocketIndexItem.data[LowerArm_R] = i;
			else if (SocketName == "hand_r") SocketIndexItem.data[Hand_R] = i;

			else if (SocketName == "thigh_l") SocketIndexItem.data[Thigh_L] = i;
			else if (SocketName == "calf_l") SocketIndexItem.data[Calf_L] = i;
			else if (SocketName == "foot_l") SocketIndexItem.data[Foot_L] = i;

			else if (SocketName == "thigh_r") SocketIndexItem.data[Thigh_R] = i;
			else if (SocketName == "calf_r") SocketIndexItem.data[Calf_R] = i;
			else if (SocketName == "foot_r") SocketIndexItem.data[Foot_R] = i;
		}

		SocketIndexList.push_back(SocketIndexItem);

		return SocketIndexItem;
	}

	void Render::DrawDistance() {

		//auto PlayerController = *reinterpret_cast<uint64*>(Pawn + Offset::APawn::Controller);
		//if (!PlayerController) {
		//	return;
		//}

		//auto PlayerCameraManager = *reinterpret_cast<uint64*>(PlayerController + Offset::APlayerController::PlayerCameraManager);
		//if (!PlayerCameraManager) {
		//	return;
		//}

		//auto RootComponent = *reinterpret_cast<uint64*>(Actor + Offset::AActor::RootComponent);
		//if (!RootComponent) {
		//	return;
		//}

		//auto POV = reinterpret_cast<FMinimalViewInfo*>(PlayerCameraManager + Offset::FCameraCacheEntry::POV);

		//auto ActorRelativeLocation = *reinterpret_cast<FVector*>(RootComponent + Offset::USceneComponent::RelativeLocation);

		//float Distance = POV->Location.Distance(ActorRelativeLocation);

		auto SocketIndexItem = GetSocketIndex(SocketNum);
		if (!SocketIndexItem.SocketNum) {
			return;
		}

		auto HeadIndex = SocketIndexItem.data[Head];
		if (!WorldToScreen[HeadIndex]) {
			return;
		}

		auto PlayerController = *reinterpret_cast<uint64*>(Pawn + Offset::APawn::Controller);
		if (!PlayerController) {
			return;
		}

		auto PlayerCameraManager = *reinterpret_cast<uint64*>(PlayerController + Offset::APlayerController::PlayerCameraManager);
		if (!PlayerCameraManager) {
			return;
		}

		FVector CameraPos = APlayerCameraManager(PlayerCameraManager).GetCameraLocation();
		if (isnan(CameraPos.X) || isnan(CameraPos.Y)) {
			return;
		}

		FVector HeadTranslation = SocketTranslation[HeadIndex];
		if (isnan(HeadTranslation.X) || isnan(HeadTranslation.Y)) {
			return;
		}

		float Distance = CameraPos.Distance(HeadTranslation) / 100.0f;
		if (isnan(Distance)) {
			return;
		}

		FVector2D TextPos = SocketScreen[HeadIndex];
		TextPos.X -= 15.f;
		TextPos.Y -= 30.f;

		FLinearColor Color;

		if (Distance < 50.0f) {

			Color = FLinearColor(1.0f, 0.2f, 0.2f, 1.0f);

		} else if (Distance < 150.0f) {

			Color = FLinearColor(1.0f, 0.9f, 0.2f, 1.0f);

		} else {

			Color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
		}

		K2_StrokeText(FString(std::format(L"{:.0f}m", Distance).data()), Color, TextPos);
	}

	void Render::DrawName(const FLinearColor& Color) {

		auto SocketIndexItem = GetSocketIndex(SocketNum);
		if (!SocketIndexItem.SocketNum) {
			return;
		}

		auto HeadIndex = SocketIndexItem.data[Head];
		if (!WorldToScreen[HeadIndex]) {
			return;
		}

		auto PlayerState = Default__SGCharacterStatics.GetPlayerState(Actor);
		if (!PlayerState) {
			return;
		}

		FVector2D TextPos = SocketScreen[HeadIndex];
		TextPos.X -= 15.f;
		TextPos.Y -= 30.f + 30.0f;

		K2_StrokeText(APlayerState(PlayerState).GetPlayerName(), Color, TextPos);
	}

	void Render::DrawAura() {

		auto PlayerController = APawn(Pawn).GetController();
		if (!PlayerController) {
			return;
		}

		auto SocketIndexItem = GetSocketIndex(SocketNum);
		if (!SocketIndexItem.SocketNum) {
			return;
		}

		auto FootIndex = SocketIndexItem.data[Foot_R];
		if (!WorldToScreen[FootIndex]) {
			return;
		}

		FVector Center = SocketTranslation[FootIndex];

		auto Now = std::chrono::high_resolution_clock::now();

		float DeltaTime = std::chrono::duration<float>(Now - Aura::LastTime).count();

		Aura::LastTime = Now;

		Aura::Angle += 90.f * DeltaTime;
		if (Aura::Angle >= 360.f) {
			Aura::Angle -= 360.f;
		}

		auto duration = Now - Aura::StartTime;
		float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();

		Aura::Color.R = std::sin(Aura::Speed / 1500.0f * elapsedTime * 0.5f) * 0.5f + 0.5f;
		Aura::Color.B = std::sin(Aura::Speed / 1500.0f * elapsedTime + 2.0f * std::numbers::pi_v<float> / 3.0f) * 0.5f + 0.5f;
		Aura::Color.G = std::sin(Aura::Speed / 1500.0f * elapsedTime + 4.0f * std::numbers::pi_v<float> / 3.0f) * 0.5f + 0.5f;

		const float Step = 360.0f / Aura::Segments;

		FVector2D First{};
		FVector2D Prev{};
		bool HasFirst = false;

		for (int i = 0; i <= Aura::Segments; i++) {

			float AngleDeg = Aura::Angle + Step * i;
			float Rad = AngleDeg * std::numbers::pi_v<float> / 180.0f;

			FVector WorldPoint = {};
			WorldPoint.X = Center.X + std::cos(Rad) * Aura::Radius;
			WorldPoint.Y = Center.Y + std::sin(Rad) * Aura::Radius;
			WorldPoint.Z = Center.Z;

			FVector2D Screen = {};
			if (!Engine::Default__GameplayStatics.ProjectWorldToScreen(PlayerController, WorldPoint, &Screen, false)) {
				continue;
			}

			if (!HasFirst) {

				First = Screen;

				HasFirst = true;

			} else {

				Engine::K2_StrokeLine(Prev, Screen, Aura::Color);
			}

			Prev = Screen;
		}

		if (HasFirst) {

			Engine::K2_StrokeLine(Prev, First, Aura::Color);
		}
	}

	void Render::DrawTeamId() {

		auto SocketIndexItem = GetSocketIndex(SocketNum);
		if (!SocketIndexItem.SocketNum) {
			return;
		}

		auto HeadIndex = SocketIndexItem.data[Head];
		if (!WorldToScreen[HeadIndex]) {
			return;
		}

		auto PlayerState = Default__SGCharacterStatics.GetPlayerState(Actor);
		if (!PlayerState) {
			return;
		}

		auto TeamIndex = Default__SGTeamStatics.GetTeamIndex(PlayerState);

		FVector2D TextPos = SocketScreen[HeadIndex];
		TextPos.X -= 13.f;
		TextPos.Y -= 30.f + 30.0f + 30.0f;

		K2_StrokeText(FString(std::format(L"∂”ŒÈ: {}", TeamIndex).data()), FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), TextPos);
	}

	void Start() {

		auto GameViewport = UAGameEngine.GetGameViewport();
		if (!GameViewport.GetAddress()) {
			return;
		}

		auto World = GameViewport.GerWorld();
		if (!World.GetAddress()) {
			return;
		}

		if (!Default__SGActorStatics.IsInBattle(World.GetValue())) {
			return;
		}

		auto OwningGameInstance = World.GetOwningGameInstance();
		if (!OwningGameInstance.GetAddress()) {
			return;
		}

		auto Pawn = OwningGameInstance.GetPawn();
		if (!Pawn.GetAddress()) {
			return;
		}

		auto PlayerState = Default__SGCharacterStatics.GetPlayerState(Pawn.GetValue());
		if (!PlayerState) {
			return;
		}

		auto PlayerController = OwningGameInstance.GetPlayerController();
		if (!PlayerController.GetAddress()) {
			return;
		}

		TArray<uint64> Actors = {};
		SGCharacter.GetAllActorsOfClass(World.GetAddress(), SGCharacter.GetAddress(), &Actors);

		for (int i = 0; i < Actors.NumElements; i++) {

			auto Actor = APawn(Actors.Data[i]);
			if (!Actor.GetAddress()) {
				continue;
			}

			if (Actor.GetController() == PlayerController.GetValue()) {
				continue;
			}

			if (Default__SGTeamStatics.IsTeammate(PlayerState, Default__SGCharacterStatics.GetPlayerState(Actor.GetValue()))) {
				continue;
			}

			if (Default__SGActorStatics.IsDead(Actor.GetValue())) {
				continue;
			}

			Render PlayerRender(Pawn.GetValue(), Actor.GetValue());

			bool IsLineOfSightTo = PlayerController.LineOfSightTo(Actor.GetValue(), FVector(), false);

			bool IsBot = Default__SGCharacterStatics.GetActorCharacterType(Actor.GetValue()) == ECharacterType::ECharacterType_AI_SCAV;

			FLinearColor Color;

			if (IsLineOfSightTo) {

				Color = IsBot ? BotVisible : PlayerVisible;

			} else {

				Color = IsBot ? BotHidden : PlayerHidden;
			}

			if (IsShowBone) {

				PlayerRender.DrawSocket(Color);
			}

			if (IsShowName) {

				PlayerRender.DrawName(Color);
			}

			if (IsShowDist) {

				PlayerRender.DrawDistance();
			}

			if (IsShowAura) {

				PlayerRender.DrawAura();
			}

			if (IsShowTeam) {

				PlayerRender.DrawTeamId();
			}
		}
	}
}