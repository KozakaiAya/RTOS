#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "os_config.h"
#include "os_task.h"

extern int os_started;

extern int os_IntNestCount;

extern task_control_block_t tcb[MAX_TASK_COUNT];

extern int currentTask;

extern int nextTask;

extern int needTaskSwitch;

extern int firstStarted;

extern int needResched;

extern int lastTaskDelay;

#endif