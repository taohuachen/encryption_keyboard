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
#ifndef __KS0108_H
#define __KS0108_H

/* Includes ------------------------------------------------------------------*/
#include "io.h"
#include "delay.h"
#include "lib_def.h"
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void LCD_Init(void);
void LCD_Reset(void);
void LCD_WriteCmd(uint8_t cmd);
void LCD_WriteData(uint8_t data);
void LCD_DispOn(void);
void LCD_DispOff(void);
void LCD_SetXY(uint8_t x, uint8_t y);
void LCD_WriteByte(uint8_t x, uint8_t y, uint8_t byte);
void LCD_Clear(void);
void LCD_SetBrightness(uint8_t ratio);

void LCD_SetPixel(uint8_t x, uint8_t y, uint8_t PixelIndex);
uint8_t LCD_GetPixel(uint8_t x, uint8_t y);
void LCD_Display(void);

/* End of define module include ----------------------------------------------*/
#endif
