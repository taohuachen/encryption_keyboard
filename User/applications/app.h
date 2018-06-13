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

/* Variables -----------------------------------------------------------------*/
extern void KeyboardTaskFunc(void const *argument);
extern void BuzzerTaskFunc(void const *argument);

extern osThreadId KeyboardTaskHandle;
extern osThreadId BuzzerTaskHandle;

extern osSemaphoreId BuzzerBinarySemHandle;

extern osMessageQId ModeQueueHandle;
extern osMessageQId AddrQueueHandle;
extern osMessageQId KeyQueueHandle;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* End of define module include ----------------------------------------------*/
#endif
