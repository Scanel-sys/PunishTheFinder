// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Vaccine.h"


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "[*] Process attached the vaccine\n";
        break;

    case DLL_THREAD_ATTACH:
        std::cout << "[*] Thread attached the vaccine\n";
        break;

    case DLL_THREAD_DETACH:
        std::cout << "[*] Thread detached the vaccine\n";
        break;

    case DLL_PROCESS_DETACH:
        std::cout << "[*] Process detached the vaccine\n";
        break;
    }
    return TRUE;
}

