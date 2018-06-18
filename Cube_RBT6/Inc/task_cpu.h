#include <stdint.h>

typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
} hw_stack_frame_t;

typedef struct {
    uint32_t lr;
    uint32_t control;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
} sw_stack_frame_t;

uint32_t getCurrentStackPtr();

void saveContext(char** ptrToCurrentSP);

__attribute__((naked)) void loadContext();

void contextSwitcher(char* nextSP);

void enterCritical();

void exitCritical();
