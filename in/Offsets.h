#pragma once

#include <Windows.h>
#include "util.h"

namespace Offset {

	inline uint32 GWorld = 0;

	inline uint32 ProcessEvent = 0x00C6A9F0;

	inline uint32 ProcessEventIdx = 0x00000046;

	namespace UEngine {

		inline ULONG GameViewport = 0x0788;
	}

	namespace UGameViewportClient {

		inline ULONG World = 0x0078;
	}

	namespace UWorld {

		inline ULONG PersistentLevel = 0x0030;

		inline ULONG GameState = 0x0120;

		inline ULONG OwningGameInstance = 0x0180;
	}

	namespace ULevel {

		inline ULONG Actors = 0x0098;
	}

	namespace AGameStateBase {

		inline ULONG PlayerArray = 0x0328;
	}

	namespace UGameInstance {

		inline ULONG LocalPlayers = 0x0038;
	}

	namespace ULocalPlayer {

		inline ULONG PlayerController = 0x0030;

		inline ULONG ViewportClient = 0x0070;
	}

	namespace APlayerController {

		inline ULONG Pawn = 0x0340;

		inline ULONG PlayerCameraManager = 0x03A8;
	}

	namespace APlayerCameraManager {

		inline ULONG CameraCachePrivate = 0x2090;
	}

	namespace FCameraCacheEntry {

		inline ULONG POV = 0x10;
	}

	namespace AActor {

		inline ULONG RootComponent = 0x0168;
	}

	namespace APawn {

		inline ULONG PlayerState = 0x0340;

		inline ULONG Controller = 0x0358;
	}

	namespace ACharacter {

		inline ULONG Mesh = 0x0380;
	}

	namespace USceneComponent {

		inline ULONG RelativeLocation = 0x016C;

		inline ULONG ComponentToWorld = 0x0210;
	}

	namespace USkeletalMeshComponent {

		inline ULONG SkeletalMesh = 0x0608;

		inline ULONG BoneSpaceTransforms = 0x0880;

		inline ULONG CachedBoneSpaceTransforms = 0x08E8;

		inline ULONG CachedComponentSpaceTransforms = 0x08F8;
	};

	namespace USkeletalMesh {

		inline ULONG Skeleton = 0x0090;
	}

	namespace USkeleton {

		inline ULONG BoneTree = 0x0038;

		inline ULONG RefLocalPoses = BoneTree + sizeof(void*) + sizeof(int32) + sizeof(int32);

		inline ULONG ReferenceSkeleton = 0x0038 + sizeof(void*) + sizeof(int32) + sizeof(int32) + sizeof(void*) + sizeof(int32) + sizeof(int32);
	}

	namespace FName {

		inline uint32 AppendString = 0x00;

		inline uint32 ComparisonIndex = 0;
		inline uint32 Number = 0;

		inline uint32 SizeOf = 0;
	}

	namespace FChunkedFixedUObjectArray {

		inline uint32 NumElementsPerChunk = 0x10000;

		inline uint32 Objects = 0x00;
		inline uint32 MaxElements = 0x10;
		inline uint32 NumElements = 0x14;
		inline uint32 MaxChunks = 0x18;
		inline uint32 NumChunks = 0x1C;
	};

	namespace FUObjectItem {

		inline uint32 Object = 0x0;

		inline uint32 SizeOf = 0x18;
	}

	namespace UObject {

		inline uint32 Vft;
		inline uint32 Flags;
		inline uint32 Index;
		inline uint32 Class;
		inline uint32 Name;
		inline uint32 Outer;
	}

	namespace UStruct {

		inline uint32 SuperStruct;
		inline uint32 Children;
		inline uint32 ChildProperties;
		inline uint32 Size;
		inline uint32 MinAlignment;
	}

	namespace UClass {

		inline uint32 CastFlags;
		inline uint32 ClassDefaultObject;
	}

	namespace UFunction {

		inline uint32 FunctionFlags = 0x00B0;
		inline uint32 ExecFunction = 0x00D8;
	}

	namespace Setting {

		inline bool IsObjectNameBeforeClass = false;

		inline bool UseFProperty = false;
	}

	constexpr int32 NotFound = -1;

	template<int Alignement = 4, typename T> inline int32 Find(const std::vector<std::pair<void*, T>>& ObjectValuePair, int MinOffset = 0x28, int MaxOffset = 0x1A0) {

		int32 HighestFoundOffset = MinOffset;

		bool bFoundOffset = false;

		for (int i = 0; i < ObjectValuePair.size(); i++) {

			if (ObjectValuePair[i].first == nullptr) {

				continue;
			}

			for (int j = HighestFoundOffset; j < MaxOffset; j += Alignement) {

				const T TypedValueAtOffset = *reinterpret_cast<T*>(static_cast<uint8*>(ObjectValuePair[i].first) + j);

				if (TypedValueAtOffset == ObjectValuePair[i].second && j >= HighestFoundOffset) {

					bFoundOffset = true;

					if (j > HighestFoundOffset) {

						HighestFoundOffset = j;

						i = 0;
					}

					j = MaxOffset;
				}
			}
		}

		return bFoundOffset ? HighestFoundOffset : NotFound;
	}

	template<bool CheckForVft = true> inline int32 GetValidPointer(const uint8* ObjA, const uint8* ObjB, int32 StartingOffset, int32 MaxOffset) {

		if (!IsUserAddressValid(ObjA) || !IsUserAddressValid(ObjB)) {

			return NotFound;
		}

		for (int j = StartingOffset; j <= MaxOffset; j += 0x8) {

			const bool bIsAValid = IsUserAddressValid(*reinterpret_cast<void* const*>(ObjA + j)) && (CheckForVft ? IsUserAddressValid(**reinterpret_cast<void** const*>(ObjA + j)) : true);

			const bool bIsBValid = IsUserAddressValid(*reinterpret_cast<void* const*>(ObjB + j)) && (CheckForVft ? IsUserAddressValid(**reinterpret_cast<void** const*>(ObjB + j)) : true);

			if (bIsAValid && bIsBValid) {

				return j;
			}
		}

		return NotFound;
	}
}