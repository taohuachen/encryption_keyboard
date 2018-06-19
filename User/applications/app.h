/**
 *******************************************************************************
 * @file    :
 * @author  :
 * @version :
 * @date    :
 * @brief   :
 *******************************************************************************
 * @attention
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_H
#define __APP_H

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "led.h"
#include "switch.h"
#include "keyboard.h"
#include "buzzer.h"
#include "spi_flash_w25qxx.h"

/* Variables -----------------------------------------------------------------*/
extern void StartTaskFunc(void const *argument);
extern void MasterTaskFunc(void const *argument);
extern void KeyboardTaskFunc(void const *argument);
extern void BuzzerTaskFunc(void const *argument);

extern osThreadId KeyboardTaskHandle;
extern osThreadId BuzzerTaskHandle;

extern osSemaphoreId BuzzerBinarySemHandle;
extern osSemaphoreId W25qxxBinarySemHandle;

extern osMessageQId ModeQueueHandle;
extern osMessageQId AddrQueueHandle;
extern osMessageQId KeyQueueHandle;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* End of define module include ----------------------------------------------*/
#endif
