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
#include <string.h>
#include <math.h>
#include "adc.h"
#include "tim.h"
#include "usart.h"
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
uint16_t RPValue[4]={0};

int countRealTime =0;
int countLastTime =0;
int SetSpeed = 0;
int RealSpeed = 0;

float UART1SendBuf[50]={0};
const uint8_t tail[4]={0x00, 0x00, 0x80, 0x7f};

float kp=2,ki=2,kd;
int ErrorRealTime,ErrorLast1Time,ErrorInt,PWMOutPut;


uint8_t KeyStateLastTime=0;
uint8_t KeyStateRealTime=0;
uint8_t keyTrigState=0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for OLEDupdateTask */
osThreadId_t OLEDupdateTaskHandle;
const osThreadAttr_t OLEDupdateTask_attributes = {
  .name = "OLEDupdateTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for ReadADCTask */
osThreadId_t ReadADCTaskHandle;
const osThreadAttr_t ReadADCTask_attributes = {
  .name = "ReadADCTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SstMotorTask */
osThreadId_t SstMotorTaskHandle;
const osThreadAttr_t SstMotorTask_attributes = {
  .name = "SstMotorTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for USART1Task */
osThreadId_t USART1TaskHandle;
const osThreadAttr_t USART1Task_attributes = {
  .name = "USART1Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PIDupdateTask */
osThreadId_t PIDupdateTaskHandle;
const osThreadAttr_t PIDupdateTask_attributes = {
  .name = "PIDupdateTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for GetEncoderTask */
osThreadId_t GetEncoderTaskHandle;
const osThreadAttr_t GetEncoderTask_attributes = {
  .name = "GetEncoderTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for CopeKeyTask */
osThreadId_t CopeKeyTaskHandle;
const osThreadAttr_t CopeKeyTask_attributes = {
  .name = "CopeKeyTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for OLEDupdateSem */
osSemaphoreId_t OLEDupdateSemHandle;
const osSemaphoreAttr_t OLEDupdateSem_attributes = {
  .name = "OLEDupdateSem"
};
/* Definitions for PIDupdateSem */
osSemaphoreId_t PIDupdateSemHandle;
const osSemaphoreAttr_t PIDupdateSem_attributes = {
  .name = "PIDupdateSem"
};
/* Definitions for MotorUpdateSem */
osSemaphoreId_t MotorUpdateSemHandle;
const osSemaphoreAttr_t MotorUpdateSem_attributes = {
  .name = "MotorUpdateSem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartOLEDupdateTask(void *argument);
void StartReadADCTask(void *argument);
void StartSstMotorTask(void *argument);
void StartUSART1Task(void *argument);
void StartPIDupdateTask(void *argument);
void StartGetEncoderTask(void *argument);
void StartCopeKeyTask(void *argument);

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
  /* creation of OLEDupdateSem */
  OLEDupdateSemHandle = osSemaphoreNew(1, 1, &OLEDupdateSem_attributes);

  /* creation of PIDupdateSem */
  PIDupdateSemHandle = osSemaphoreNew(1, 1, &PIDupdateSem_attributes);

  /* creation of MotorUpdateSem */
  MotorUpdateSemHandle = osSemaphoreNew(1, 1, &MotorUpdateSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of OLEDupdateTask */
  OLEDupdateTaskHandle = osThreadNew(StartOLEDupdateTask, NULL, &OLEDupdateTask_attributes);

  /* creation of ReadADCTask */
  ReadADCTaskHandle = osThreadNew(StartReadADCTask, NULL, &ReadADCTask_attributes);

  /* creation of SstMotorTask */
  SstMotorTaskHandle = osThreadNew(StartSstMotorTask, NULL, &SstMotorTask_attributes);

  /* creation of USART1Task */
  USART1TaskHandle = osThreadNew(StartUSART1Task, NULL, &USART1Task_attributes);

  /* creation of PIDupdateTask */
  PIDupdateTaskHandle = osThreadNew(StartPIDupdateTask, NULL, &PIDupdateTask_attributes);

  /* creation of GetEncoderTask */
  GetEncoderTaskHandle = osThreadNew(StartGetEncoderTask, NULL, &GetEncoderTask_attributes);

  /* creation of CopeKeyTask */
  CopeKeyTaskHandle = osThreadNew(StartCopeKeyTask, NULL, &CopeKeyTask_attributes);

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

/* USER CODE BEGIN Header_StartOLEDupdateTask */
/**
* @brief Function implementing the OLEDupdateTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOLEDupdateTask */
void StartOLEDupdateTask(void *argument)
{
  /* USER CODE BEGIN StartOLEDupdateTask */
  /* Infinite loop */
  for(;;)
  {
	  if(osSemaphoreAcquire(OLEDupdateSemHandle, 1000) == osOK)
	  {
		  OLED_updata();
	  }
  }
  /* USER CODE END StartOLEDupdateTask */
}

/* USER CODE BEGIN Header_StartReadADCTask */
/**
* @brief Function implementing the ReadADCTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReadADCTask */
void StartReadADCTask(void *argument)
{
  /* USER CODE BEGIN StartReadADCTask */
  /* Infinite loop */
  for(;;)
  {
	  osDelay(100);
	  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)RPValue, 4);

  }
  /* USER CODE END StartReadADCTask */
}

/* USER CODE BEGIN Header_StartSstMotorTask */
/**
* @brief Function implementing the SstMotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSstMotorTask */
void StartSstMotorTask(void *argument)
{
  /* USER CODE BEGIN StartSstMotorTask */
  /* Infinite loop */
  for(;;)
  {
	  if (osSemaphoreAcquire(MotorUpdateSemHandle, 1000) == osOK)
	  {
		  if(PWMOutPut==0){motorSet(MOTOR_A, MOTOR_BREAK, 0);}
		  if(PWMOutPut>0){motorSet(MOTOR_A, MOTOR_FORWARD, PWMOutPut);}
		  if(PWMOutPut<0){motorSet(MOTOR_A, MOTOR_BREAK, -PWMOutPut);}
	  }
  }
  /* USER CODE END StartSstMotorTask */
}

/* USER CODE BEGIN Header_StartUSART1Task */
/**
* @brief Function implementing the USART1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUSART1Task */
void StartUSART1Task(void *argument)
{
  /* USER CODE BEGIN StartUSART1Task */
  /* Infinite loop */
  for(;;)
  {
//	  uint8_t UART_SetSpeedBuf[sizeof(int)],UART_RealSpeedBuf[sizeof(int)],UART_PWMOutPutBuf[sizeof(int)];
	  UART1SendBuf[0]=(float)SetSpeed;
	  UART1SendBuf[1]=(float)RealSpeed;
	  UART1SendBuf[2]=(float)(PWMOutPut/36);
	  UART1SendBuf[3]=(float)ErrorRealTime;
	  HAL_UART_Transmit(&huart1, (uint8_t*)UART1SendBuf, sizeof(float)*4, HAL_MAX_DELAY);
	  HAL_UART_Transmit(&huart1, tail, 4, HAL_MAX_DELAY);
	  osDelay(10);
  }
  /* USER CODE END StartUSART1Task */
}

/* USER CODE BEGIN Header_StartPIDupdateTask */
/**
* @brief Function implementing the PIDupdateTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPIDupdateTask */
void StartPIDupdateTask(void *argument)
{
  /* USER CODE BEGIN StartPIDupdateTask */
  /* Infinite loop */
  for(;;)
  {
	if (osSemaphoreAcquire(PIDupdateSemHandle, 1000) == osOK)
	{
		ErrorLast1Time=ErrorRealTime;
		ErrorRealTime=SetSpeed-RealSpeed;
		if (fabs(ki) > 0.001){ErrorInt+=ErrorRealTime;}
		PWMOutPut=kp*ErrorRealTime+ki*ErrorInt+kd*(ErrorRealTime-ErrorLast1Time);
		if(PWMOutPut>3600){PWMOutPut=3600;}
		if(PWMOutPut<-3600){PWMOutPut=-3600;}
		osSemaphoreRelease (MotorUpdateSemHandle);
	}
  }
  /* USER CODE END StartPIDupdateTask */
}

/* USER CODE BEGIN Header_StartGetEncoderTask */
/**
* @brief Function implementing the GetEncoderTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGetEncoderTask */
void StartGetEncoderTask(void *argument)
{
  /* USER CODE BEGIN StartGetEncoderTask */
  /* Infinite loop */
  for(;;)
  {
	  countRealTime =__HAL_TIM_GET_COUNTER(&htim3);
	  osDelay(10);
	  if((countRealTime - countLastTime)<100 && (countRealTime - countLastTime)>-100){RealSpeed = countRealTime - countLastTime;}
	  osSemaphoreRelease (PIDupdateSemHandle);
	  OLED_showSignedNUM(0, 0, RealSpeed, 8);
	  countLastTime = countRealTime;
	  osSemaphoreRelease (OLEDupdateSemHandle);

  }
  /* USER CODE END StartGetEncoderTask */
}

/* USER CODE BEGIN Header_StartCopeKeyTask */
/**
* @brief Function implementing the CopeKeyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCopeKeyTask */
void StartCopeKeyTask(void *argument)
{
  /* USER CODE BEGIN StartCopeKeyTask */
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
	  if((keyTrigState&0x01)==0x01){SetSpeed-=5;}
	  if((keyTrigState&0x02)==0x02){SetSpeed+=5;}
	  if((keyTrigState&0x04)==0x04){SetSpeed=0;}
	  if((keyTrigState&0x08)==0x08){}
	  keyTrigState=0;
	  osDelay(20);
	  KeyStateLastTime=KeyStateRealTime;
//	  OLED_showHEX(2, 0, KeyStateRealTime, 8);
//	  OLED_showHEX(4, 0, KeyStateLastTime, 8);
//	  OLED_showHEX(6, 0, keyTrigState, 8);
//	  osSemaphoreRelease (OLEDupdateSemHandle);

  }
  /* USER CODE END StartCopeKeyTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

