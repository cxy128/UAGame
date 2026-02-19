#pragma once

#include <Windows.h>
#include "util.h"
#include "UnrealContainers.h"

inline float LineHeight = 35.0f;

inline FLinearColor OnColor = FLinearColor(0.2f, 0.9f, 0.65f, 1.0f);
inline FLinearColor OffColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);

void Render();
