#pragma once

#include <Windows.h>
#include "UnrealTypes.h"
#include "util.h"

namespace UAGame {

	inline void(*PostRender)(void* GameViewport, void* Canvas) = nullptr;

	inline void(*Draw)(void* GameViewport, void* InViewport, void* SceneCanvas) = nullptr;

	bool bootstrap();

	bool ExchangePointer();

	void fDraw(void* GameViewport, void* InViewport, void* SceneCanvas);

	void fPostRender(void* GameViewport, void* Canvas);
}
