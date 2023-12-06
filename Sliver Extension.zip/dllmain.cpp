#include "pch.h"

#include <cstdint>
#include <stdlib.h>
#include <string.h>

#include "output.h"
#include "argparse.h"

Output* output;

// this function is required to be executed by the C2
extern "C" {
	__declspec(dllexport) int __cdecl Execute(char* argsBuffer, uint32_t bufferSize, goCallback callback);
}
int Execute(char* argsBuffer, uint32_t bufferSize, goCallback callback)
{
	size_t arg_count = 0;

	// parse the arguments first
	if (bufferSize > 0)
	{
		char** args = parse_args(argsBuffer, &arg_count);
	}

	// do your thang...

	// return the utput to the C2
	output = NewOutput(128, callback);
	append(output, "Executed!\n");
	return success(output);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
