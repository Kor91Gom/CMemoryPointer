#include "CMemoryPointer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

CMem* CMemCreate(size_t size, void (*destructor)(void *))
{
    CMem *cmem = (CMem *)malloc(sizeof(CMem));
    if (!cmem) {
        fprintf(stderr, "CMemCreate: allocation failed\n");
        return NULL;
    }

    cmem->data = malloc(size);
    if (!cmem->data) {
        free(cmem);
        fprintf(stderr, "CMemCreate: data allocation failed\n");
        return NULL;
    }

    cmem->size = size;
    cmem->destructor = destructor;
    atomic_init(&cmem->refCount, 1);
    cmem->tag = "default";
    cmem->type = "raw";
    cmem->timestamp = time(NULL);

    return cmem;
}

CMem* CMemCreateArray(size_t count, size_t elementSize, void (*elementDestructor)(void *))
{
    CMem *ptr = CMemCreate(count * elementSize, elementDestructor);
    if (ptr) {
        ptr->type = "array";
        ptr->tag = "array";
    }
    return ptr;
}

CMem** CMemCreateDoublePtr(size_t outerCount, size_t innerCount, size_t elementSize, void (*elementDestructor)(void *))
{
    CMem **outer = (CMem **)malloc(sizeof(CMem *) * outerCount);
    if (!outer) return NULL;

    for (size_t i = 0; i < outerCount; ++i)
    {
        outer[i] = CMemCreate(innerCount * elementSize, elementDestructor);
        if (!outer[i])
        {
            for (size_t j = 0; j < i; ++j)
                CMemUnref(outer[j]);
            free(outer);
            return NULL;
        }
        outer[i]->type = "matrix_row";
        outer[i]->tag = "row";
    }

    return outer;
}

void CMemRef(CMem *ptr)
{
    if (ptr) atomic_fetch_add(&ptr->refCount, 1);
}

void CMemUnref(CMem *ptr)
{
    if (!ptr) return;
    if (atomic_fetch_sub(&ptr->refCount, 1) == 1)
    {
        if (ptr->destructor) ptr->destructor(ptr->data);
        free(ptr->data);
        free(ptr);
    }
}

void CMemFree(CMem *ptr)
{
    if (!ptr) return;
    if (ptr->destructor) ptr->destructor(ptr->data);
    free(ptr->data);
    free(ptr);
}

void* CMemGetData(CMem *ptr)
{
    return ptr ? ptr->data : NULL;
}

size_t CMemGetSize(CMem *ptr)
{
    return ptr ? ptr->size : 0;
}
