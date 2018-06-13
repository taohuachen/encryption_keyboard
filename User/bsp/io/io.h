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
#ifndef __IO_H
#define __IO_H

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    GPIO_TypeDef *Port;
    uint16_t Pin;

} IO_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* 操作单个GPIO状态 */
uint32_t IO_ReadPin(IO_TypeDef *io);
void IO_WritePin(IO_TypeDef *io, uint32_t state);
void IO_TogglePin(IO_TypeDef *io);

/* 操作n个GPIO状态 */
uint32_t IO_Read(IO_TypeDef *io, uint8_t n);
void IO_Write(IO_TypeDef *io, uint32_t value, uint8_t n);

/* End of define module include ----------------------------------------------*/
#endif
