#include <stdlib.h>

#include "task_cpu.h"


#include "debug.h"


inline void* getCurrentStackPtr()
{
    void* result = 0;
    asm volatile (
        "MRS    %0, psp             \n\t"
        : "=r" (result)
    );
    return result;
}

inline int saveContext(char** ptrToCurrentSP)
{
    uint32_t t;
    asm volatile (
        "MRS    %0, psp             \n\t"
        "STMDB  %0!, {r4-r11}       \n\t"
        "MSR    psp, %0             \n\t"
        : "=r" (t)        
    );
    *(ptrToCurrentSP) = (char*)t;
    return 0;
}

inline int loadContext(char** ptrToNextSP)
{
    uint32_t t;
    logger(&huart1, "LoadContext\n");
    asm volatile (
        "MRS    %0, psp             \n\t"
        "LDMFD  %0!, {r4-r11}       \n\t"
        "MSR    psp, %0             \n\t"
        "MOV    lr, 0xFFFFFFFD      \n\t"
        "CPSIE I                    \n\t"
        "BX     lr                  \n\t"
        : "=r" (t)
    );
    return 0;
}

inline int contextSwitcher(char* nextSP)
{
    logger(&huart1, "ContextSwitcher\n");
    asm volatile (
        "MSR    psp, %0             \n\t"
        :
        : "r" (nextSP)
    );
    return 0;
}

inline int enterCritical()
{
    asm volatile (
        "CPSID I"
    );
    return 0;
}

inline int exitCritical()
{
    asm volatile (
        "CPSIE I"
    );
    return 0;
}