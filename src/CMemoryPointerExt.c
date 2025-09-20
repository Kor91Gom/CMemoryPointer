#include "CMemoryPointerExt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCOPE_STACK 128
CMem *g_scopeStack[MAX_SCOPE_STACK];
int g_scopeTop = 0;
CMemObserver g_cmemObserver = NULL;
CMemLogLevel g_logLevel = LOG_INFO;

static CMem *g_allocatedList[1024];
static int g_allocatedCount = 0;

void CMemAutoFree(CMem **ptr)
{
    CMemUnref(*ptr);
}

void CMemScopePush(CMem *ptr)
{
    if (g_scopeTop < MAX_SCOPE_STACK)
        g_scopeStack[g_scopeTop++] = ptr;
}

void CMemScopePop(void)
{
    if (g_scopeTop > 0)
        CMemUnref(g_scopeStack[--g_scopeTop]);
}

void CMemScopeCleanup(void)
{
    while (g_scopeTop > 0)
        CMemUnref(g_scopeStack[--g_scopeTop]);
}

void CMemRegisterObserver(CMemObserver observer)
{
    g_cmemObserver = observer;
}

void CMemGlobalInit(void)
{
    g_scopeTop = 0;
    g_cmemObserver = NULL;
    g_allocatedCount = 0;
    atexit(CMemDumpLeaks);
}

void CMemGlobalShutdown(void)
{
    CMemScopeCleanup();
}

void CMemDebugLog(const char *msg, CMem *ptr)
{
    if (g_logLevel >= LOG_DEBUG)
    {
        printf("[Debug] %s: %p (size: %zu, tag: %s, type: %s)\n",
            msg, ptr->data, ptr->size, ptr->tag, ptr->type);
    }
}

void CMemTrack(CMem *ptr)
{
    if (g_allocatedCount < 1024)
        g_allocatedList[g_allocatedCount++] = ptr;
}

void CMemDumpLeaks(void)
{
    if (g_logLevel >= LOG_INFO && g_allocatedCount > 0)
    {
        printf("🔍 Memory Leak Report:\n");
        for (int i = 0; i < g_allocatedCount; ++i)
            CMemDebugLog("Leaked", g_allocatedList[i]);
    }
}

void CMemSetLogLevel(CMemLogLevel level)
{
    g_logLevel = level;
}

void CMemSetTag(CMem *ptr, const char *tag)
{
    ptr->tag = strdup(tag);
}

void CMemSetType(CMem *ptr, const char *type)
{
    ptr->type = strdup(type);
}
