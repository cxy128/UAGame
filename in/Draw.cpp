#include "Draw.h"
#include "Player.h"
#include "Engine.h"

void Render() {

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		IsDrawWindow = !IsDrawWindow;
	}

	if (GetAsyncKeyState(VK_F1) & 1) {
		IsShowBone = !IsShowBone;
	}

	if (GetAsyncKeyState(VK_F2) & 1) {
		IsShowName = !IsShowName;
	}

	if (GetAsyncKeyState(VK_F3) & 1) {
		IsShowDist = !IsShowDist;
	}

	if (GetAsyncKeyState(VK_F4) & 1) {
		IsShowAura = !IsShowAura;
	}

	if (GetAsyncKeyState(VK_F5) & 1) {
		IsShowTeam = !IsShowTeam;
	}

	if (IsDrawWindow) {

		float StartX = 200.0f;
		float StartY = Engine::ScreenCenter.Y * 0.35f;

		Engine::K2_StrokeText(FString(std::format(L"[F1] 显示骨骼: {}", IsShowBone ? L"ON" : L"OFF").data()), IsShowBone ? OnColor : OffColor, FVector2D(StartX, StartY), 1.5f);

		StartY += LineHeight;

		Engine::K2_StrokeText(FString(std::format(L"[F2] 显示名字: {}", IsShowName ? L"ON" : L"OFF").data()), IsShowName ? OnColor : OffColor, FVector2D(StartX, StartY), 1.5f);

		StartY += LineHeight;

		Engine::K2_StrokeText(FString(std::format(L"[F3] 显示距离: {}", IsShowDist ? L"ON" : L"OFF").data()), IsShowDist ? OnColor : OffColor, FVector2D(StartX, StartY), 1.5f);

		StartY += LineHeight;

		Engine::K2_StrokeText(FString(std::format(L"[F4] 显示光环: {}", IsShowAura ? L"ON" : L"OFF").data()), IsShowAura ? OnColor : OffColor, FVector2D(StartX, StartY), 1.5f);

		StartY += LineHeight;

		Engine::K2_StrokeText(FString(std::format(L"[F5] 显示队伍: {}", IsShowTeam ? L"ON" : L"OFF").data()), IsShowTeam ? OnColor : OffColor, FVector2D(StartX, StartY), 1.5f);
	}

	Player::Start();
}
