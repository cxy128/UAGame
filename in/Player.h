#pragma once

#include <Windows.h>
#include <array>
#include <chrono>
#include <numbers>
#include <cmath>
#include "ObjectArray.h"
#include "Engine.h"
#include "util.h"

namespace Player {

	using namespace Engine;

	inline FLinearColor PlayerVisible = FLinearColor(0.00f, 0.95f, 0.85f, 1.0f);
	inline FLinearColor PlayerHidden = FLinearColor(0.55f, 0.35f, 1.00f, 1.0f);

	inline FLinearColor BotVisible = FLinearColor(0.70f, 0.70f, 0.70f, 1.0f);
	inline FLinearColor BotHidden = FLinearColor(0.35f, 0.35f, 0.35f, 1.0f);

	namespace Aura {

		inline int Segments = 72;

		inline float Radius = 100.0f;

		inline float Angle = 0.0f;

		inline float Speed = 10000.0f;

		inline FLinearColor Color;

		inline auto StartTime = std::chrono::high_resolution_clock::now();

		inline auto LastTime = std::chrono::high_resolution_clock::now();
	}

	enum EBone {

		Head,
		Neck_01,
		Spine_03,
		Spine_02,
		Spine_01,
		Pelvis,

		Hand_L,
		LowerArm_L,
		UpperArm_L,
		Clavicle_L,

		Clavicle_R,
		UpperArm_R,
		LowerArm_R,
		Hand_R,

		Foot_L,
		Calf_L,
		Thigh_L,

		Thigh_R,
		Calf_R,
		Foot_R,

		DataSize
	};

	class SocketIndex {

	public:

		std::array<uint32, DataSize> data;

		uint32 SocketNum;
	};

	class Render {

	public:

		uint64 Pawn;
		uint64 Actor;

		uint32 SocketNum;

		std::vector<bool> WorldToScreen;
		std::vector<FVector> SocketTranslation;
		std::vector<FVector2D> SocketScreen;

	public:

		Render(uint64 PawnValue, uint64 ActorValue);

		void DrawSocket(FLinearColor Color);

		bool DrawPartBone(uint32 Start, uint32 End, SocketIndex SocketIndexItem, FLinearColor Color) const;

		SocketIndex GetSocketIndex(uint32 Num);

		void DrawDistance();

		void DrawName(const FLinearColor& Color);

		void DrawAura();

		void DrawTeamId();

		void DrawHealth();
	};

	inline std::vector<SocketIndex> SocketIndexList;

	void Start();
}