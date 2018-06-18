#include <stdint.h>

#define OS_SVC_SET_TASK_DELAY 1

int scheduler();

int halfScheduler();

int osStart();

int os_sysTickHandler();

__attribute__((naked)) int os_setTaskDelay(uint32_t ms);

int os_createTask( void* (*foo)());

int os_enterCritical();

int os_exitCritical();

int os_pre();
