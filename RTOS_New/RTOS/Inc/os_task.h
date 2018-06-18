#include <stdint.h>

#ifndef _OS_TASK_H_
#define _OS_TASK_H_

typedef enum
{
    TASK_FREE, TASK_READY, TASK_RUNNING, TASK_SLEEPING
} task_state_t;

typedef struct 
{
    uint32_t* sp;
    uint32_t prio;
    task_state_t state;
    uint32_t delay;
} task_control_block_t;

void task_InitTCB();

uint32_t* task_createTSK(void (*task)(void *), void* args, uint32_t* stk);

int task_getNextReady();

#endif

