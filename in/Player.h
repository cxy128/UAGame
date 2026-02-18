#pragma once

#include <Windows.h>
#include <array>
#include "ObjectArray.h"
#include "Engine.h"
#include "util.h"

namespace Player {

	using namespace Engine;

	inline FLinearColor PlayerVisible = FLinearColor(0.00f, 1.00f, 1.00f, 1.0f); 
	inline FLinearColor PlayerHidden = FLinearColor(0.60f, 0.00f, 1.00f, 1.0f); 

	inline FLinearColor BotVisible = FLinearColor(1.00f, 1.00f, 0.00f, 1.0f); 
	inline FLinearColor BotHidden = FLinearColor(1.00f, 0.40f, 0.00f, 1.0f);

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
	};

	inline std::vector<SocketIndex> SocketIndexList;

	void Start();
}