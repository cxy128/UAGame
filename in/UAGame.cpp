#include "UAGame.h"
#include "ObjectArray.h"
#include "Engine.h"
#include "Hook.h"
#include "Draw.h"

namespace UAGame {

	using namespace Engine;

	bool bootstrap() {

		FName::Init();

		ObjectArray::Init();

		//ObjectArray::DumpObjects();

		Engine::Init();

		ExchangePointer();

		return true;
	}

	bool ExchangePointer() {

		auto [ImageBase, ImageSize] = GetImageBaseAndSize();

		auto JmpAddress = reinterpret_cast<uint8*>(FindPatternInRange("CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC", ImageBase, ImageSize));
		if (!JmpAddress) {
			return false;
		}

		auto JmpPageProtect = 0ul;
		if (!VirtualProtect(JmpAddress, PAGE_SIZE, PAGE_EXECUTE_READWRITE, &JmpPageProtect)) {
			return false;
		}

		JmpAddress[0] = 0x89;
		JmpAddress[1] = 0xC0;
		JmpAddress[2] = 0x89;
		JmpAddress[3] = 0xC0;
		JmpAddress[4] = 0xFF;
		JmpAddress[5] = 0x25;
		JmpAddress[6] = 0x00;
		JmpAddress[7] = 0x00;
		JmpAddress[8] = 0x00;
		JmpAddress[9] = 0x00;
		*reinterpret_cast<void**>(JmpAddress + 10) = fPostRender;

		if (!VirtualProtect(JmpAddress, PAGE_SIZE, JmpPageProtect, &JmpPageProtect)) {
			return false;
		}

		struct _T {

			PVOID* vft;
		};

		auto _t = reinterpret_cast<_T*>(UAGameEngine.GetGameViewport().GetValue());

		ULONG AccessProtection = 0;

		if (VirtualProtect(_t->vft, PAGE_SIZE, PAGE_READWRITE, &AccessProtection)) {

			//PostRender = reinterpret_cast<void (*)(void* GameViewport, void* Canvas)>(_InterlockedExchangePointer(&_t->vft[113], fPostRender));

			PostRender = reinterpret_cast<void (*)(void* GameViewport, void* Canvas)>(_InterlockedExchangePointer(&_t->vft[114], JmpAddress));

			//Draw = reinterpret_cast<void (*)(void* GameViewport, void* InViewport, void* SceneCanvas)>(_InterlockedExchangePointer(&_t->vft[], fDraw));

			VirtualProtect(_t->vft, PAGE_SIZE, AccessProtection, &AccessProtection);
		}

		Print("--> UAGame.exe PostRender %p %p \n", _t, PostRender);

		return true;
	}

	void fDraw(void* GameViewport, void* InViewport, void* SceneCanvas) {

		Render();

		Draw(GameViewport, InViewport, SceneCanvas);
	}

	void fPostRender(void* GameViewport, void* Canvas) {

		Render();

		PostRender(GameViewport, Canvas);
	}
}
