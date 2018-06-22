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
#include "ks0108.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_RS_Write(value) IO_WritePin(&lcd_rs, value)
#define LCD_RW_Write(value) IO_WritePin(&lcd_rw, value)
#define LCD_EN_Write(value) IO_WritePin(&lcd_en, value)
#define LCD_CS1_Write(value) IO_WritePin(&lcd_cs1, value)
#define LCD_CS2_Write(value) IO_WritePin(&lcd_cs2, value)
#define LCD_RST_Write(value) IO_WritePin(&lcd_rst, value)
#define LCD_BRIGHT_Write(value) IO_WritePin(&lcd_bright, value)

#define LCD_DB_Write(value) IO_Write(lcd_db, value, 8)

#define LCD_Delay() delay(15)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
IO_TypeDef lcd_rs = {LCD_RS_GPIO_Port, LCD_RS_Pin};
IO_TypeDef lcd_rw = {LCD_RW_GPIO_Port, LCD_RW_Pin};
IO_TypeDef lcd_en = {LCD_EN_GPIO_Port, LCD_EN_Pin};
IO_TypeDef lcd_cs1 = {LCD_CS1_GPIO_Port, LCD_CS1_Pin};
IO_TypeDef lcd_cs2 = {LCD_CS2_GPIO_Port, LCD_CS2_Pin};
IO_TypeDef lcd_rst = {LCD_RST_GPIO_Port, LCD_RST_Pin};
IO_TypeDef lcd_bright = {LCD_BRIGHT_GPIO_Port, LCD_BRIGHT_Pin};

IO_TypeDef lcd_db[] = {
    {LCD_DB0_GPIO_Port, LCD_DB0_Pin},
    {LCD_DB1_GPIO_Port, LCD_DB1_Pin},
    {LCD_DB2_GPIO_Port, LCD_DB2_Pin},
    {LCD_DB3_GPIO_Port, LCD_DB3_Pin},
    {LCD_DB4_GPIO_Port, LCD_DB4_Pin},
    {LCD_DB5_GPIO_Port, LCD_DB5_Pin},
    {LCD_DB6_GPIO_Port, LCD_DB6_Pin},
    {LCD_DB7_GPIO_Port, LCD_DB7_Pin},
};

enum
{
    LCD_SIDE_BOTH,
    LCD_SIDE_LEFT,
    LCD_SIDE_RIGHT,
};

volatile uint32_t LCD_SideState = LCD_SIDE_BOTH;

uint8_t video_memory[8][128];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void LCD_CS_Write(void)
{
    switch (LCD_SideState)
    {
    case LCD_SIDE_BOTH:
        LCD_CS1_Write(1);
        LCD_CS2_Write(1);
        break;
    case LCD_SIDE_LEFT:
        LCD_CS1_Write(1);
        LCD_CS2_Write(0);
        break;
    case LCD_SIDE_RIGHT:
        LCD_CS1_Write(0);
        LCD_CS2_Write(1);
        break;
    default:
        LCD_CS1_Write(0);
        LCD_CS2_Write(0);
        break;
    }
}

/* Public functions ----------------------------------------------------------*/
void LCD_Init(void)
{
    LCD_EN_Write(1);
    LCD_RST_Write(1);
    LCD_Clear();
    LCD_DispOn();
}

void LCD_Reset(void)
{
    LCD_RST_Write(1);
    LCD_Delay();
    LCD_RST_Write(0);
    LCD_Delay();
    LCD_RST_Write(1);
}

void LCD_WriteCmd(uint8_t cmd)
{
    LCD_RS_Write(0);
    LCD_RW_Write(0);
    LCD_CS_Write();
    LCD_DB_Write(cmd);

    LCD_EN_Write(1);
    LCD_Delay();
    LCD_EN_Write(0);
    LCD_Delay();
    LCD_EN_Write(1);
}

void LCD_WriteData(uint8_t data)
{
    LCD_RS_Write(1);
    LCD_RW_Write(0);
    LCD_CS_Write();
    LCD_DB_Write(data);

    LCD_EN_Write(1);
    LCD_Delay();
    LCD_EN_Write(0);
    LCD_Delay();
    LCD_EN_Write(1);
}

void LCD_DispOn(void)
{
    LCD_WriteCmd(0x3f);
}

void LCD_DispOff(void)
{
    LCD_WriteCmd(0x3e);
}

void LCD_SetXY(uint8_t x, uint8_t y)
{
    if (x < 64)
    {
        LCD_SideState = LCD_SIDE_LEFT;
    }
    else
    {
        LCD_SideState = LCD_SIDE_RIGHT;
        x = x - 64;
    }

    y = y / 8;

    LCD_WriteCmd(0x40 + (x & 0x3f));
    LCD_WriteCmd(0xb8 + (y & 0x07));
}

void LCD_WriteByte(uint8_t x, uint8_t y, uint8_t byte)
{
    LCD_SetXY(x, y);
    LCD_WriteData(byte);
}

void LCD_Clear(void)
{
    uint8_t row, col;

    LCD_SideState = LCD_SIDE_BOTH;

    LCD_WriteCmd(0x40 + 0);

    for (row = 0; row < 8; row++)
    {
        LCD_WriteCmd(0xb8 + row);

        for (col = 0; col < 64; col++)
        {
            LCD_WriteData(0x00);
        }
    }
}

void LCD_SetBrightness(uint8_t ratio)
{
    if ((ratio > 0) && (ratio < 100))
    {
    }
    else if (ratio >= 100)
    {
        LCD_BRIGHT_Write(1);
    }
    else
    {
        LCD_BRIGHT_Write(0);
    }
}

void LCD_SetPixel(int x, int y, int PixelIndex)
{
    uint8_t byte;
    uint8_t pos;

    byte = video_memory[y / 8][x];
    pos = BIT_00 << (y % 8);

    (PixelIndex) ? (DEF_BIT_SET(byte, pos)) : (DEF_BIT_CLR(byte, pos));

    video_memory[y / 8][x] = byte;
}

unsigned int LCD_GetPixel(int x, int y)
{
    uint8_t byte;
    uint8_t pos;
    uint8_t PixelIndex;

    byte = video_memory[y / 8][x];
    pos = BIT_00 << (y % 8);

    (byte & pos) ? (PixelIndex = 1) : (PixelIndex = 0);

    return PixelIndex;
}

void LCD_Display(void)
{
    uint8_t row, col;

    LCD_SideState = LCD_SIDE_LEFT;

    LCD_WriteCmd(0x40 + 0);

    for (row = 0; row < 8; row++)
    {
        LCD_WriteCmd(0xb8 + row);

        for (col = 0; col < 64; col++)
        {
            LCD_WriteData(video_memory[row][col]);
        }
    }

    LCD_SideState = LCD_SIDE_RIGHT;

    LCD_WriteCmd(0x40 + 0);

    for (row = 0; row < 8; row++)
    {
        LCD_WriteCmd(0xb8 + row);

        for (col = 0; col < 64; col++)
        {
            LCD_WriteData(video_memory[row][col + 64]);
        }
    }
}
