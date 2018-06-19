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
#include "switch.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
IO_TypeDef mode = {MODE_GPIO_Port, MODE_Pin};

IO_TypeDef addr[] = {
    {ADDR1_GPIO_Port, ADDR1_Pin},
    {ADDR2_GPIO_Port, ADDR2_Pin},
    {ADDR3_GPIO_Port, ADDR3_Pin},
    {ADDR4_GPIO_Port, ADDR4_Pin},
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
uint8_t ModeRead(void)
{
    uint8_t value;

    value = IO_ReadPin(&mode);
    DEF_BIT_TOGGLE(value, BIT_00);

    return value;
}

uint8_t AddrRead(void)
{
    uint8_t value;
    uint8_t n = sizeof(addr) / sizeof(IO_TypeDef);

    value = IO_Read(addr, n);
    DEF_BIT_TOGGLE(value, (1u << n) - 1);

    return value;
}
