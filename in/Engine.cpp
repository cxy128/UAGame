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

		Print("--> UAGame.exe FindFunction USGCharacterStatics %p \n", UESGCharacterStatics::GetActorCharacterType_Function);

		return true;
	}

	bool FindObject() {

		UAGameEngine = ObjectArray::FindObjectFast<UEEngine>("UAGameEngine_2147482570");
		if (!UAGameEngine.GetAddress()) {
			return false;
		}

		SGGameViewportClient = ObjectArray::FindObjectFast<UEGameViewportClient>("SGGameViewportClient_2147482420");
		if (!SGGameViewportClient.GetAddress()) {
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

		Print("--> UAGame.exe FindObject 0x%llX 0x%llX 0x%llX \n", UAGameEngine.GetValue(), SGGameViewportClient.GetValue(), SGCharacter.GetValue());

		Print("--> UAGame.exe FindObject 0x%llX 0x%llX 0x%llX \n", Default__GameplayStatics.GetValue(), Default__SGActorStatics.GetValue(), Default__SGCharacterStatics.GetValue());

		return true;
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

		FindFunction();

		FindFont();

		return true;
	}

	void K2_StrokeLine(FVector2D& P1, FVector2D& P2, FLinearColor& Color, float LineThickness) {

		Canvas.K2_DrawLine(P1, P2, LineThickness, Color);
	}

	void K2_StrokeText(const FString& RenderText, FLinearColor& Color, FVector2D& Pos, float ScaleValue, bool bCenter, bool bOutline) {

		FVector2D Scale(ScaleValue, ScaleValue);
		FVector2D ShadowOffset(1.0f, 1.0f);

		FLinearColor ShadowColor(0.0f, 0.0f, 0.0f, 1.0f);
		FLinearColor OutlineColor(0.0f, 0.0f, 0.0f, 1.0f);

		Canvas.K2_DrawText(UEFont::DefaultRegularFont, RenderText, Pos, Scale, Color, 1.0f, ShadowColor, ShadowOffset, bCenter, bCenter, bOutline, OutlineColor);
	}

	void K2_DrawBox(const FVector2D& ScreenPosition, const FVector2D& ScreenSize, const FLinearColor& RenderColor, float Thickness) {

		Canvas.K2_DrawBox(ScreenPosition, ScreenSize, Thickness, RenderColor);
	}
}