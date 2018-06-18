#include "os_config.h"
#include "os_task.h"

task_control_block_t tcb[MAX_TASK_COUNT];

void task_InitTCB()
{
    int i;
    for (i = 0; i < MAX_TASK_COUNT; i++)
    {
        tcb[i].state = TASK_FREE;
    }
}

uint32_t* task_createTSK(void (*task)(void *), void* args, uint32_t* ptos)
{
    uint32_t *stk;

    stk       = ptos;                            /* Load stack pointer                                 */

                                                 /* Registers stacked as if auto-saved on exception    */
    *(stk)    = (uint32_t)0x01000000uL;            /* xPSR                                               */
    *(--stk)  = (uint32_t)task & 0xfffffffeUL;     /* Entry Point                                        */
    *(--stk)  = (uint32_t)0xFFFFFFFDuL;           /* R14 (LR)                                           */
    *(--stk)  = (uint32_t)0x12121212uL;            /* R12                                                */
    *(--stk)  = (uint32_t)0x03030303uL;            /* R3                                                 */
    *(--stk)  = (uint32_t)0x02020202uL;            /* R2                                                 */
    *(--stk)  = (uint32_t)0x01010101uL;            /* R1                                                 */
    *(--stk)  = (uint32_t)args;                   /* R0 : argument                                      */

                                                 /* Remaining registers saved on process stack         */
    *(--stk)  = (uint32_t)0x11111111uL;            /* R11                                                */
    *(--stk)  = (uint32_t)0x10101010uL;            /* R10                                                */
    *(--stk)  = (uint32_t)0x09090909uL;            /* R9                                                 */
    *(--stk)  = (uint32_t)0x08080808uL;            /* R8                                                 */
    *(--stk)  = (uint32_t)0x07070707uL;            /* R7                                                 */
    *(--stk)  = (uint32_t)0x06060606uL;            /* R6                                                 */
    *(--stk)  = (uint32_t)0x05050505uL;            /* R5                                                 */
    *(--stk)  = (uint32_t)0x04040404uL;            /* R4                                                 */
    *(--stk) = (uint32_t)0x3uL;//CONTROL
    *(--stk) = (uint32_t)0xFFFFFFFDuL; //LR

    return (stk);
}

int task_getNextReady()
{
    int i, max_prio, nTsk;

    max_prio = 0;
    nTsk = -1;
    for (i = 0;i < MAX_TASK_COUNT; i++)
    {
        //printf("Task: %d State: %d\n", i, tcb[i].state);
        if ((tcb[i].state == TASK_READY) || (tcb[i].state == TASK_RUNNING))
        {
            //printf("Prio: %d Max Prio: %d\n", tcb[i].prio, max_prio);
            if (tcb[i].prio > max_prio)
            {
                max_prio = tcb[i].prio;
                nTsk = i;
            }
        }
    }
    return nTsk;
}