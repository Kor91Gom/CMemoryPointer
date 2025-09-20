#ifndef CMEMORYPOINTEREXT_H
#define CMEMORYPOINTEREXT_H

#include "CMemoryPointer.h"

#define AUTO_FREE __attribute__((cleanup(CMemAutoFree)))
void CMemAutoFree(CMem **ptr);

void CMemScopePush(CMem *ptr);
void CMemScopePop(void);
void CMemScopeCleanup(void);

typedef void (*CMemObserver)(CMem *ptr, const char *event);
void CMemRegisterObserver(CMemObserver observer);

void CMemGlobalInit(void);
void CMemGlobalShutdown(void);

void CMemDebugLog(const char *msg, CMem *ptr);
void CMemTrack(CMem *ptr);
void CMemDumpLeaks(void);

extern CMemObserver g_cmemObserver;
extern CMem *g_scopeStack[];
extern int g_scopeTop;

#endif
