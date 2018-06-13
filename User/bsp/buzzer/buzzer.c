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
#include "buzzer.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
IO_TypeDef buzzer = {BUZZER_GPIO_Port, BUZZER_Pin};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
 * @brief 蜂鸣器打开
 */
void Buzzer_On(void)
{
    IO_WritePin(&buzzer, 1);
}

/**
 * @brief 蜂鸣器关闭
 */
void Buzzer_Off(void)
{
    IO_WritePin(&buzzer, 0);
}
