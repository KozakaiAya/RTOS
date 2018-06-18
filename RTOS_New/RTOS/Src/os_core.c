#include <stdint.h>
#include "os.h"
#include "os_task.h"

#include "stm32f1xx_hal.h"

int os_started;
int os_IntNestCount;
int currentTask;
int nextTask;
int needTaskSwitch;
int firstStarted;
int needResched;
int lastTaskDelay;

extern task_control_block_t tcb[MAX_TASK_COUNT];

extern UART_HandleTypeDef huart1;

uint32_t idleTaskStack[256];

void osIdleTask(void *args)
{
    //HAL_UART_Transmit(&huart1, "TSKIDLR\n", 8, 0xFF);
    while (1)
    {
        //HAL_UART_Transmit(&huart1, "TSKIDLR\n", 8, 0xFF);
    }
    return;
}

void osInit()
{
    OS_Enter_Critical;
    task_InitTCB();
    osTaskCreate(osIdleTask, (void *)0, 1, (uint32_t *)&idleTaskStack[255]);
    currentTask = -1;
    nextTask = -1;
    needTaskSwitch = 0;
    os_started = 0;
    os_IntNestCount = 0;
    firstStarted = 0;
    needResched = 0;
    lastTaskDelay = 0;
    asm volatile(
        "MOV R0, #0 \n\t"
        "MSR PSP, R0");
    OS_Exit_Critical;
    return;
}

void osStart()
{
    os_started = 1;
    //HAL_UART_Transmit(&huart1, "OSS\n", 4, 0xFF);
    OS_Exit_Critical;
}

void osTaskCreate(void (*task)(void *), void *args, uint32_t prio, uint32_t *stk)
{
    int i;
    OS_Enter_Critical;
    for (i = 0; i < MAX_TASK_COUNT; i++)
    {
        if (tcb[i].state == TASK_FREE)
        {
            tcb[i].sp = task_createTSK(task, args, stk);
            tcb[i].prio = prio;
            tcb[i].state = TASK_READY;
            tcb[i].delay = 0;
            //printf("Task %d; SP: %lx\n", i, tcb[i].sp);
            break;
        }
    }
    OS_Exit_Critical;
}

