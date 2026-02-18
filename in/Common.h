#pragma once

#include <Windows.h>

constexpr DWORD PAGE_SIZE = 0x1000;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	WCHAR* Buffer;
}UNICODE_STRING, * PUNICODE_STRING;

typedef struct _CLIENT_ID {
	VOID* UniqueProcess;
	VOID* UniqueThread;
}CLIENT_ID, * PCLIENT_ID;

typedef struct _PEB {
	UCHAR InheritedAddressSpace;
	UCHAR ReadImageFileExecOptions;
	UCHAR BeingDebugged;
	union {
		UCHAR BitField;
		struct {
			UCHAR ImageUsesLargePages : 1;
			UCHAR IsProtectedProcess : 1;
			UCHAR IsImageDynamicallyRelocated : 1;
			UCHAR SkipPatchingUser32Forwarders : 1;
			UCHAR IsPackagedProcess : 1;
			UCHAR IsAppContainer : 1;
			UCHAR IsProtectedProcessLight : 1;
			UCHAR IsLongPathAwareProcess : 1;
		};
	};
	UCHAR Padding0[4];
	VOID* Mutant;
	VOID* ImageBaseAddress;
}PEB, * PPEB;

typedef struct _TEB {
	NT_TIB NtTib;
	VOID* EnvironmentPointer;
	CLIENT_ID ClientId;
	VOID* ActiveRpcHandle;
	VOID* ThreadLocalStoragePointer;
	PEB* ProcessEnvironmentBlock;
	ULONG LastErrorValue;
	ULONG CountOfOwnedCriticalSections;
	VOID* CsrClientThread;
	VOID* Win32ThreadInfo;
}TEB, * PTEB;

using fnRtlInitUnicodeString = void (*)(PUNICODE_STRING DestinationString, PCWSTR SourceString);

using fnRtlCompareUnicodeString = LONG(*)(PUNICODE_STRING String1, PUNICODE_STRING String2, BOOLEAN CaseInSensitive);

inline fnRtlInitUnicodeString fRtlInitUnicodeString = nullptr;

inline fnRtlCompareUnicodeString fRtlCompareUnicodeString = nullptr;