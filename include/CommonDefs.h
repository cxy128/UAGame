#pragma once

enum class FILTER_INPUT_NUMBER : ULONG {

	CreatePuppetProcess,
	DeletePuppetProcess,

	CreatePuppetHandle,
	DeletePuppetHandle
};

struct FILTER_INPUT_BUFFER {

	FILTER_INPUT_NUMBER Number;

	void* data;
};

struct CREATE_PUPPET_PROCESS_INPUT_BUFFER {

	void* PuppetProcessId;

	void* GameProcessId;
};

struct CREATE_PUPPET_PROCESS_OUTPUT_BUFFER {

	void* PuppetHandle;

	unsigned __int64 ImageBase;
};
