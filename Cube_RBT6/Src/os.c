#include "os.h"
#include "task.h"

#include "debug.h"

int os_started = 0;

#define svc(code) asm volatile ("svc %[immediate]"::[immediate] "I" (code))

int scheduler()
{
    enterCritical();
    int nextTask = getNextReady();
    if (nextTask == NO_SUCH_TASK)
    {
        //Enter Sleep Mode
        //logger(&huart1, "Scheduler_Sleep\n");
        exitCritical();
        while(1){};
    }
    else
    {
        //logger(&huart1, "Scheduler_Switch\n");
        switchTaskTo(nextTask);
    }
    return 0;
}

int halfScheduler()
{
    enterCritical();

    int nextTask = getNextReady();
    if (nextTask != NO_SUCH_TASK)
    {
        runFirstTask(nextTask);
    }
    return 0;
}

int osStart()
{
    //logger(&huart1, "OS_Started\n");
    os_started = 1;
    //scheduler();
    halfScheduler();
    return 0;
}

int os_pre()
{
    //logger(&huart1, "OS_Pre\n");
    initTask();
    return 0;
}

int os_sysTickHandler()
{
    //logger(&huart1, "OS_Systick1\n");
    if (!os_started)
    {
        return 0;
    }
    //TODO: Problem with the Interrupt
        
    //logger(&huart1, "OS_Systick\n");
    enterCritical();
    if (task_sysTickHandler())
    {
        scheduler();
    }
    
    //TODO: Problem with the Interrupt
    exitCritical();
    return 0;
}

__attribute__((naked)) int os_setTaskDelay(uint32_t ms)
{
    svc(OS_SVC_SET_TASK_DELAY);
    return 0;
}

int os_createTask( void* (*foo)())
{
    return createTask(foo);
}

int os_enterCritical()
{
    enterCritical();
    return 0;
}

int os_exitCritical()
{
    exitCritical();
    return 0;
}
