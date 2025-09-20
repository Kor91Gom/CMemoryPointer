#ifndef CMEMPLATFORM_H
#define CMEMPLATFORM_H

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void CMemInitPlatform(void);
EXPORT const char* CMemGetPlatformName(void);
EXPORT int CMemIsLittleEndian(void);

extern char g_platformName[];

#ifdef __cplusplus
}
#endif

#endif
