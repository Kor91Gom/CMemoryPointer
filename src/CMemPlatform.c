#include "CMemPlatform.h"
#include <string.h>

char g_platformName[64] = "Unknown";

EXPORT void CMemInitPlatform(void)
{
#if defined(_WIN32)
    strcpy(g_platformName, "Windows");
#elif defined(__APPLE__)
    strcpy(g_platformName, "macOS/iOS");
#elif defined(__ANDROID__)
    strcpy(g_platformName, "Android");
#elif defined(__linux__)
    #if defined(__aarch64__)
        strcpy(g_platformName, "Linux ARM64");
    #elif defined(__arm__)
        strcpy(g_platformName, "Linux ARM");
    #elif defined(__riscv) // cSpell:ignore riscv
        strcpy(g_platformName, "Linux RISC-V");
    #else
        strcpy(g_platformName, "Linux x86/x64");
    #endif
#else
    strcpy(g_platformName, "Unknown");
#endif
}

EXPORT const char* CMemGetPlatformName(void)
{
    return g_platformName;
}

EXPORT int CMemIsLittleEndian(void)
{
    unsigned int x = 1;
    return *((char*)&x);
}
