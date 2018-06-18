/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "global.h"
#include "os.h"
#include "os_task.h"

#include <stdio.h>
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;

void SVC_Handler_C(unsigned int *svc_args)
{
  unsigned int svc_number;
  uint32_t stacked_r1, curSP;

  os_IntNestCount++;
  OS_Enter_Critical;

  svc_number = ((char *)svc_args[6])[-2];
  stacked_r1 = svc_args[1];
  //HAL_UART_Transmit(&huart1, "SVCHC\n", 6, 0xFF);
  if (os_IntNestCount <= 1)
  {
    switch (svc_number)
    {
    case 5:
      //osTaskDelay
      asm volatile(
          "MRS    R0, PSP \n\t"
          "MOV    R2, LR \n\t"
          "MRS    R3, CONTROL \n\t"
          "STMDB  R0!, {R2-R11} \n\t"
          "MOV    %0, R0"
          : "=r"(curSP));
      tcb[currentTask].sp = curSP;
      tcb[currentTask].state = TASK_SLEEPING;
      tcb[currentTask].delay = lastTaskDelay;
      printf("SVCC Task %d Delay %d State %d\n", currentTask, tcb[currentTask].delay, tcb[currentTask].state);
      nextTask = task_getNextReady();
      //printf("SVCC nextTask: %d\n", nextTask);
      SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
      needResched = 1;
      break;
    }
  }
  OS_Exit_Critical;
  os_IntNestCount--;
  return;
}
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
  HAL_UART_Transmit(&huart1, "NMI\n", 4, 0xFF);
  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
  uint32_t psp;
  HAL_UART_Transmit(&huart1, "HardFT\n", 7, 0xFF);
  asm volatile("MRS %0, PSP"
               : "=r"(psp));
  printf("PSP: %lx\n", psp);
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */
  HAL_UART_Transmit(&huart1, "MemM\n", 5, 0xFF);
  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */
  HAL_UART_Transmit(&huart1, "BusF\n", 5, 0xFF);
  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */
  HAL_UART_Transmit(&huart1, "UsgF\n", 5, 0xFF);
  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */
  asm volatile(
      "TST     lr, #0x4            \n\t"
      "ITE     eq                  \n\t"
      "MRSEQ   r0, msp             \n\t"
      "MRSNE   r0, psp             \n\t"
      "B   SVC_Handler_C           \n\t");
  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */
  HAL_UART_Transmit(&huart1, "DbgMon\n", 7, 0xFF);
  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
// Load Context and BX
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */
  uint32_t *nextSP;
  uint32_t *curSP;
  os_IntNestCount++;
  OS_Enter_Critical;
  //HAL_UART_Transmit(&huart1, "PendSV\n", 7, 0xFF);
  if (needResched && (os_IntNestCount <= 1))
  {
    //HAL_UART_Transmit(&huart1, "PendSVSch\n", 10, 0xFF);
    printf("PendSV Resched %d to %d\n", currentTask, nextTask);
    if (os_started && (os_IntNestCount <= 1))
    {
      if (firstStarted)
      {
        asm volatile(
            "MRS    R0, PSP \n\t"
            "MOV    R2, LR \n\t"
            "MRS    R3, CONTROL \n\t"
            "STMDB  R0!, {R2-R11} \n\t"
            //            "SUBS   R0, R0, #0x28 \n\t"
            //            "STM    R0, {R2-R11} \n\t"
            "MOV    %0, R0"
            : "=r"(curSP));
        tcb[currentTask].sp = curSP;
        if (tcb[currentTask].state == TASK_RUNNING)
          tcb[currentTask].state = TASK_READY;
        //printf("currentTask: %d SP: %lx\n", currentTask, curSP);
      }
      else
      {
        firstStarted = 1;
      }
      nextSP = tcb[nextTask].sp;
      tcb[nextTask].state = TASK_RUNNING;
      //printf("nextTask: %d SP: %lx\n", nextTask, nextSP);
      currentTask = nextTask;
      os_IntNestCount--;
      needResched = 0;
      //HAL_UART_Transmit(&huart1, "PSVTSK\n", 7, 0xFF);
      asm volatile(
          "MOV    R1, %0  \n\t"
          "LDMIA  R1!, {R2-R11} \n\t"
          //          "LDM    R1, {R2-R11} \n\t"
          //          "ADDS   R1, R1, #0x28 \n\t"
          "MSR    PSP, R1 \n\t"
          //          "MOV    LR, R2  \n\t"
          "MOV    LR, #0xFFFFFFFD \n\t"
          "MSR    CONTROL, R3 \n\t"
          "ISB    \n\t"
          "CPSIE  I \n\t"
          "BX     LR"
          :
          : "r"(nextSP));
    }
    OS_Exit_Critical;
  }
  OS_Exit_Critical;
  os_IntNestCount--;
  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */
  //HAL_UART_Transmit(&huart1, "PSVERR\n", 7, 0xFF);
  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  uint32_t *curSP;
  int i;
  os_IntNestCount++;
  OS_Enter_Critical;
  //HAL_UART_Transmit(&huart1, "Tick\n", 5, 0xFF);
  if (os_started && (os_IntNestCount <= 1) && (needResched == 0))
  {
    //HAL_UART_Transmit(&huart1, "TickOS\n", 7, 0xFF);
    for (i = 0; i < MAX_TASK_COUNT; i++)
    {
      if (tcb[i].state == TASK_SLEEPING)
      {
        tcb[i].delay--;
        //printf("Task %d, Delay %d\n", i, tcb[i].delay);
        if (tcb[i].delay == 0)
          tcb[i].state = TASK_READY;
      }
    }
    nextTask = task_getNextReady();
    //printf("SYSTICK nextTask: %d\n", nextTask);
    if (nextTask != currentTask)
    {
      SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
      needResched = 1;
    }
  }
  OS_Exit_Critical;
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  os_IntNestCount--;
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
