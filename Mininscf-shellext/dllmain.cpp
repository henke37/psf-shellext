#include "common.h"
#include "globals.h"

HINSTANCE dllHinstance;

DllExport BOOL WINAPI DllMain(
  _In_ HINSTANCE hinstDLL,
  _In_ DWORD     fdwReason,
  _In_ LPVOID    lpvReserved
) {
	if(fdwReason==DLL_PROCESS_ATTACH) {
		dllHinstance=hinstDLL;
		DisableThreadLibraryCalls(hinstDLL);
	}
	return TRUE;
}
