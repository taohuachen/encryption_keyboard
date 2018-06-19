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
struct
{
    uint8_t mode;
    uint8_t address;
    uint8_t key;

} master_global;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void MasterTaskFunc(void const *argument)
{
    osEvent event;

    for (;;)
    {
        printf("MasterTask is running!\n");

        event = osMessageGet(ModeQueueHandle, 0);
        if (event.status == osEventMessage)
        {
            master_global.mode = event.value.v;
        }

        event = osMessageGet(AddrQueueHandle, 0);
        if (event.status == osEventMessage)
        {
            master_global.address = event.value.v;
        }

        event = osMessageGet(KeyQueueHandle, 0);
        if (event.status == osEventMessage)
        {
            master_global.key = event.value.v;
        }
        else
        {
            master_global.key = KB_VALUE_NULL;
        }

        osDelay(100);
    }
}
