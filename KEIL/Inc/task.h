#include <stdint.h>

#include "task_cpu.h"

#define MAX_TASK_COUNT 16

#define STACK_FRAME_SIZE 256

#define TASK_STACK_SIZE (MAX_TASK_COUNT * STACK_FRAME_SIZE + 1)

#define PSR_INIT 0x21000000

#define SRAM_TOP_ADDRESS 0x20001000

#define TASK_PC_MASK 0xfffffffe

#define PROC_STACK_TOP (SRAM_TOP_ADDRESS - STACK_FRAME_SIZE)

#define NO_SUCH_TASK -1

#define RETURN_THREAD_MODE_EXEC_PSP 0xFFFFFFFD

#define RETURN_THREAD_MODE_EXEC_MSP 0xFFFFFFF9

#define RETURN_HANDLER_MODE_EXEC_MSP 0xFFFFFFF1

typedef enum {
    TASK_FREE,
    TASK_READY,
    TASK_RUNNING,
    TASK_SLEEPING,
    TASK_TERMINATED
} task_state_t;

typedef struct {
    char* sp;
    char* stack;
    uint32_t priority;
    uint32_t delay;
    task_state_t state;
} task_control_block_t;

int initTask();

int createTask( void* (*foo)(void*));

int delTask();

int taskDelay(uint32_t value);

int getNextReady();

int switchTaskTo(int nextTask);

int runFirstTask(int nextTask);

int task_sysTickHandler();




