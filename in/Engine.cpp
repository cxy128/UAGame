#include "Engine.h"
#include "Hook.h"

namespace Engine {

	bool FindFunction() {

		UECanvas::K2_DrawBox_Function = ObjectArray::FindObjectFast("K2_DrawBox").GetAddress();
		if (!UECanvas::K2_DrawBox_Function) {
			return false;
		}

		UECanvas::K2_DrawLine_Function = ObjectArray::FindObjectFast("K2_DrawLine").GetAddress();
		if (!UECanvas::K2_DrawLine_Function) {
			return false;
		}

		UECanvas::K2_DrawText_Function = ObjectArray::FindObjectFast("K2_DrawText").GetAddress();
		if (!UECanvas::K2_DrawText_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction UCanvas %p %p %p \n", UECanvas::K2_DrawLine_Function, UECanvas::K2_DrawLine_Function, UECanvas::K2_DrawLine_Function);

		UEGameplayStatics::ProjectWorldToScreen_Function = ObjectArray::FindObjectFast("ProjectWorldToScreen").GetAddress();
		if (!UEGameplayStatics::ProjectWorldToScreen_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction UGameplayStatics %p \n", UEGameplayStatics::ProjectWorldToScreen_Function);

		ASGCharacter::GetAllActorsOfClass_Function = ObjectArray::FindObjectFast("GetAllActorsOfClass").GetAddress();
		if (!ASGCharacter::GetAllActorsOfClass_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction ASGCharacter %p \n", ASGCharacter::GetAllActorsOfClass_Function);

		UESceneComponent::GetAllSocketNames_Function = ObjectArray::FindObjectFast("GetAllSocketNames").GetAddress();
		if (!UESceneComponent::GetAllSocketNames_Function) {
			return false;
		}

		UESceneComponent::GetSocketTransform_Function = ObjectArray::FindObjectFast("GetSocketTransform").GetAddress();
		if (!UESceneComponent::GetSocketTransform_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction USceneComponent %p %p \n", UESceneComponent::GetAllSocketNames_Function, UESceneComponent::GetSocketTransform_Function);

		APlayerController::ProjectWorldLocationToScreen_Function = ObjectArray::FindObjectFast("ProjectWorldLocationToScreen").GetAddress();
		if (!APlayerController::ProjectWorldLocationToScreen_Function) {
			return false;
		}

		APlayerController::LineOfSightTo_Function = ObjectArray::FindObjectFast("LineOfSightTo").GetAddress();
		if (!APlayerController::LineOfSightTo_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction APlayerController %p %p \n", APlayerController::ProjectWorldLocationToScreen_Function, APlayerController::LineOfSightTo_Function);

		APlayerCameraManager::GetCameraLocation_Function = ObjectArray::FindObjectFastInOuter("GetCameraLocation", "PlayerCameraManager").GetAddress();
		if (!APlayerCameraManager::GetCameraLocation_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction APlayerCameraManager %p \n", APlayerCameraManager::GetCameraLocation_Function);

		APawn::GetController_Function = ObjectArray::FindObjectFastInOuter("GetController", "Pawn").GetAddress();
		if (!APawn::GetController_Function) {
			return false;
		}

		APawn::IsBotControlled_Function = ObjectArray::FindObjectFastInOuter("IsBotControlled", "Pawn").GetAddress();
		if (!APawn::IsBotControlled_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction APawn %p %p \n", APawn::GetController_Function, APawn::IsBotControlled_Function);

		UESGActorStatics::GetHealth_Function = ObjectArray::FindObjectFastInOuter("GetHealth", "SGActorStatics").GetAddress();
		if (!UESGActorStatics::GetHealth_Function) {
			return false;
		}

		UESGActorStatics::GetHealthMax_Function = ObjectArray::FindObjectFastInOuter("GetHealthMax", "SGActorStatics").GetAddress();
		if (!UESGActorStatics::GetHealthMax_Function) {
			return false;
		}

		UESGActorStatics::IsDead_Function = ObjectArray::FindObjectFastInOuter("IsDead", "SGActorStatics").GetAddress();
		if (!UESGActorStatics::IsDead_Function) {
			return false;
		}

		UESGActorStatics::IsInBattle_Function = ObjectArray::FindObjectFastInOuter("IsInBattle", "SGActorStatics").GetAddress();
		if (!UESGActorStatics::IsInBattle_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction USGActorStatics %p %p \n", UESGActorStatics::GetHealth_Function, UESGActorStatics::GetHealthMax_Function);

		Print("--> UAGame.exe FindFunction USGActorStatics %p %p \n", UESGActorStatics::IsDead_Function, UESGActorStatics::IsInBattle_Function);

		UESGCharacterStatics::GetActorCharacterType_Function = ObjectArray::FindObjectFastInOuter("GetActorCharacterType", "SGCharacterStatics").GetAddress();
		if (!UESGCharacterStatics::GetActorCharacterType_Function) {
			return false;
		}

		UESGCharacterStatics::GetPlayerName_Function = ObjectArray::FindObjectFastInOuter("GetPlayerName", "SGCharacterStatics").GetAddress();
		if (!UESGCharacterStatics::GetPlayerName_Function) {
			return false;
		}

		UESGCharacterStatics::GetPlayerState_Function = ObjectArray::FindObjectFastInOuter("GetPlayerState", "SGCharacterStatics").GetAddress();
		if (!UESGCharacterStatics::GetPlayerState_Function) {
			return false;
		}

		UESGCharacterStatics::GetTeamIndex_Function = ObjectArray::FindObjectFastInOuter("GetTeamIndex", "SGCharacterStatics").GetAddress();
		if (!UESGCharacterStatics::GetTeamIndex_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction USGCharacterStatics %p %p \n", UESGCharacterStatics::GetActorCharacterType_Function, UESGCharacterStatics::GetPlayerName_Function);

		Print("--> UAGame.exe FindFunction USGCharacterStatics %p %p \n", UESGCharacterStatics::GetPlayerState_Function, UESGCharacterStatics::GetTeamIndex_Function);

		APlayerState::GetPlayerName_Function = ObjectArray::FindObjectFastInOuter("GetPlayerName", "PlayerState").GetAddress();
		if (!APlayerState::GetPlayerName_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction APlayerState %p \n", APlayerState::GetPlayerName_Function);

		UESGTeamStatics::GetTeamIndex_Function = ObjectArray::FindObjectFastInOuter("GetTeamIndex", "SGTeamStatics").GetAddress();
		if (!UESGTeamStatics::GetTeamIndex_Function) {
			return false;
		}

		UESGTeamStatics::IsTeammate_Function = ObjectArray::FindObjectFastInOuter("IsTeammate", "SGTeamStatics").GetAddress();
		if (!UESGTeamStatics::IsTeammate_Function) {
			return false;
		}

		Print("--> UAGame.exe FindFunction USGTeamStatics %p %p \n", UESGTeamStatics::GetTeamIndex_Function, UESGTeamStatics::IsTeammate_Function);

		return true;
	}

	bool FindObject() {

		UAGameEngine = ObjectArray::FindObjectFast<UEEngine>("UAGameEngine_2147482570");
		if (!UAGameEngine.GetAddress()) {
			return false;
		}

		Default__GameplayStatics = ObjectArray::FindObjectFast<UEGameplayStatics>("Default__GameplayStatics");
		if (!Default__GameplayStatics.GetAddress()) {
			return false;
		}

		SGCharacter = ObjectArray::FindObjectFast<ASGCharacter>("SGCharacter");
		if (!SGCharacter.GetAddress()) {
			return false;
		}

		Default__SGActorStatics = ObjectArray::FindObjectFast<UESGActorStatics>("Default__SGActorStatics");
		if (!Default__SGActorStatics.GetAddress()) {
			return false;
		}

		Default__SGCharacterStatics = ObjectArray::FindObjectFast<UESGCharacterStatics>("Default__SGCharacterStatics");
		if (!Default__SGCharacterStatics.GetAddress()) {
			return false;
		}

		Default__SGTeamStatics = ObjectArray::FindObjectFast<UESGTeamStatics>("Default__SGTeamStatics");
		if (!Default__SGTeamStatics.GetAddress()) {
			return false;
		}

		EngineWorld = ObjectArray::FindObjectFastInOuter<UEClass>("World", "Engine");
		if (!EngineWorld.GetAddress()) {
			return false;
		}

		EngineGameStateBase = ObjectArray::FindObjectFastInOuter<UEClass>("GameStateBase", "Engine");
		if (!EngineGameStateBase.GetAddress()) {
			return false;
		}

		EngineGameInstance = ObjectArray::FindObjectFastInOuter<UEClass>("GameInstance", "Engine");
		if (!EngineGameInstance.GetAddress()) {
			return false;
		}

		EngineLocalPlayer = ObjectArray::FindObjectFastInOuter<UEClass>("LocalPlayer", "Engine");
		if (!EngineLocalPlayer.GetAddress()) {
			return false;
		}

		EnginePlayerController = ObjectArray::FindObjectFastInOuter<UEClass>("PlayerController", "Engine");
		if (!EnginePlayerController.GetAddress()) {
			return false;
		}

		EnginePlayerCameraManager = ObjectArray::FindObjectFastInOuter<UEClass>("PlayerCameraManager", "Engine");
		if (!EnginePlayerCameraManager.GetAddress()) {
			return false;
		}

		EnginePawn = ObjectArray::FindObjectFastInOuter<UEClass>("Pawn", "Engine");
		if (!EnginePawn.GetAddress()) {
			return false;
		}

		DebugCanvasObject = ObjectArray::FindObjectFastInOuter<UECanvas>("DebugCanvasObject", "Transient");
		if (!DebugCanvasObject.GetAddress()) {
			return false;
		}

		Print("--> UAGame.exe FindObject 0x%llX 0x%llX 0x%llX \n", UAGameEngine.GetValue(),  SGCharacter.GetValue(), DebugCanvasObject.GetValue());

		Print("--> UAGame.exe FindObject 0x%llX 0x%llX 0x%llX \n", Default__GameplayStatics.GetValue(), Default__SGActorStatics.GetValue(), Default__SGCharacterStatics.GetValue());

		Print("--> UAGame.exe FindObject 0x%llX 0x%llX 0x%llX \n", Default__SGTeamStatics.GetValue(), EngineGameStateBase.GetValue(), EngineGameInstance.GetValue());

		Print("--> UAGame.exe FindObject 0x%llX 0x%llX 0x%llX \n", EngineLocalPlayer.GetValue(), EngineWorld.GetValue(), EnginePlayerController.GetValue());

		Print("--> UAGame.exe FindObject 0x%llX 0x%llX \n", EnginePlayerCameraManager.GetValue(), EnginePawn.GetValue());

		return true;
	}

	void FindOffset() {

		Offset::UEngine::GameViewport = ObjectArray::FindObjectFastInOuter<UEClass>("Engine", "Engine").FindMember("GameViewport").GetOffset();
		Print("--> UAGame.exe FindOffset UEngine::GameViewport 0x%lX \n", Offset::UEngine::GameViewport);

		Offset::UGameViewportClient::World = ObjectArray::FindObjectFastInOuter<UEClass>("GameViewportClient", "Engine").FindMember("World").GetOffset();
		Print("--> UAGame.exe FindOffset UGameViewportClient::World 0x%lX \n", Offset::UGameViewportClient::World);

		Offset::UWorld::PersistentLevel = EngineWorld.FindMember("PersistentLevel").GetOffset();
		Print("--> UAGame.exe FindOffset UWorld::PersistentLevel 0x%lX \n", Offset::UWorld::PersistentLevel);

		Offset::UWorld::GameState = EngineWorld.FindMember("GameState").GetOffset();
		Print("--> UAGame.exe FindOffset UWorld::GameState 0x%lX \n", Offset::UWorld::GameState);

		Offset::UWorld::OwningGameInstance = EngineWorld.FindMember("OwningGameInstance").GetOffset();
		Print("--> UAGame.exe FindOffset UWorld::OwningGameInstance 0x%lX \n", Offset::UWorld::OwningGameInstance);

		Offset::AGameStateBase::PlayerArray = EngineGameStateBase.FindMember("PlayerArray").GetOffset();
		Print("--> UAGame.exe FindOffset AGameStateBase::PlayerArray 0x%lX \n", Offset::AGameStateBase::PlayerArray);

		Offset::UGameInstance::LocalPlayers = EngineGameInstance.FindMember("LocalPlayers").GetOffset();
		Print("--> UAGame.exe FindOffset UGameInstance::LocalPlayers 0x%lX \n", Offset::UGameInstance::LocalPlayers);

		Offset::ULocalPlayer::PlayerController = ObjectArray::FindObjectFastInOuter<UEClass>("Player", "Engine").FindMember("PlayerController").GetOffset();
		Print("--> UAGame.exe FindOffset ULocalPlayer::PlayerController 0x%lX \n", Offset::ULocalPlayer::PlayerController);

		Offset::ULocalPlayer::ViewportClient = EngineLocalPlayer.FindMember("ViewportClient").GetOffset();
		Print("--> UAGame.exe FindOffset ULocalPlayer::ViewportClient 0x%lX \n", Offset::ULocalPlayer::ViewportClient);

		Offset::APlayerController::Pawn = ObjectArray::FindObjectFastInOuter<UEClass>("Controller", "Engine").FindMember("Pawn").GetOffset();
		Print("--> UAGame.exe FindOffset APlayerController::Pawn 0x%lX \n", Offset::APlayerController::Pawn);

		Offset::APlayerController::PlayerCameraManager = ObjectArray::FindObjectFastInOuter<UEClass>("PlayerController", "Engine").FindMember("PlayerCameraManager").GetOffset();
		Print("--> UAGame.exe FindOffset APlayerController::PlayerCameraManager 0x%lX \n", Offset::APlayerController::PlayerCameraManager);

		Offset::APlayerCameraManager::CameraCachePrivate = ObjectArray::FindObjectFastInOuter<UEClass>("PlayerCameraManager", "Engine").FindMember("CameraCachePrivate").GetOffset();
		Print("--> UAGame.exe FindOffset APlayerCameraManager::CameraCachePrivate 0x%lX \n", Offset::APlayerCameraManager::CameraCachePrivate);

		Offset::APawn::PlayerState = ObjectArray::FindObjectFastInOuter<UEClass>("Pawn", "Engine").FindMember("PlayerState").GetOffset();
		Print("--> UAGame.exe FindOffset APawn::PlayerState 0x%lX \n", Offset::APawn::PlayerState);

		Offset::APawn::Controller = ObjectArray::FindObjectFastInOuter<UEClass>("Pawn", "Engine").FindMember("Controller").GetOffset();
		Print("--> UAGame.exe FindOffset APawn::Controller 0x%lX \n", Offset::APawn::Controller);
	}

	bool FindFont() {

		UEFont::DefaultRegularFont = reinterpret_cast<UEFont*>(ObjectArray::FindObjectFast("DefaultRegularFont").GetAddress());
		if (!UEFont::DefaultRegularFont) {
			return false;
		}

		Print("--> UAGame.exe FindFont %p \n", UEFont::DefaultRegularFont);

		return true;
	}

	bool Init() {

		Hwnd = FindWindowA("UnrealWindow", nullptr);

		ImageBase = reinterpret_cast<uint64>(GetModuleHandleA(nullptr));

		RECT rc = {};
		GetClientRect(Hwnd, &rc);

		Width = float(rc.right - rc.left);
		Height = float(rc.bottom - rc.top);

		ScreenCenter.X = Width * 0.5f;
		ScreenCenter.Y = Height * 0.5f;

		FindObject();

		FindOffset();

		FindFunction();

		FindFont();

		return true;
	}

	void K2_StrokeLine(const FVector2D& P1, const FVector2D& P2, const FLinearColor& Color, float LineThickness) {

		DebugCanvasObject.K2_DrawLine(P1, P2, LineThickness, Color);
	}

	void K2_StrokeText(const FString& RenderText, const FLinearColor& Color, const FVector2D& Pos, float ScaleValue, bool bCenter, bool bOutline) {

		FVector2D Scale(ScaleValue, ScaleValue);
		FVector2D ShadowOffset(1.0f, 1.0f);

		FLinearColor ShadowColor(0.0f, 0.0f, 0.0f, 1.0f);
		FLinearColor OutlineColor(0.0f, 0.0f, 0.0f, 1.0f);

		DebugCanvasObject.K2_DrawText(UEFont::DefaultRegularFont, RenderText, Pos, Scale, Color, 1.0f, ShadowColor, ShadowOffset, bCenter, bCenter, bOutline, OutlineColor);
	}

	void K2_DrawBox(const FVector2D& ScreenPosition, const FVector2D& ScreenSize, const FLinearColor& RenderColor, float Thickness) {

		DebugCanvasObject.K2_DrawBox(ScreenPosition, ScreenSize, Thickness, RenderColor);
	}
}