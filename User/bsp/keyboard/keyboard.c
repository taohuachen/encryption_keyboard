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
#include "keyboard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const uint8_t KB_VALUE[5][4] = {
    {0x01, 0x02, 0x03, 0x04},
    {0x05, 0x06, 0x07, 0x08},
    {0x09, 0x0a, 0x0b, 0x0c},
    {0x0d, 0x0e, 0x0f, 0x10},
    {0x11, 0x12, 0x13, 0x14},
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
IO_TypeDef keyboard_row[] = {
    {KB_ROW1_GPIO_Port, KB_ROW1_Pin},
    {KB_ROW2_GPIO_Port, KB_ROW2_Pin},
    {KB_ROW3_GPIO_Port, KB_ROW3_Pin},
    {KB_ROW4_GPIO_Port, KB_ROW4_Pin},
    {KB_ROW5_GPIO_Port, KB_ROW5_Pin},
};

IO_TypeDef keyboard_col[] = {
    {KB_COL1_GPIO_Port, KB_COL1_Pin},
    {KB_COL2_GPIO_Port, KB_COL2_Pin},
    {KB_COL3_GPIO_Port, KB_COL3_Pin},
    {KB_COL4_GPIO_Port, KB_COL4_Pin},
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
uint8_t Keyboard_Scan(void)
{
    uint8_t key = KB_VALUE_NULL;
    uint8_t rown = sizeof(keyboard_row) / sizeof(IO_TypeDef);
    uint8_t coln = sizeof(keyboard_col) / sizeof(IO_TypeDef);
    uint8_t out_max = (1u << rown) - 1;
    uint8_t in_max = (1u << coln) - 1;
    uint8_t out;
    uint8_t in;
    uint8_t i, j;

    IO_Write(keyboard_row, 0, rown);
    in = IO_Read(keyboard_col, coln);

    if (in < in_max)
    {
        for (i = 0; i < rown; i++)
        {
            out = DEF_BIT_CLR(out_max, 1u << i);
            IO_Write(keyboard_row, out, rown);
            in = IO_Read(keyboard_col, coln);

            for (j = 0; j < coln; j++)
            {
                if ((in & (1u << j)) == 0)
                {
                    key = KB_VALUE[i][j];
                    return key;
                }
            }
        }
    }

    return key;
}
