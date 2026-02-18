#pragma once

#include <Windows.h>
#include <type_traits>
#include "util.h"

template<typename E> constexpr bool IsEnumFlags = std::is_enum_v<E> && std::is_unsigned_v<std::underlying_type_t<E>>;

template<typename E> requires IsEnumFlags<E> constexpr E operator|(E Left, E Right) {
	using U = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<U>(Left) | static_cast<U>(Right));
}

template<typename E> requires IsEnumFlags<E>constexpr E& operator|=(E& Left, E Right) {
	Left = Left | Right;
	return Left;
}

template<typename E> requires IsEnumFlags<E> constexpr bool operator&(E Left, E Right) {
	using U = std::underlying_type_t<E>;
	return (static_cast<U>(Left) & static_cast<U>(Right)) == static_cast<U>(Right);
}

enum class EClassCastFlags : uint64 {

	None = 0x0000000000000000,

	Field = 0x0000000000000001,
	Int8Property = 0x0000000000000002,
	Enum = 0x0000000000000004,
	Struct = 0x0000000000000008,
	ScriptStruct = 0x0000000000000010,
	Class = 0x0000000000000020,
	ByteProperty = 0x0000000000000040,
	IntProperty = 0x0000000000000080,
	FloatProperty = 0x0000000000000100,
	UInt64Property = 0x0000000000000200,
	ClassProperty = 0x0000000000000400,
	UInt32Property = 0x0000000000000800,
	InterfaceProperty = 0x0000000000001000,
	NameProperty = 0x0000000000002000,
	StrProperty = 0x0000000000004000,
	Property = 0x0000000000008000,
	ObjectProperty = 0x0000000000010000,
	BoolProperty = 0x0000000000020000,
	UInt16Property = 0x0000000000040000,
	Function = 0x0000000000080000,
	StructProperty = 0x0000000000100000,
	ArrayProperty = 0x0000000000200000,
	Int64Property = 0x0000000000400000,
	DelegateProperty = 0x0000000000800000,
	NumericProperty = 0x0000000001000000,
	MulticastDelegateProperty = 0x0000000002000000,
	ObjectPropertyBase = 0x0000000004000000,
	WeakObjectProperty = 0x0000000008000000,
	LazyObjectProperty = 0x0000000010000000,
	SoftObjectProperty = 0x0000000020000000,
	TextProperty = 0x0000000040000000,
	Int16Property = 0x0000000080000000,
	DoubleProperty = 0x0000000100000000,
	SoftClassProperty = 0x0000000200000000,
	Package = 0x0000000400000000,
	Level = 0x0000000800000000,
	Actor = 0x0000001000000000,
	PlayerController = 0x0000002000000000,
	Pawn = 0x0000004000000000,
	SceneComponent = 0x0000008000000000,
	PrimitiveComponent = 0x0000010000000000,
	SkinnedMeshComponent = 0x0000020000000000,
	SkeletalMeshComponent = 0x0000040000000000,
	Blueprint = 0x0000080000000000,
	DelegateFunction = 0x0000100000000000,
	StaticMeshComponent = 0x0000200000000000,
	MapProperty = 0x0000400000000000,
	SetProperty = 0x0000800000000000,
	EnumProperty = 0x0001000000000000,
	SparseDelegateFunction = 0x0002000000000000,
	MulticastInlineDelegateProperty = 0x0004000000000000,
	MulticastSparseDelegateProperty = 0x0008000000000000,
	FieldPathProperty = 0x0010000000000000,
	LargeWorldCoordinatesRealProperty = 0x0080000000000000,
	OptionalProperty = 0x0100000000000000,
	VValueProperty = 0x0200000000000000,
	VerseVMClass = 0x0400000000000000,
	VRestValueProperty = 0x0800000000000000,
};

enum class EObjectFlags : uint32 {

	NoFlags = 0x00000000,

	Public = 0x00000001,
	Standalone = 0x00000002,
	MarkAsNative = 0x00000004,
	Transactional = 0x00000008,
	ClassDefaultObject = 0x00000010,
	ArchetypeObject = 0x00000020,
	Transient = 0x00000040,

	MarkAsRootSet = 0x00000080,
	TagGarbageTemp = 0x00000100,

	NeedInitialization = 0x00000200,
	NeedLoad = 0x00000400,
	KeepForCooker = 0x00000800,
	NeedPostLoad = 0x00001000,
	NeedPostLoadSubobjects = 0x00002000,
	NewerVersionExists = 0x00004000,
	BeginDestroyed = 0x00008000,
	FinishDestroyed = 0x00010000,

	BeingRegenerated = 0x00020000,
	DefaultSubObject = 0x00040000,
	WasLoaded = 0x00080000,
	TextExportTransient = 0x00100000,
	LoadCompleted = 0x00200000,
	InheritableComponentTemplate = 0x00400000,
	DuplicateTransient = 0x00800000,
	StrongRefOnFrame = 0x01000000,
	NonPIEDuplicateTransient = 0x02000000,
	Dynamic = 0x04000000,
	WillBeLoaded = 0x08000000,
};

enum class ERelativeTransformSpace : uint8 {
	RTS_World = 0,
	RTS_Actor = 1,
	RTS_Component = 2,
	RTS_ParentBoneSpace = 3,
	RTS_MAX = 4,
};

enum class ECharacterType : uint8 {
	ECharacterType_None = 0,
	ECharacterType_PMC = 1,
	ECharacterType_SCAV = 2,
	ECharacterType_AI_SCAV = 3,
	ECharacterType_AI_SCAV_BOSS = 4,
	ECharacterType_AI_PMC = 5,
	ECharacterType_AI_ELIT = 6,
	ECharacterType_BOSS = 7,
	ECharacterType_AI_SCAV_Follower = 8,
	ECharacterType_MAX = 9,
};