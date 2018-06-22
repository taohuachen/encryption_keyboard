/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BUZZER_Pin GPIO_PIN_2
#define BUZZER_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOE
#define LCD_DB0_Pin GPIO_PIN_1
#define LCD_DB0_GPIO_Port GPIOA
#define LCD_DB1_Pin GPIO_PIN_2
#define LCD_DB1_GPIO_Port GPIOA
#define LCD_DB2_Pin GPIO_PIN_3
#define LCD_DB2_GPIO_Port GPIOA
#define LCD_DB3_Pin GPIO_PIN_4
#define LCD_DB3_GPIO_Port GPIOA
#define LCD_DB4_Pin GPIO_PIN_5
#define LCD_DB4_GPIO_Port GPIOA
#define LCD_DB5_Pin GPIO_PIN_6
#define LCD_DB5_GPIO_Port GPIOA
#define LCD_DB6_Pin GPIO_PIN_7
#define LCD_DB6_GPIO_Port GPIOA
#define LCD_DB7_Pin GPIO_PIN_4
#define LCD_DB7_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_5
#define LCD_RS_GPIO_Port GPIOC
#define LCD_BRIGHT_Pin GPIO_PIN_0
#define LCD_BRIGHT_GPIO_Port GPIOB
#define LCD_RW_Pin GPIO_PIN_1
#define LCD_RW_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_7
#define LCD_EN_GPIO_Port GPIOE
#define LCD_CS1_Pin GPIO_PIN_8
#define LCD_CS1_GPIO_Port GPIOE
#define LCD_CS2_Pin GPIO_PIN_9
#define LCD_CS2_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_10
#define LCD_RST_GPIO_Port GPIOE
#define KB_ROW5_Pin GPIO_PIN_11
#define KB_ROW5_GPIO_Port GPIOE
#define KB_ROW4_Pin GPIO_PIN_12
#define KB_ROW4_GPIO_Port GPIOE
#define KB_ROW3_Pin GPIO_PIN_13
#define KB_ROW3_GPIO_Port GPIOE
#define KB_ROW2_Pin GPIO_PIN_14
#define KB_ROW2_GPIO_Port GPIOE
#define KB_ROW1_Pin GPIO_PIN_15
#define KB_ROW1_GPIO_Port GPIOE
#define KB_COL4_Pin GPIO_PIN_10
#define KB_COL4_GPIO_Port GPIOB
#define KB_COL3_Pin GPIO_PIN_11
#define KB_COL3_GPIO_Port GPIOB
#define W25QXX_NSS_Pin GPIO_PIN_12
#define W25QXX_NSS_GPIO_Port GPIOB
#define W25QXX_SCK_Pin GPIO_PIN_13
#define W25QXX_SCK_GPIO_Port GPIOB
#define W25QXX_MISO_Pin GPIO_PIN_14
#define W25QXX_MISO_GPIO_Port GPIOB
#define W25QXX_MOSI_Pin GPIO_PIN_15
#define W25QXX_MOSI_GPIO_Port GPIOB
#define KB_COL2_Pin GPIO_PIN_8
#define KB_COL2_GPIO_Port GPIOD
#define KB_COL1_Pin GPIO_PIN_9
#define KB_COL1_GPIO_Port GPIOD
#define MODE_Pin GPIO_PIN_9
#define MODE_GPIO_Port GPIOA
#define ADDR1_Pin GPIO_PIN_8
#define ADDR1_GPIO_Port GPIOB
#define ADDR2_Pin GPIO_PIN_9
#define ADDR2_GPIO_Port GPIOB
#define ADDR3_Pin GPIO_PIN_0
#define ADDR3_GPIO_Port GPIOE
#define ADDR4_Pin GPIO_PIN_1
#define ADDR4_GPIO_Port GPIOE

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
