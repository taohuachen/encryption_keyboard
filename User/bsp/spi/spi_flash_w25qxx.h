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

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint8_t W25QXX_ReadSR(void);
void W25QXX_WriteSR(uint8_t sr);
void W25QXX_ReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte);
void W25QXX_FastReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte);
void W25QXX_WritePage(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);
void W25QXX_WriteNoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);
void W25QXX_WriteNByte(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);
void W25QXX_EraseSector4K(uint32_t SectorAddr);
void W25QXX_EraseBlock32K(uint32_t BlockAddr);
void W25QXX_EraseBlock64K(uint32_t BlockAddr);
void W25QXX_EraseChip(void);
void W25QXX_PowerDown(void);
void W25QXX_WAKEUP(void);
uint8_t W25QXX_ReadDeviceID(void);
uint16_t W25QXX_ReadManufactDeviceID(void);
uint32_t W25QXX_ReadJedecID(void);

/* End of define module include ----------------------------------------------*/
#endif
