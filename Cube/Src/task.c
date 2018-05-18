#include "task.h"

task_control_block_t tcb[MAX_TASK_COUNT];

uint32_t currentTask = 0;

int initTask()
{
    int i;
    for (i = 0;i < MAX_TASK_COUNT; i++)
    {
        tcb[i].state == TASK_FREE;
    }
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
            tcb[i].sp = (char*)((SRAM_TOP_ADDRESS - (i * STACK_FRAME_SIZE)) - sizeof(hw_stack_frame_t));
            hw_process_frame = (hw_stack_frame_t*)tcb[i].sp;
            hw_process_frame->r0 = 0;
            hw_process_frame->r1 = 0;
            hw_process_frame->r2 = 0;
            hw_process_frame->r3 = 0;
            hw_process_frame->r12 = 0;
            hw_process_frame->pc = (uint32_t)foo;
            hw_process_frame->lr = RETURN_THREAD_MODE_EXEC_PSP;
            hw_process_frame->psr = PSR_INIT;

            tcb[i].sp = tcb[i].sp - sizeof(sw_stack_frame_t);
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
        }
    }
    exitCritical();

    if (i == MAX_TASK_COUNT)
        return -1;
    else
        return i;
}

int delTask()
{
    return 0;
}

int taskDelay(uint32_t value)
{
    tcb[currentTask].delay = value;
    tcb[currentTask].state = TASK_SLEEPING;
}

int getNextReady()
{
    int i;
    for (i = 0; i < MAX_TASK_COUNT; i++)
    {
        if (tcb[i].state == TASK_READY)
        {
            return i;
        }
    }
    return NO_SUCH_TASK;
}

int switchTaskTo(int nextTask)
{
    int oldTask = currentTask;
    tcb[currentTask].state = TASK_READY;
    tcb[nextTask].state = TASK_RUNNING;
    currentTask = nextTask;
    saveContext(&tcb[oldTask].sp);
    loadContext(&tcb[currentTask].sp);

}

int task_sysTickHandler()
{
    int i;
    for (i = 0; i < MAX_TASK_COUNT; i++)
    {
        if (tcb[i].state == TASK_SLEEPING)
        {
            tcb[i].delay--;
            if (tcb[i].delay == 0)
            {
                tcb[i].state = TASK_READY;
            }
        }
    }
    return 0;
}

