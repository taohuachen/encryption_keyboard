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

/* Includes ------------------------------------------------------------------*/
#include "app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void KeyboardTaskFunc(void const *argument)
{
    uint8_t pre_key = KB_VALUE_NULL;
    uint8_t now_key = KB_VALUE_NULL;

    for (;;)
    {
        printf("KeyboardTask is running!\n");

        pre_key = now_key;
        now_key = Keyboard_Scan();

        if ((pre_key == KB_VALUE_NULL) && (now_key != KB_VALUE_NULL))
        {
            osMessagePut(KeyQueueHandle, now_key, osWaitForever);
            osSemaphoreRelease(BuzzerBinarySemHandle);
        }

        osDelay(20);
    }
}
