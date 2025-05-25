/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
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
#include "OLED.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEY_TRIG_STATE 0
#define KEY_RETRIG_STATE 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
typedef struct{
	uint8_t KeyState;
	uint8_t ClickFlag;
	uint8_t DbclickFlag;
	uint8_t HoldFlag;
	uint8_t IgnoreClickFlag;
	uint8_t HoldCount;

//	DisplayInfo DisplayInfo;
}Key;
enum{
	KeyRelease=0,
	KeyClick,
	KeyDbclick,
	KeyHold
}KeyState;

typedef struct{
	uint8_t language;
	uint8_t Page;
	uint8_t X;
	char *String;
	uint8_t fontSize;
}DisplayInfo;
enum{
	zh=0,
	en
}language;

Key k1, k2, k3, k4;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for KeyClickTask */
osThreadId_t KeyClickTaskHandle;
const osThreadAttr_t KeyClickTask_attributes = {
  .name = "KeyClickTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for KeyDbclickTask */
osThreadId_t KeyDbclickTaskHandle;
const osThreadAttr_t KeyDbclickTask_attributes = {
  .name = "KeyDbclickTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for KeyHoldTask */
osThreadId_t KeyHoldTaskHandle;
const osThreadAttr_t KeyHoldTask_attributes = {
  .name = "KeyHoldTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for DisplayTask */
osThreadId_t DisplayTaskHandle;
const osThreadAttr_t DisplayTask_attributes = {
  .name = "DisplayTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for CopeKeyStateTas */
osThreadId_t CopeKeyStateTasHandle;
const osThreadAttr_t CopeKeyStateTas_attributes = {
  .name = "CopeKeyStateTas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for KeyStateQueue */
osMessageQueueId_t KeyStateQueueHandle;
const osMessageQueueAttr_t KeyStateQueue_attributes = {
  .name = "KeyStateQueue"
};
/* Definitions for DisplayInfoQueue */
osMessageQueueId_t DisplayInfoQueueHandle;
const osMessageQueueAttr_t DisplayInfoQueue_attributes = {
  .name = "DisplayInfoQueue"
};
/* Definitions for HoldDetectBinarySem */
osSemaphoreId_t HoldDetectBinarySemHandle;
const osSemaphoreAttr_t HoldDetectBinarySem_attributes = {
  .name = "HoldDetectBinarySem"
};
/* Definitions for DbclickStartBinarySem */
osSemaphoreId_t DbclickStartBinarySemHandle;
const osSemaphoreAttr_t DbclickStartBinarySem_attributes = {
  .name = "DbclickStartBinarySem"
};
/* Definitions for WaitDbclickBinarySem */
osSemaphoreId_t WaitDbclickBinarySemHandle;
const osSemaphoreAttr_t WaitDbclickBinarySem_attributes = {
  .name = "WaitDbclickBinarySem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartKeyClickTask(void *argument);
void StartKeyDbclickTask(void *argument);
void StartKeyHoldTask(void *argument);
void StartDisplayTask(void *argument);
void StartCopeKeyStateTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* Create the semaphores(s) */
  /* creation of HoldDetectBinarySem */
  HoldDetectBinarySemHandle = osSemaphoreNew(1, 0, &HoldDetectBinarySem_attributes);

  /* creation of DbclickStartBinarySem */
  DbclickStartBinarySemHandle = osSemaphoreNew(1, 0, &DbclickStartBinarySem_attributes);

  /* creation of WaitDbclickBinarySem */
  WaitDbclickBinarySemHandle = osSemaphoreNew(1, 0, &WaitDbclickBinarySem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of KeyStateQueue */
  KeyStateQueueHandle = osMessageQueueNew (16, sizeof(Key), &KeyStateQueue_attributes);

  /* creation of DisplayInfoQueue */
  DisplayInfoQueueHandle = osMessageQueueNew (16, sizeof(DisplayInfo), &DisplayInfoQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of KeyClickTask */
  KeyClickTaskHandle = osThreadNew(StartKeyClickTask, NULL, &KeyClickTask_attributes);

  /* creation of KeyDbclickTask */
  KeyDbclickTaskHandle = osThreadNew(StartKeyDbclickTask, NULL, &KeyDbclickTask_attributes);

  /* creation of KeyHoldTask */
  KeyHoldTaskHandle = osThreadNew(StartKeyHoldTask, NULL, &KeyHoldTask_attributes);

  /* creation of DisplayTask */
  DisplayTaskHandle = osThreadNew(StartDisplayTask, NULL, &DisplayTask_attributes);

  /* creation of CopeKeyStateTas */
  CopeKeyStateTasHandle = osThreadNew(StartCopeKeyStateTask, NULL, &CopeKeyStateTas_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartKeyClickTask */
/**
* @brief Function implementing the KeyClickTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKeyClickTask */
void StartKeyClickTask(void *argument)
{
  /* USER CODE BEGIN StartKeyClickTask */
	uint8_t KeyStateLastTime=0;
	uint8_t KeyStateRealTime=0;
	uint8_t keyTrigState=0;
  /* Infinite loop */
  for(;;)
  {
	  KeyStateRealTime=0;
	  if(HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin)==KEY_TRIG_STATE){KeyStateRealTime|=0X01;}
	  if(HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin)==KEY_TRIG_STATE){KeyStateRealTime|=0X02;}
	  if(HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3_Pin)==KEY_TRIG_STATE){KeyStateRealTime|=0X04;}
	  if(HAL_GPIO_ReadPin(Key4_GPIO_Port, Key4_Pin)==KEY_TRIG_STATE){KeyStateRealTime|=0X08;}

	  if(KeyStateRealTime!=KeyStateLastTime)
	  {
		  for (int i = 0; i < 4; ++i)
		  {
			  if((KeyStateRealTime&(0x01<<i))==0 && (KeyStateLastTime&(0x01<<i))==(0x01<<i))
			  {
				  keyTrigState|=(0x01<<i);
			  }
		  }
	  }

	if (keyTrigState!=0)
	{

		if((keyTrigState&0x01)==0x01)
		{
			k1.ClickFlag=1;
		}
		if((keyTrigState&0x02)==0x02)
		{
			k2.ClickFlag=1;
		}
		if((keyTrigState&0x04)==0x04)
		{
			k3.ClickFlag=1;
		}
		if((keyTrigState&0x08)==0x08)
		{
			k4.ClickFlag=1;
		}
		keyTrigState=0;
		osSemaphoreRelease(DbclickStartBinarySemHandle);
	}
	  osDelay(20);

	  KeyStateLastTime=KeyStateRealTime;
  }
  /* USER CODE END StartKeyClickTask */
}

/* USER CODE BEGIN Header_StartKeyDbclickTask */
/**
* @brief Function implementing the KeyDbclickTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKeyDbclickTask */
void StartKeyDbclickTask(void *argument)
{
  /* USER CODE BEGIN StartKeyDbclickTask */
	uint8_t KeyStateLastTime=0;
	uint8_t KeyStateRealTime=0;
	uint8_t keyTrigState=0;
  /* Infinite loop */
	for(;;)
	{
		osSemaphoreAcquire(DbclickStartBinarySemHandle, osWaitForever);
		for(uint8_t i=0; i<30; ++i)
		{

			KeyStateRealTime=0;
			if(HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin)==KEY_TRIG_STATE && k1.ClickFlag==1){KeyStateRealTime|=0X01;}
			if(HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin)==KEY_TRIG_STATE && k2.ClickFlag==1){KeyStateRealTime|=0X02;}
			if(HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3_Pin)==KEY_TRIG_STATE && k3.ClickFlag==1){KeyStateRealTime|=0X04;}
			if(HAL_GPIO_ReadPin(Key4_GPIO_Port, Key4_Pin)==KEY_TRIG_STATE && k4.ClickFlag==1){KeyStateRealTime|=0X08;}

			if(KeyStateRealTime!=KeyStateLastTime)
			{

				for (int i = 0; i < 4; ++i)
				{
					if((KeyStateRealTime&(0x01<<i))==0 && (KeyStateLastTime&(0x01<<i))==(0x01<<i))
					{
						keyTrigState|=(0x01<<i);
					}
				}
			}

			if (keyTrigState!=0)
			{
				if((keyTrigState&0x01)==0x01)
				{
					k1.DbclickFlag=1;
				}
				if((keyTrigState&0x02)==0x02)
				{
					k2.DbclickFlag=1;
				}
				if((keyTrigState&0x04)==0x04)
				{
					k3.DbclickFlag=1;
				}
				if((keyTrigState&0x08)==0x08)
				{
					k4.DbclickFlag=1;
				}
				keyTrigState=0;
				osSemaphoreRelease(DbclickStartBinarySemHandle);
				osSemaphoreRelease(WaitDbclickBinarySemHandle);
//				break;
			}
			KeyStateLastTime=KeyStateRealTime;
			osDelay(20);
		}

	}

  /* USER CODE END StartKeyDbclickTask */
}

/* USER CODE BEGIN Header_StartKeyHoldTask */
/**
* @brief Function implementing the KeyHoldTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKeyHoldTask */
void StartKeyHoldTask(void *argument)
{
  /* USER CODE BEGIN StartKeyHoldTask */
	uint8_t i;
  /* Infinite loop */
	for(;;)
	{
		for(i=0; i<10; ++i)
		{
			if(HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin)==KEY_TRIG_STATE){k1.HoldCount++;}
			if(HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin)==KEY_TRIG_STATE){k2.HoldCount++;}
			if(HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3_Pin)==KEY_TRIG_STATE){k3.HoldCount++;}
			if(HAL_GPIO_ReadPin(Key4_GPIO_Port, Key4_Pin)==KEY_TRIG_STATE){k4.HoldCount++;}

			osDelay(100);
		}
		if(k1.HoldCount>=8){k1.HoldFlag=1;k1.IgnoreClickFlag=1;osSemaphoreRelease(HoldDetectBinarySemHandle);}
		if(k2.HoldCount>=8){k2.HoldFlag=1;k2.IgnoreClickFlag=1;osSemaphoreRelease(HoldDetectBinarySemHandle);}
		if(k3.HoldCount>=8){k3.HoldFlag=1;k3.IgnoreClickFlag=1;osSemaphoreRelease(HoldDetectBinarySemHandle);}
		if(k4.HoldCount>=8){k4.HoldFlag=1;k4.IgnoreClickFlag=1;osSemaphoreRelease(HoldDetectBinarySemHandle);}
		if(k1.HoldCount<=7){k1.IgnoreClickFlag=0;}
		if(k2.HoldCount<=7){k2.IgnoreClickFlag=0;}
		if(k3.HoldCount<=7){k3.IgnoreClickFlag=0;}
		if(k4.HoldCount<=7){k4.IgnoreClickFlag=0;}
		k1.HoldCount=k2.HoldCount=k3.HoldCount=k4.HoldCount=0;
	}
  /* USER CODE END StartKeyHoldTask */
}

