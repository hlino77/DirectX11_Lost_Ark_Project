#include "Engine_Defines.h"
#include <Windows.h>
#include <atomic>
#include "TLS.h"


BOOL APIENTRY DllMain(HINSTANCE hinstDLL, // DLL module handle
    DWORD fdwReason,                    // reason called
    LPVOID lpvReserved)                 // reserved
{
    LPVOID lpvData;
    BOOL fIgnore;

    switch (fdwReason)
    {
        // The DLL is loading due to process 
        // initialization or a call to LoadLibrary. 

    case DLL_PROCESS_ATTACH:

        // Allocate a TLS index.

        if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
            return FALSE;
 
        // No break: Initialize the index for first thread.

    // The attached process creates a new thread. 

    case DLL_THREAD_ATTACH:

        // Initialize the TLS index for this thread.

        lpvData = (LPVOID)LocalAlloc(LPTR, 256);
        if (lpvData != NULL)
            fIgnore = TlsSetValue(dwTlsIndex, lpvData);

        InitTLS();

        break;

        // The thread of the attached process terminates.

    case DLL_THREAD_DETACH:

        // Release the allocated memory for this thread.

        lpvData = TlsGetValue(dwTlsIndex);
        if (lpvData != NULL)
            LocalFree((HLOCAL)lpvData);

        break;

        // DLL unload due to process termination or FreeLibrary. 

    case DLL_PROCESS_DETACH:

        // Release the allocated memory for this thread.

        lpvData = TlsGetValue(dwTlsIndex);
        if (lpvData != NULL)
            LocalFree((HLOCAL)lpvData);

        // Release the TLS index.

        TlsFree(dwTlsIndex);
        break;

    default:
        break;
    }

    return TRUE;
    UNREFERENCED_PARAMETER(hinstDLL);
    UNREFERENCED_PARAMETER(lpvReserved);
}