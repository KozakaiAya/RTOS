#include "task.h"

#include "debug.h"
#include <stdio.h>


task_control_block_t tcb[MAX_TASK_COUNT];

char stackFrame[TASK_STACK_SIZE];

uint32_t currentTask = -1;

int initTask()
{
    int i;
    for (i = 0;i < MAX_TASK_COUNT; i++)
    {
        tcb[i].state == TASK_FREE;
    }
    return 0;
}

int createTask( void* (*foo)(void*))
{
    hw_stack_frame_t* hw_process_frame;
    sw_stack_frame_t* sw_process_frame;
    int i, j;

    enterCritical();
    for (i = 0; i < MAX_TASK_COUNT; i++)
    {
        if (tcb[i].state == TASK_FREE)
        {
            tcb[i].sp = (char*)((stackFrame + (TASK_STACK_SIZE) - 1 - i * STACK_FRAME_SIZE) - sizeof(hw_stack_frame_t));
            printf("Task %d init, SP = %lx\n", i, tcb[i].sp);
            hw_process_frame = (hw_stack_frame_t*)tcb[i].sp;
            hw_process_frame->r0 = 0;
            hw_process_frame->r1 = 0;
            hw_process_frame->r2 = 0;
            hw_process_frame->r3 = 0;
            hw_process_frame->r12 = 0;
            hw_process_frame->lr = RETURN_THREAD_MODE_EXEC_PSP;
            hw_process_frame->pc = (uint32_t)foo & TASK_PC_MASK;
            hw_process_frame->psr = PSR_INIT;

            tcb[i].sp = (char*)((uint32_t)tcb[i].sp - sizeof(sw_stack_frame_t));
            printf("Task %d init, SP = %lx\n", i, tcb[i].sp);
            sw_process_frame = (sw_stack_frame_t*)tcb[i].sp;
            sw_process_frame->r4 = 0;
            sw_process_frame->r5 = 0;
            sw_process_frame->r6 = 0;
            sw_process_frame->r7 = 0;
            sw_process_frame->r8 = 0;
            sw_process_frame->r9 = 0;
            sw_process_frame->r10 = 0;
            sw_process_frame->r11 = 0;

            tcb[i].state = TASK_READY;
            tcb[i].delay = 0;
            tcb[i].priority = 0;
            break;
        }
    }
    exitCritical();

    printf("Created Task %d\tSP: %lx\n", i, (uint32_t)tcb[i].sp);
    if (i == MAX_TASK_COUNT)
        return -1;
    else
        return i;
}

int delTask()
{
    return 0;
}

inline int taskDelay(uint32_t value)
{
    tcb[currentTask].delay = value;
    tcb[currentTask].state = TASK_SLEEPING;
    return 0;
}

inline int getNextReady()
{
    int i, j;
    for (i = 1; i <= MAX_TASK_COUNT; i++)
    {
        j = currentTask + i;
        if (j >= MAX_TASK_COUNT) j-=MAX_TASK_COUNT;
        if (tcb[j].state == TASK_READY)
        {
            return j;
        }
    }
    return NO_SUCH_TASK;
}

inline int switchTaskTo(int nextTask)
{
    int oldTask = currentTask;
    tcb[currentTask].state = TASK_READY;
    tcb[nextTask].state = TASK_RUNNING;
    currentTask = nextTask;
    saveContext(&tcb[oldTask].sp);
    logger(&huart1, "ContextSwitcher\n");
    contextSwitcher(tcb[nextTask].sp);
    logger(&huart1, "LoadContext\n");
    loadContext();
    return 0;
}

inline int runFirstTask(int nextTask)
{
    //char* buf[30];
    //memset(buf, 0, 30);
    logger(&huart1, "RunFirstTask\n");
    currentTask = nextTask;
    tcb[currentTask].state = TASK_RUNNING;
    //itoa(tcb[nextTask].sp, 16);
    //logger(&huart1, buf);
    logger(&huart1, "ContextSwitcher\n");
//    printf("Run First Task %d\tSP: %lx\n", nextTask, (uint32_t)tcb[nextTask].sp);
//    printf("Current SP: %lx\n", getCurrentStackPtr());
    contextSwitcher(tcb[nextTask].sp);
    printf("Current SP: %lx\n", getCurrentStackPtr());
    logger(&huart1, "LoadContext\n");
    loadContext();
    logger(&huart1, "Load Context Returned\n");
    return 0;
}

int task_sysTickHandler()
{
    logger(&huart1, "Task_Systick\n");

    int i, f;
    f = 0;
    for (i = 0; i < MAX_TASK_COUNT; i++)
    {
        if (tcb[i].state == TASK_SLEEPING)
        {
            logger(&huart1, "Task_Systick_HasSleeping\n");
            tcb[i].delay--;
            if (tcb[i].delay == 0)
            {
                logger(&huart1, "Task_SystickHandler_NeedResched\n");
                f = 1;
                tcb[i].state = TASK_READY;
            }
        }
    }
    return f;
}

