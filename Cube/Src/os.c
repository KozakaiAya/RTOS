#include "os.h"
#include "task.h"


int scheduler()
{
    int nextTask = getNextReady();
    if (nextTask != NO_SUCH_TASK)
    {
        //Enter Sleep Mode
    }
    else
    {
        switchTaskTo(nextTask);
    }
    return 0;
}

int osStart()
{
    scheduler();
    return 0;
}

int os_sysTickHandler()
{
    task_sysTickHandler();
    scheduler();
    return 0;
}

int os_setTaskDelay(uint32_t ms)
{
    taskDelay(ms * 72);
    scheduler();
    return 0;
}

int os_createTask( void* (*foo)(void*))
{
    return createTask(foo);
}
