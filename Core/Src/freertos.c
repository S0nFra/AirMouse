/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "mpu6050.h"
#include "string.h"
#include "printf.h"
#include "math.h"
#include "ds3231.h"
#include "File_Handling_RTOS.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
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

/* Variables of the two MPU6050 on index hand and thumb declared in main.c */
extern MPU6050 mpu_ind;
extern MPU6050 mpu_pol;

/* Extern variables for I2C and USART ports */
extern USART_HandleTypeDef husart2;
extern I2C_HandleTypeDef hi2c1;

/* Useful variables for file management declared in File_Handling_RTOS.c */
extern FIL fil; // File
extern UINT br, bw;  // File read/write count

/******* ALT + TAB ********/
/* ALT_TAB_flag is 1 when we are in ALT+TAB modality (vertical hand), 0 otherwise. It is used
 * when we want to know if we are in ALT+TAB mode or not. */
uint8_t ALT_TAB_flag = 0;
/* ALT_TAB_move is 1 even, except when we pass from ALT+TAB mode to normal mode. It is used
 * when we don't want to move our mouse when we pass from ALT+TAB mode to normal mode. */
uint8_t ALT_TAB_move = 1;

/********* CLICKS *********/
/* numClick is an integer variable that counts the number of clicks occurred in a period. */
uint8_t numClick = 0;
/* clickFlag is 1 when we are in click phase and we don't want to move our mouse, 0 otherwise. */
uint8_t clickFlag = 0;
/* moveFlag is 1 when we have already detected 1 click, and we don't want to move our mouse
 * because there are a chance for a double click */
uint8_t moveFlag = 1;

/******** ZOOM ********/
/* numZoom is an integer variable that counts the number of zoom command occurred in a period. */
uint8_t numZoom = 0;
/* zoomFlag is 1 when we are in zoom phase and we don't want to move our mouse, 0 otherwise. */
uint8_t zoomFlag = 0;

/* This variables is setted by the click and zoom callback to log click, double click, zoom in and
 * zoom out.
 * logEvent=1 --> log a click,
 * logEvent=2 --> log a double click,
 * logEvent=3 --> log a zoom in,
 * logEvent=4 --> log a zoom out
 * */
int8_t logEvent = -1;

/* Log's string length*/
#define LOG_STRING_LEN 200

