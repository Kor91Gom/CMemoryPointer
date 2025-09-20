#include <stdio.h>
#include <stdlib.h>
#include "CMemoryPointer.h"
#include "CMemoryPointerExt.h"
#include "CMemPlatform.h"

void IntDestructor(void *data)
{
    printf("Int memory freed: %p\n", data);
}

void ArrayDestructor(void *data)
{
    printf("Array memory freed: %p\n", data);
}

void MemoryLogger(CMem *ptr, const char *event)
{
    printf("[Observer] %s: %p (size: %zu, tag: %s)\n", event, ptr->data, ptr->size, ptr->tag);
}

int main()
{
    // 🌐 플랫폼 감지 및 출력
    CMemInitPlatform();
    printf("Platform: %s\n", CMemGetPlatformName());
    printf("Endian: %s\n\n", CMemIsLittleEndian() ? "Little" : "Big");

    // 🧠 스마트 포인터 시스템 초기화
    CMemGlobalInit();
    CMemRegisterObserver(MemoryLogger);

    // ✅ 일반 포인터 입력
    int intValue;
    printf("Enter an integer value: ");
    scanf("%d", &intValue);

    AUTO_FREE CMem *intPtr = CMemCreate(sizeof(int), IntDestructor);
    *(int *)intPtr->data = intValue;
    CMemTrack(intPtr);
    CMemDebugLog("Created intPtr", intPtr);
    printf("intPtr value: %d\n\n", *(int *)intPtr->data);

    // ✅ 배열 입력
    int arraySize;
    printf("Enter array size: ");
    scanf("%d", &arraySize);

    CMem *arrayPtr = CMemCreateArray(arraySize, sizeof(int), ArrayDestructor);
    CMemScopePush(arrayPtr);
    CMemTrack(arrayPtr);
    CMemDebugLog("Created arrayPtr", arrayPtr);

    int *arr = (int *)arrayPtr->data;
    for (int i = 0; i < arraySize; ++i)
    {
        printf("Enter arr[%d]: ", i);
        scanf("%d", &arr[i]);
    }
    printf("\n");
    for (int i = 0; i < arraySize; ++i)
        printf("arr[%d] = %d\n", i, arr[i]);
    printf("\n");

    // ✅ 이중 포인터 입력
    int rows, cols;
    printf("Enter matrix rows and columns: ");
    scanf("%d %d", &rows, &cols);

    CMem **matrix = CMemCreateDoublePtr(rows, cols, sizeof(int), ArrayDestructor);
    for (int i = 0; i < rows; ++i)
    {
        int *row = (int *)matrix[i]->data;
        for (int j = 0; j < cols; ++j)
        {
            printf("Enter matrix[%d][%d]: ", i, j);
            scanf("%d", &row[j]);
        }
        CMemScopePush(matrix[i]);
        CMemTrack(matrix[i]);
        CMemDebugLog("Created matrix row", matrix[i]);
    }
    printf("\n");

    for (int i = 0; i < rows; ++i)
    {
        int *row = (int *)matrix[i]->data;
        for (int j = 0; j < cols; ++j)
            printf("matrix[%d][%d] = %d\n", i, j, row[j]);
    }

    // ✅ 외부 포인터 배열 해제
    free(matrix);

    // 🧹 자동 해제 및 누수 검사
    CMemScopeCleanup();
    CMemGlobalShutdown();

    return 0;
}
