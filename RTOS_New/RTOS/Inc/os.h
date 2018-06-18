#include <stdint.h>

#ifndef _OS_H_
#define _OS_H_

#include "os_config.h"
#include "os_task.h"

#define OS_Enter_Critical asm volatile ("CPSID I")
#define OS_Exit_Critical asm volatile ("CPSIE I")
#define osTaskDelay(ticks) lastTaskDelay = ticks; \
                            asm volatile ("SVC #0x5");

void osInit();

void osStart();

void osTaskCreate(void (*task)(void *), void* args, uint32_t prio, uint32_t* stk);

#endif

//__attribute__( ( always_inline ) ) inline void osTaskDelay()
//{
//  asm volatile ("SVC #0x5");
//}
