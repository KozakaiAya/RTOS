#include <stdint.h>

int scheduler();

int halfScheduler();

int osStart();

int os_sysTickHandler();

int os_setTaskDelay(uint32_t ms);

int os_createTask( void* (*foo)());

int os_enterCritical();

int os_exitCritical();

int os_pre();
