#include <stdlib.h>

#include "task_cpu.h"


#include "debug.h"
#include <stdio.h>


uint32_t getCurrentStackPtr()
{
    uint32_t result = 0;
    asm volatile (
        "MRS    %0, psp             \n\t"
        : "=r" (result)
    );
    return result;
}

void saveContext(char** ptrToCurrentSP)
{
    uint32_t t;
    asm volatile (
        "MRS    %0, psp             \n\t"
        "STMDB  %0!, {r4-r11}       \n\t"
        "MSR    psp, %0             \n\t"
        : "=r" (t)        
    );
    *(ptrToCurrentSP) = (char*)t;
}

__attribute__((naked)) void loadContext()
{
    uint32_t t;
    asm volatile (
        "MRS    %0, psp             \n\t"
        "LDMIA  %0!, {r4-r11}       \n\t"
        "MSR    psp, %0             \n\t"
        "MOV    lr, #0xFFFFFFFD      \n\t"
        "CPSIE I                    \n\t"
        "BX     lr                  \n\t"
        : "=r" (t)
    );
}

void contextSwitcher(char* nextSP)
{
    asm volatile (
        "MSR    psp, %0             \n\t"
        :
        : "r" (nextSP)
    );
}

void enterCritical()
{
    asm volatile (
        "CPSID I"
    );
}

void exitCritical()
{
    asm volatile (
        "CPSIE I"
    );
}