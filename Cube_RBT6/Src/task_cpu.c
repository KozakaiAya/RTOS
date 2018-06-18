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
    uint32_t t = 0;
/*    asm volatile (
        "MRS    r0, psp             \n\t"
        "MOV    r2, lr              \n\t"
        "MRS    r3, CONTROL         \n\t"
        "STMDB  r0!, {r2-r11}       \n\t"
        "MOV    %0, r0              \n\t"
        "ISB                        \n\t"
        : "=r" (t)
    );*/
    asm volatile("MRS   r0, psp");
    asm volatile("LDR   r2, [r0, #0x18]");
    asm volatile("MRS   r3, CONTROL");
    asm volatile("STMDB r0!, {r2-r11}");
    asm volatile(
        "MOV %0, r0"
        : "=r" (t));
    asm volatile("ISB");    
    *(ptrToCurrentSP) = (char*)t;
}

__attribute__((naked)) void loadContext()
{
    asm volatile (
        "MRS    r0, psp             \n\t"
        "LDMIA  r0!, {r2-r11}       \n\t"
        "MOV    lr, r2              \n\t"
        "MSR    CONTROL, r3         \n\t"
        "MSR    psp, r0             \n\t"
        "ISB                        \n\t"
        "CPSIE  I                   \n\t"
//        "BX     lr"
        "MOV    pc, lr"
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