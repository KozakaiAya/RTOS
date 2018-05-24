#include "os.h"
#include "task.h"

#include "debug.h"

extern huart1;

int scheduler()
{
    enterCritical();
    int nextTask = getNextReady();
    if (nextTask == NO_SUCH_TASK)
    {
        //Enter Sleep Mode
        logger(&huart1, "Scheduler_Sleep\n");

        while(1){};
    }
    else
    {
        logger(&huart1, "Scheduler_Switch\n");
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
    logger(&huart1, "OS_Started\n");

    initTask();
    //scheduler();
    halfScheduler();
    return 0;
}

int os_sysTickHandler()
{
    //TODO: Problem with the Interrupt
    enterCritical();
    
    logger(&huart1, "OS_Systick\n");

    task_sysTickHandler();
    scheduler();
    
    //TODO: Problem with the Interrupt
    exitCritical();
    return 0;
}

int os_setTaskDelay(uint32_t ms)
{
    logger(&huart1, "OS_SetDelay\n");

    taskDelay(ms * 72);
    scheduler();
    return 0;
}

int os_createTask( void* (*foo)(void*))
{
    return createTask(foo);
}

int os_enterCritical()
{
    enterCritical();
}

int os_exitCritical()
{
    exitCritical();
}
