#include "os.h"
#include "task.h"

#include "debug.h"

int os_started = 0;

int scheduler()
{
    enterCritical();
    int nextTask = getNextReady();
    if (nextTask == NO_SUCH_TASK)
    {
        //Enter Sleep Mode
        logger(&huart1, "Scheduler_Sleep\n");
        exitCritical();
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
    os_started = 1;
    initTask();
    //scheduler();
    halfScheduler();
    return 0;
}

int os_sysTickHandler()
{
    enterCritical();
    if (!os_started)
    {
        exitCritical();
        return 0;
    }
    //TODO: Problem with the Interrupt
    
    
    logger(&huart1, "OS_Systick\n");

    if (task_sysTickHandler())
    {
        scheduler();
    }
    
    //TODO: Problem with the Interrupt
    exitCritical();
    return 0;
}

int os_setTaskDelay(uint32_t ms)
{
    enterCritical();

    logger(&huart1, "OS_SetDelay\n");

    taskDelay(ms * 72);
    scheduler();
    exitCritical();
    return 0;
}

inline int os_createTask( void* (*foo)(void*))
{
    return createTask(foo);
}

inline int os_enterCritical()
{
    enterCritical();
}

inline int os_exitCritical()
{
    exitCritical();
}
