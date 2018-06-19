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
#ifndef __BUFFER_H
#define __BUFFER_H

/* Includes ------------------------------------------------------------------*/
#include "lib_def.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"

/* Exported types ------------------------------------------------------------*/
/**
 * BUFFER_TypeDef structure definition
 */
typedef struct
{
    uint32_t Len;      /*缓冲区长度*/
    uint32_t EmptyCnt; /*缓冲区空余长度*/
    uint32_t IxRd;     /*读指针*/
    uint32_t IxWr;     /*写指针*/
    uint8_t *DataPtr;  /*指向缓冲区数据片的指针*/

} BUFFER_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Buffer_Init(BUFFER_TypeDef *pbuf, uint8_t *pdata, uint32_t len, bool full); /*初始化缓冲区*/
void Buffer_Clear(BUFFER_TypeDef *pbuf);                                         /*清空缓冲区*/
bool Buffer_IsEmpty(BUFFER_TypeDef *pbuf);                                       /*确定缓冲区是否空*/
bool Buffer_IsFull(BUFFER_TypeDef *pbuf);                                        /*确定缓冲区是否满*/
uint32_t Buffer_Size(BUFFER_TypeDef *pbuf);                                      /*检索缓冲区长度*/
uint32_t Buffer_DataLen(BUFFER_TypeDef *pbuf);                                   /*检索缓冲区数据片长度*/

/* 以下缓冲区读写函数使用时需要开启临界区保护 */
uint32_t Buffer_Read(BUFFER_TypeDef *pbuf, uint8_t *pdest, uint32_t len);  /*从缓冲区读取数据*/
uint32_t Buffer_Write(BUFFER_TypeDef *pbuf, uint8_t *pdest, uint32_t len); /*往缓冲区写入数据*/
bool Buffer_ReadOctet(BUFFER_TypeDef *pbuf, uint8_t *pdatum);              /*从缓冲区读取1字节数据*/
bool Buffer_WriteOctet(BUFFER_TypeDef *pbuf, uint8_t datum);               /*往缓冲区写入1字节数据*/

/* End of define module include ----------------------------------------------*/
#endif
