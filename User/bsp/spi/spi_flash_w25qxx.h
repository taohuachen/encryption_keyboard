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
#ifndef __SPI_FLASH_W25QXX_H
#define __SPI_FLASH_W25QXX_H

/* Includes ------------------------------------------------------------------*/
#include "spi.h"
#include "io.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* w25qxx芯片工作模式选择，当选择非阻塞模式时需要RTOS（默认FreeRTOS） */
#define W25QXX_MODE_NON_BLOCKING 0 /* 0：阻塞模式  1：非阻塞模式 */

/* w25qxx芯片GPIO配置 */
#define W25qxx_hspi hspi2
#define W25qxx_Init() MX_SPI2_Init()
#define W25qxx_CS_GPIO_Port W25QXX_NSS_GPIO_Port
#define W25qxx_CS_Pin W25QXX_NSS_Pin

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void W25qxx_WriteEnable(void);
void W25qxx_WriteDisable(void);
uint8_t W25qxx_ReadSR1(void);
void W25qxx_WriteSR1(uint8_t sr);
void W25qxx_ReadData(uint8_t *pBuffer, uint32_t Addr, uint16_t nByte);
void W25qxx_PageProgram(uint8_t *pBuffer, uint32_t Addr, uint16_t nByte);
void W25qxx_WriteNoCheck(uint8_t *pBuffer, uint32_t Addr, uint16_t nByte);
void W25qxx_WriteData(uint8_t *pBuffer, uint32_t Addr, uint16_t nByte);
void W25qxx_EraseSector4K(uint32_t SectorAddr);
void W25qxx_EraseBlock32K(uint32_t BlockAddr);
void W25qxx_EraseBlock64K(uint32_t BlockAddr);
void W25qxx_EraseChip(void);
void W25qxx_PowerDown(void);
void W25qxx_ReleasePowerDown(void);
uint8_t W25qxx_ReadDeviceID(void);
uint16_t W25qxx_ReadManufactDeviceID(void);
uint32_t W25qxx_ReadJedecID(void);
uint32_t W25Qxx_GetFlashSize(void);

/* End of define module include ----------------------------------------------*/
#endif
