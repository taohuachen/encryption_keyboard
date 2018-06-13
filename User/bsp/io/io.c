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
#include "io.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
 * @brief      读单个GPIO状态
 * @param  io  GPIO
 * @return     GPIO电平值
 */
uint32_t IO_ReadPin(IO_TypeDef *io)
{
    return HAL_GPIO_ReadPin(io->Port, io->Pin);
}

/**
 * @brief         写单个GPIO状态
 * @param  io     GPIO
 * @param  state  GPIO电平值
 * @return        None
 */
void IO_WritePin(IO_TypeDef *io, uint32_t state)
{
    HAL_GPIO_WritePin(io->Port, io->Pin, (GPIO_PinState)state);
}

/**
 * @brief      取反单个GPIO状态
 * @param  io  GPIO
 * @return     None
 */
void IO_TogglePin(IO_TypeDef *io)
{
    HAL_GPIO_TogglePin(io->Port, io->Pin);
}

/**
 * @brief      读n个GPIO状态
 * @param  io  GPIO
 * @param  n   GPIO数量
 * @return     GPIO电平值
 */
uint32_t IO_Read(IO_TypeDef *io, uint8_t n)
{
    uint32_t value = 0;
    uint8_t i;

    for (i = 0; i < n; i++)
    {
        value |= IO_ReadPin(&io[i]) << i;
    }

    return value;
}

/**
 * @brief         写n个GPIO状态
 * @param  io     GPIO
 * @param  value  GPIO电平值
 * @param  n      GPIO数量
 * @return        None
 */
void IO_Write(IO_TypeDef *io, uint32_t value, uint8_t n)
{
    uint8_t i;

    for (i = 0; i < n; i++)
    {
        IO_WritePin(&io[i], (value >> i) & 1u);
    }
}
