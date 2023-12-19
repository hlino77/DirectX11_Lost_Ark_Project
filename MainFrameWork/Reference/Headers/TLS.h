#pragma once
#include "Engine_Defines.h"
#include <Windows.h>
#include <atomic>
#include <iostream>


static DWORD dwTlsIndex; // address of shared memory
static std::atomic<unsigned __int32> SThreadId = 1;

BEGIN(Engine)

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

    __declspec(dllexport)
        BOOL InitTLS()
    {
        LPVOID lpvData;
        TLSDESC** pData;  // The stored memory pointer 

        lpvData = TlsGetValue(dwTlsIndex);
        if (lpvData == NULL)
        {
            lpvData = (LPVOID)LocalAlloc(LPTR, 256);
            if (lpvData == NULL)
                return FALSE;
            if (!TlsSetValue(dwTlsIndex, lpvData))
                return FALSE;
        }

        pData = (TLSDESC**)lpvData; // Cast to my data type.
        // In this example, it is only a pointer to a DWORD
        // but it can be a structure pointer to contain more complicated data.

        TLSDESC* pTLS = new TLSDESC;
        pTLS->LThreadId = SThreadId.fetch_add(1);


        (*pData) = pTLS;
        return TRUE;
    }

    __declspec(dllexport)
        BOOL GetTLS(TLSDESC** pdw)
    {
        LPVOID lpvData;
        TLSDESC** pData;  // The stored memory pointer 

        lpvData = TlsGetValue(dwTlsIndex);
        if (lpvData == NULL)
            return FALSE;

        pData = (TLSDESC**)lpvData;
        (*pdw) = *pData;

        //std::cout << (*pData)->LThreadId << std::endl;
        return TRUE;
    }
#ifdef __cplusplus
}
#endif

END