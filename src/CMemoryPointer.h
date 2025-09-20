#ifndef CMEMORYPOINTER_H
#define CMEMORYPOINTER_H

#include <stddef.h>
#include <time.h>
#include <stdatomic.h>

typedef struct {
    void *data;
    void (*destructor)(void *);
    size_t size;
    atomic_int refCount;
    const char *tag;
    const char *type;
    time_t timestamp;
} CMem;

CMem* CMemCreate(size_t size, void (*destructor)(void *));
CMem* CMemCreateArray(size_t count, size_t elementSize, void (*elementDestructor)(void *));
CMem** CMemCreateDoublePtr(size_t outerCount, size_t innerCount, size_t elementSize, void (*elementDestructor)(void *));

void CMemRef(CMem *ptr);
void CMemUnref(CMem *ptr);
void CMemFree(CMem *ptr);

// FFI-safe API
void* CMemGetData(CMem *ptr);
size_t CMemGetSize(CMem *ptr);

#endif
