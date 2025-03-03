/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usb_device.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for userMain */
osThreadId_t userMainHandle;
uint32_t userMainBuffer[ 256 ];
osStaticThreadDef_t userMainControlBlock;
const osThreadAttr_t userMain_attributes = {
  .name = "userMain",
  .stack_mem = &userMainBuffer[0],
  .stack_size = sizeof(userMainBuffer),
  .cb_mem = &userMainControlBlock,
  .cb_size = sizeof(userMainControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for guiThread */
osThreadId_t guiThreadHandle;
uint32_t guiThreadBuffer[ 1024 ];
osStaticThreadDef_t guiThreadControlBlock;
const osThreadAttr_t guiThread_attributes = {
  .name = "guiThread",
  .stack_mem = &guiThreadBuffer[0],
  .stack_size = sizeof(guiThreadBuffer),
  .cb_mem = &guiThreadControlBlock,
  .cb_size = sizeof(guiThreadControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for usbThread */
osThreadId_t usbThreadHandle;
uint32_t usbThreadBuffer[ 512 ];
osStaticThreadDef_t usbThreadControlBlock;
const osThreadAttr_t usbThread_attributes = {
  .name = "usbThread",
  .stack_mem = &usbThreadBuffer[0],
  .stack_size = sizeof(usbThreadBuffer),
  .cb_mem = &usbThreadControlBlock,
  .cb_size = sizeof(usbThreadControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for consoleBufferQueue */
osMessageQueueId_t consoleBufferQueueHandle;
uint8_t consoleBufferQueueBuffer[ 512 * sizeof( char ) ];
osStaticMessageQDef_t consoleBufferQueueControlBlock;
const osMessageQueueAttr_t consoleBufferQueue_attributes = {
  .name = "consoleBufferQueue",
  .cb_mem = &consoleBufferQueueControlBlock,
  .cb_size = sizeof(consoleBufferQueueControlBlock),
  .mq_mem = &consoleBufferQueueBuffer,
  .mq_size = sizeof(consoleBufferQueueBuffer)
};
/* Definitions for powerControlTimer */
osTimerId_t powerControlTimerHandle;
osStaticTimerDef_t powerControlTimerControlBlock;
const osTimerAttr_t powerControlTimer_attributes = {
  .name = "powerControlTimer",
  .cb_mem = &powerControlTimerControlBlock,
  .cb_size = sizeof(powerControlTimerControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void user_main(void *argument);
extern void gui_thread(void *argument);
extern void usb_thread(void *argument);
extern void power_control_timer_handle(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of powerControlTimer */
  powerControlTimerHandle = osTimerNew(power_control_timer_handle, osTimerPeriodic, NULL, &powerControlTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of consoleBufferQueue */
  consoleBufferQueueHandle = osMessageQueueNew (512, sizeof(char), &consoleBufferQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of userMain */
  userMainHandle = osThreadNew(user_main, NULL, &userMain_attributes);

  /* creation of guiThread */
  guiThreadHandle = osThreadNew(gui_thread, NULL, &guiThread_attributes);

  /* creation of usbThread */
  usbThreadHandle = osThreadNew(usb_thread, NULL, &usbThread_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_user_main */
/**
  * @brief  Function implementing the userMain thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_user_main */
__weak void user_main(void *argument)
{
  /* init code for USB_Device */
  MX_USB_Device_Init();
  /* USER CODE BEGIN user_main */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END user_main */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