/* USER CODE BEGIN Header_StartDisplayTask */
/**
* @brief Function implementing the DisplayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDisplayTask */
void StartDisplayTask(void *argument)
{
  /* USER CODE BEGIN StartDisplayTask */
	DisplayInfo ReceivedInfo;
  /* Infinite loop */
  for(;;)
  {
	  osMessageQueueGet (DisplayInfoQueueHandle, &ReceivedInfo, NULL, osWaitForever);
	  if(ReceivedInfo.language==zh)
	  {OLED_showChinese(ReceivedInfo.Page, ReceivedInfo.X, ReceivedInfo.String);}
	  if(ReceivedInfo.language==en)
	  {OLED_showString(ReceivedInfo.Page, ReceivedInfo.X, ReceivedInfo.String, ReceivedInfo.fontSize);}
	  OLED_updata();
  }
  /* USER CODE END StartDisplayTask */
}

/* USER CODE BEGIN Header_StartCopeKeyStateTask */
/**
* @brief Function implementing the CopeKeyStateTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCopeKeyStateTask */
void StartCopeKeyStateTask(void *argument)
{
  /* USER CODE BEGIN StartCopeKeyStateTask */
	DisplayInfo InfoOfKey;
	osStatus_t state;

  /* Infinite loop */
  for(;;)
  {
	  state=osSemaphoreAcquire(HoldDetectBinarySemHandle, 0);
	  if(state==osOK)
	  {
		  if(k1.HoldFlag==1)
		  {
			  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键一长按";InfoOfKey.fontSize=8;
			  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
			  k1.HoldFlag=0;
		  }
		  if(k2.HoldFlag==1)
		  {
			  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键二长按";InfoOfKey.fontSize=8;
			  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
			  k2.HoldFlag=0;
		  }
		  if(k3.HoldFlag==1)
		  {
			  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键三长按";InfoOfKey.fontSize=8;
			  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
			  k3.HoldFlag=0;
		  }
		  if(k4.HoldFlag==1)
		  {
			  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键四长按";InfoOfKey.fontSize=8;
			  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
			  k4.HoldFlag=0;
		  }
	  }
	  state=osSemaphoreAcquire(DbclickStartBinarySemHandle, 0);
//	  if(state==osOK)
	  {
		  state=osSemaphoreAcquire(WaitDbclickBinarySemHandle, 600);//双击超时时长
		  if(state==osErrorTimeout)
		  {
			  k1.DbclickFlag=k2.DbclickFlag=k3.DbclickFlag=k4.DbclickFlag=0;
			  if(k1.ClickFlag==1 && k1.IgnoreClickFlag==0)
			  {
				  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键一单击";InfoOfKey.fontSize=8;
				  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
				  k1.ClickFlag=0;
			  }
			  if(k1.IgnoreClickFlag==1){k1.ClickFlag=0;}

			  if(k2.ClickFlag==1 && k2.IgnoreClickFlag==0)
			  {
				  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键二单击";InfoOfKey.fontSize=8;
				  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
				  k2.ClickFlag=0;
			  }
			  if(k2.IgnoreClickFlag==1){k2.ClickFlag=0;}

			  if(k3.ClickFlag==1 && k3.IgnoreClickFlag==0)
			  {
				  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键三单击";InfoOfKey.fontSize=8;
				  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
				  k3.ClickFlag=0;
			  }
			  if(k3.IgnoreClickFlag==1){k3.ClickFlag=0;}

			  if(k4.ClickFlag==1 && k4.IgnoreClickFlag==0)
			  {
				  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键四单击";InfoOfKey.fontSize=8;
				  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
				  k4.ClickFlag=0;
			  }
			  if(k4.IgnoreClickFlag==1){k4.ClickFlag=0;}
		  }
//		  if(state==osOK)
		  {
			  if(k1.DbclickFlag==1)
			  {
				  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键一双击";InfoOfKey.fontSize=8;
				  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
				  k1.ClickFlag=0;
				  k1.DbclickFlag=0;
			  }
			  if(k2.DbclickFlag==1)
			  {
				  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键二双击";InfoOfKey.fontSize=8;
				  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
				  k2.ClickFlag=0;
				  k2.DbclickFlag=0;
			  }
			  if(k3.DbclickFlag==1)
			  {
				  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键三双击";InfoOfKey.fontSize=8;
				  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
				  k3.ClickFlag=0;
				  k3.DbclickFlag=0;
			  }
			  if(k4.DbclickFlag==1)
			  {
				  InfoOfKey.language=zh;InfoOfKey.Page=0;InfoOfKey.X=0;InfoOfKey.String="键四双击";InfoOfKey.fontSize=8;
				  osMessageQueuePut (DisplayInfoQueueHandle, &InfoOfKey, 0, osWaitForever);
				  k4.ClickFlag=0;
				  k4.DbclickFlag=0;
			  }
		  }
	  }
  }
  /* USER CODE END StartCopeKeyStateTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

