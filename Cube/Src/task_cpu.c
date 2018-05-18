#include "task_cpu.h"

void* getCurrentStackPtr()
{
    void* result = NULL;
    asm volatile (
        "MRS    %0, psp             \n\t"
        : "=r" (result)
    );
    return result;
}

int saveContext(char** ptrToCurrentSP)
{
    uint32_t t;
    *(ptrToCurrentSP) = (char*)getCurrentStackPtr();
    asm volatile (
        "MRS    %0, psp             \n\t"
        "STMDB  %0!, {r4-r11}       \n\t"
        "MSR    psp, %0             \n\t"
        : "=r" (t)        
    );
    return 0;
}

int loadContext(char** ptrToNextSP)
{
    uint32_t t;
    contextSwitcher(*ptrToNextSP);
    asm volatile (
        "MRS    %0, psp             \n\t"
        "LDMFD  %0!, {r4-r11}       \n\t"
        "MSR    psp, %0             \n\t"
        "bx     lr                  \n\t"
        : "=r" (t)
    );
    return 0;
}

int contextSwitcher(char* nextSP)
{
    asm volatile (
        "MSR    psp, %0             \n\t"
        :
        : "r" (nextSP)
    );
    return 0;
}

int enterCritical()
{
    __disable_irq();
}

int exitCritical()
{
    __enable_irq();
}