/* USER CODE END Variables */
/* Definitions for readTask */
osThreadId_t readTaskHandle;
const osThreadAttr_t readTask_attributes = {
  .name = "readTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for commandTask */
osThreadId_t commandTaskHandle;
const osThreadAttr_t commandTask_attributes = {
  .name = "commandTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SDTask */
osThreadId_t SDTaskHandle;
const osThreadAttr_t SDTask_attributes = {
  .name = "SDTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for EventQueue */
osMessageQueueId_t EventQueueHandle;
const osMessageQueueAttr_t EventQueue_attributes = {
  .name = "EventQueue"
};
/* Definitions for clickTimer */
osTimerId_t clickTimerHandle;
const osTimerAttr_t clickTimer_attributes = {
  .name = "clickTimer"
};
/* Definitions for timerZoom */
osTimerId_t timerZoomHandle;
const osTimerAttr_t timerZoom_attributes = {
  .name = "timerZoom"
};
/* Definitions for moveTimer */
osTimerId_t moveTimerHandle;
const osTimerAttr_t moveTimer_attributes = {
  .name = "moveTimer"
};
/* Definitions for altTabTimer */
osTimerId_t altTabTimerHandle;
const osTimerAttr_t altTabTimer_attributes = {
  .name = "altTabTimer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* This function is used to log a relative movement of the mouse with x and y as values in their respective axes.*/
void logMovement(int x, int y) {
	char log[LOG_STRING_LEN];
	/* Get the current timestamp and print the string */
	date_time_t datetime;
	rtc_get_date_time(&datetime);
	sprintf(log,
			"[%d-%d-%d - %d:%d:%d]\tEVENT: relative movement x:%d y:%d\t========\tAcc. Values: {T: %.2f, %.2f, %.2f, %.2f, %.2f, %.2f || I: %.2f, %.2f, %.2f, %.2f, %.2f, %.2f}\n",
			datetime.date, datetime.month, datetime.year, datetime.hours,
			datetime.minutes, datetime.seconds, x, y, mpu_pol.accelerometer_X,
			mpu_pol.accelerometer_Y, mpu_pol.accelerometer_Z,
			mpu_pol.gyroscope_X, mpu_pol.gyroscope_Y, mpu_pol.gyroscope_Z,
			mpu_ind.accelerometer_X, mpu_ind.accelerometer_Y,
			mpu_ind.accelerometer_Z, mpu_ind.gyroscope_X, mpu_ind.gyroscope_Y,
			mpu_ind.gyroscope_Z);
	/* Insert the string into the queue */
	if (osMessageQueuePut(EventQueueHandle, (void*) &log, 0U, 0U) != osOK) {
		HAL_USART_Transmit(&husart2, (uint8_t*) "QUEUE INSERT ERROR\n",
				sizeof(char) * strlen("QUEUE INSERT ERROR\n"), HAL_MAX_DELAY);
	}
}

/*
 * This function is used to log a gesture codified with event parameter according to the values:
 * 1 -> left click
 * 2 -> double click
 * 3 -> zoom in
 * 4 -> zoom out
 * 5 -> ALT+TAB
 * 6 -> ALT+TAB windows switch
 * 7 -> ALT+TAB release
 * */
void logGestures(int event) {
	char log[LOG_STRING_LEN];
	date_time_t datetime;
	rtc_get_date_time(&datetime);
	char eventString[50];

	if (event == 1) {
		sprintf(eventString, "left click");
	} else if (event == 2) {
		sprintf(eventString, "double click");
	} else if (event == 3) {
		sprintf(eventString, "zoom in");
	} else if (event == 4) {
		sprintf(eventString, "zoom out");
	} else if (event == 5) {
		sprintf(eventString, "ALT+TAB");
	} else if (event == 6) {
		sprintf(eventString, "ALT+TAB Switch");
	} else if (event == 7) {
		sprintf(eventString, "ALT+TAB Release");
	}

	sprintf(log,
			"[%d-%d-%d - %d:%d:%d]\tEVENT: %s\t========\tAcc. Values: {T: %.2f, %.2f, %.2f, %.2f, %.2f, %.2f || I: %.2f, %.2f, %.2f, %.2f, %.2f, %.2f}\n",
			datetime.date, datetime.month, datetime.year, datetime.hours,
			datetime.minutes, datetime.seconds, eventString,
			mpu_pol.accelerometer_X, mpu_pol.accelerometer_Y,
			mpu_pol.accelerometer_Z, mpu_pol.gyroscope_X, mpu_pol.gyroscope_Y,
			mpu_pol.gyroscope_Z, mpu_ind.accelerometer_X,
			mpu_ind.accelerometer_Y, mpu_ind.accelerometer_Z,
			mpu_ind.gyroscope_X, mpu_ind.gyroscope_Y, mpu_ind.gyroscope_Z);
	/* Insert the log string into the Queue */
	if (osMessageQueuePut(EventQueueHandle, (void*) &log, 0U, 0U) != osOK) {
		HAL_USART_Transmit(&husart2, (uint8_t*) "QUEUE INSERT ERROR\n",
				sizeof(char) * strlen("QUEUE INSERT ERROR\n"), HAL_MAX_DELAY);
	}

}
/* USER CODE END FunctionPrototypes */

void StartReadTask(void *argument);
void StartCommandTask(void *argument);
void StartSDTask(void *argument);
void clickCallback(void *argument);
void zoomCallback(void *argument);
void moveTimerCallback(void *argument);
void altTabCallback(void *argument);

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of clickTimer */
  clickTimerHandle = osTimerNew(clickCallback, osTimerOnce, NULL, &clickTimer_attributes);

  /* creation of timerZoom */
  timerZoomHandle = osTimerNew(zoomCallback, osTimerOnce, NULL, &timerZoom_attributes);

  /* creation of moveTimer */
  moveTimerHandle = osTimerNew(moveTimerCallback, osTimerOnce, NULL, &moveTimer_attributes);

  /* creation of altTabTimer */
  altTabTimerHandle = osTimerNew(altTabCallback, osTimerOnce, NULL, &altTabTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of EventQueue */
  EventQueueHandle = osMessageQueueNew (16,LOG_STRING_LEN* sizeof(char), &EventQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of readTask */
  readTaskHandle = osThreadNew(StartReadTask, NULL, &readTask_attributes);

  /* creation of commandTask */
  commandTaskHandle = osThreadNew(StartCommandTask, NULL, &commandTask_attributes);

  /* creation of SDTask */
  SDTaskHandle = osThreadNew(StartSDTask, NULL, &SDTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartReadTask */
/**
 * @brief  Function implementing the readTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadTask */
void StartReadTask(void *argument)
{
  /* USER CODE BEGIN StartReadTask */
	/* Infinite loop */
	for (;;) {
		/* Read the values from the index hand's accelerometer */
		MPU6050_ReadGyroscope(&hi2c1, &mpu_ind);
		MPU6050_ReadAccelerometer(&hi2c1, &mpu_ind);
		/* Read the values from the thumb's accelerometer */
		MPU6050_ReadGyroscope(&hi2c1, &mpu_pol);
		MPU6050_ReadAccelerometer(&hi2c1, &mpu_pol);

		osDelay(25);
	}
  /* USER CODE END StartReadTask */
}

/* USER CODE BEGIN Header_StartCommandTask */
/**
 * @brief Function implementing the commandTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCommandTask */
void StartCommandTask(void *argument)
{
  /* USER CODE BEGIN StartCommandTask */

	char command[100];

	/* Infinite loop */
	for (;;) {
		/* If there are clicks, double clicks, zoom in/out, then log it */
		if (logEvent != -1) {
			logGestures(logEvent);
			logEvent = -1;
		}
		/* Initial click detecting */
		if (mpu_ind.gyroscope_X <= -240 && mpu_ind.accelerometer_Z < 0
				&& clickFlag == 0 && ALT_TAB_flag == 0) {
			clickFlag = 1;
			/* One click detected */
		} else if (clickFlag == 1 && mpu_ind.gyroscope_X >= 240) {
			numClick += 1;
			clickFlag = 0;
			moveFlag = 0;
			osTimerStart(moveTimerHandle, 60);
			osTimerStart(clickTimerHandle, 600);
			/* Initial Zoom In detecting*/
		} else if (mpu_pol.gyroscope_X <= -240 && mpu_pol.accelerometer_Z < 0
				&& zoomFlag == 0 && ALT_TAB_flag == 0 && ALT_TAB_move == 1) {
			zoomFlag = 1;
			/* Zoom In detected */
		} else if (zoomFlag == 1 && mpu_pol.gyroscope_X >= 240) {
			numZoom += 1;
			zoomFlag = 0;
			osTimerStart(timerZoomHandle, 600);
			/* Alt+Tab detected */
		} else if (mpu_pol.gyroscope_Y >= 230 && ALT_TAB_flag == 0
				&& zoomFlag == 0 && numZoom == 0) {

			sprintf(command, "xdotool keydown alt key Tab\n");
			HAL_USART_Transmit(&husart2, (uint8_t*) command,
					sizeof(char) * strlen(command), HAL_MAX_DELAY);
			logGestures(5);
			ALT_TAB_flag = 1;
			ALT_TAB_move = 0;
			osTimerStart(altTabTimerHandle, 120);
			/* Switch windows in Alt+Tab modality */
		} else if (mpu_ind.gyroscope_X <= -240 && ALT_TAB_flag == 1
				&& ALT_TAB_move == 1) {
			sprintf(command,
					"xdotool keydown Tab && sleep 0.1 && xdotool keyup Tab\n");
			HAL_USART_Transmit(&husart2, (uint8_t*) command,
					sizeof(char) * strlen(command), HAL_MAX_DELAY);
			logGestures(6);
			ALT_TAB_move = 0;
			osTimerStart(altTabTimerHandle, 600);
		}
		/* Release Alt+Tab modality */
		else if (ALT_TAB_flag == 1 && mpu_pol.gyroscope_Y <= -230) {
			sprintf(command, "xdotool keyup alt\n");
			HAL_USART_Transmit(&husart2, (uint8_t*) command,
					sizeof(char) * strlen(command), HAL_MAX_DELAY);
			logGestures(7);
			ALT_TAB_flag = 0;
			ALT_TAB_move = 0;
			osTimerStart(altTabTimerHandle, 600);
		}
		/* Mouse Movements */
		else if (numClick == 0 && clickFlag == 0 && moveFlag == 1
				&& ALT_TAB_flag == 0 && zoomFlag == 0 && numZoom == 0
				&& ALT_TAB_move == 1) {

			int m_x = (-(int) mpu_ind.accelerometer_X) * 3;
			int m_y = -((int) mpu_ind.accelerometer_Y) * 2;
			/* Mouse Movements */
			if (fabs(m_x) + fabs(m_y) >= 10) {

				sprintf(command, "xdotool mousemove_relative -- %d %d\n", m_x,
						m_y);
				HAL_USART_Transmit(&husart2, (uint8_t*) command,
						sizeof(char) * strlen(command), HAL_MAX_DELAY);
				logMovement(m_x, m_y);
			}
		}

		osDelay(25);
	}
  /* USER CODE END StartCommandTask */
}

/* USER CODE BEGIN Header_StartSDTask */
/**
 * @brief Function implementing the SDTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSDTask */
void StartSDTask(void *argument)
{
  /* USER CODE BEGIN StartSDTask */
	/* Infinite loop */
	for (;;) {
		char buffer[LOG_STRING_LEN];

		for (int i = 0; i < 4; i++) {
			if (osMessageQueueGet(EventQueueHandle, (void*) &buffer, NULL, 0U)
					== osOK) {
				/* Writing text */
				if (f_write(&fil, buffer, strlen(buffer), &bw) != FR_OK) {
					HAL_USART_Transmit(&husart2,
							(uint8_t*) "ERROR IN WRITING FILE\n",
							sizeof(char) * strlen("ERROR IN WRITING FILE\n"),
							HAL_MAX_DELAY);
				}

			} else {
				break;
			}
		}
		f_sync(&fil);

		osDelay(100);
	}
  /* USER CODE END StartSDTask */
}

/* clickCallback function */
void clickCallback(void *argument)
{
  /* USER CODE BEGIN clickCallback */
	char value[100];
	if (numClick >= 2) {
		sprintf(value, "xdotool click --repeat 2 1\n");

	} else if (numClick == 1) {
		sprintf(value, "xdotool click 1\n");
	}
	HAL_USART_Transmit(&husart2, (uint8_t*) value, sizeof(char) * strlen(value),
	HAL_MAX_DELAY);
	logEvent = numClick;
	numClick = 0;

  /* USER CODE END clickCallback */
}

/* zoomCallback function */
void zoomCallback(void *argument)
{
  /* USER CODE BEGIN zoomCallback */
	char value[100];

	if (numZoom >= 2) {
		sprintf(value, "xdotool key Ctrl+minus\n");
		logEvent = 4;
	} else if (numZoom == 1) {
		sprintf(value, "xdotool key Ctrl+plus\n");
		logEvent = 3;
	}
	HAL_USART_Transmit(&husart2, (uint8_t*) value, sizeof(char) * strlen(value),
	HAL_MAX_DELAY);

	numZoom = 0;
  /* USER CODE END zoomCallback */
}

/* moveTimerCallback function */
void moveTimerCallback(void *argument)
{
  /* USER CODE BEGIN moveTimerCallback */
	moveFlag = 1;
  /* USER CODE END moveTimerCallback */
}

/* altTabCallback function */
void altTabCallback(void *argument)
{
  /* USER CODE BEGIN altTabCallback */
	ALT_TAB_move = 1;
  /* USER CODE END altTabCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

