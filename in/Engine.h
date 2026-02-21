#pragma once

#include <Windows.h>
#include <string>
#include "UnrealContainers.h"
#include "ObjectArray.h"
#include "util.h"

namespace Engine {

	inline HWND Hwnd = nullptr;
	inline uint64 ImageBase = 0;

	inline float Width = 0;
	inline float Height = 0;
	inline FVector2D ScreenCenter;

	inline UEEngine UAGameEngine;
	inline UEGameplayStatics Default__GameplayStatics;
	inline ASGCharacter SGCharacter;
	inline UESGActorStatics Default__SGActorStatics;
	inline UESGCharacterStatics Default__SGCharacterStatics;
	inline UESGTeamStatics Default__SGTeamStatics;

	inline UEClass GameViewportClient;
	inline UEClass EngineWorld;
	inline UEClass EngineGameStateBase;
	inline UEClass EngineGameInstance;
	inline UEClass EngineLocalPlayer;
	inline UEClass EnginePlayerController;
	inline UEClass EnginePlayerCameraManager;
	inline UEClass EnginePawn;

	inline UECanvas DebugCanvasObject;

	bool Init();

	bool FindFunction();

	bool FindObject();

	void FindOffset();

	bool FindFont();

	void K2_StrokeLine(const FVector2D& P1, const FVector2D& P2, const FLinearColor& Color, float LineThickness = 1.0f);

	void K2_StrokeText(const FString& RenderText, const FLinearColor& Color, const FVector2D& Pos, float ScaleValue = 1.0f, bool bCenter = false, bool bOutline = true);

	void K2_DrawBox(const FVector2D& ScreenPosition, const FVector2D& ScreenSize, const FLinearColor& RenderColor, float Thickness = 0.0f);
}