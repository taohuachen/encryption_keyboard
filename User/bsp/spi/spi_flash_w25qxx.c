/**
 *******************************************************************************
 * @file    :
 * @author  :
 * @version :
 * @date    :
 * @brief   :
 *******************************************************************************
 * @attention
 * 1、函数接口支持芯片类型：
 * 	  原则上支持市面上所有SPI通信的FLASH
 *    因芯片电气特性差异（如:电压、速度等）, 可能部分芯片不支持
 *
 * 2、W25QXX信息：
 * 页(Page) ------ 256Byte
 * 扇区(Sector) -- 4KByte ------ （等于16页）
 * 块(Block) ----- 64KByte ----- （等于16扇区）
 *
 * 3、读写操作：
 * 读 ------------ 一次最大读一页（256byte）
 * 写 ------------ 页
 * 擦除 ---------- 扇区、块、整个芯片
 *
 * 4、寄存器命令说明
 * BIT7 6   5   4   3   2   1   0
 * SPR  RV  TB  BP2 BP1 BP0 WEL BUSY
 * SPR：默认0，状态寄存器保护位，配合WP使用
 * TB，BP2，BP1，BP0：FLASH区域写保护设置
 * WEL：写使能锁定
 * BUSY：忙标记位（1，忙；0，空闲）
 * 默认：0x00
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "spi_flash_w25qxx.h"

#if W25QXX_MODE_NON_BLOCKING
#include "cmsis_os.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PAGE_SIZE (256)
#define SECTOR_SIZE (4 * 1024)
#define BLOCK_SIZE_32K (32 * 1024)
#define BLOCK_SIZE_64K (64 * 1024)

/* Manufacturer ID */
#define MF_ID (0xEF)

/* JEDEC ID */
#define W25Q16JV_ID (0x4015)
#define W25Q32JV_ID (0x4016)
#define W25Q64JV_ID (0x4017)
#define W25Q128JV_ID (0x4018)

/* Command List */
#define CMD_WRITE_ENABLE (0x06)  /* 写使能 */
#define CMD_WRITE_DISABLE (0x04) /* 写失能 */

#define CMD_DEVICE_ID (0xAB)              /* Device ID */
#define CMD_MANUFACTURER_DEVICE_ID (0x90) /* Manufacturer ID/Device ID */
#define CMD_JEDEC_ID (0x9F)               /* JEDEC ID */

#define CMD_READ_DATA (0x03) /* 读数据 */
#define CMD_FAST_READ (0x0B) /* 快读数据 */

#define CMD_PAGE_PROGRAM (0x02) /* 页编程 */

#define CMD_SECTOR_ERASE_4K (0x20) /* 擦除扇区(4K) */
#define CMD_BLOCK_ERASE_32K (0x52) /* 擦除块(32K) */
#define CMD_BLOCK_ERASE_64K (0xD8) /* 擦除块(64K) */
#define CMD_CHIP_ERASE (0xC7)      /* 擦除芯片 */

#define CMD_READ_SR1 (0x05)  /* 读状态寄存器1 */
#define CMD_WRITE_SR1 (0x01) /* 写状态寄存器1 */
#define CMD_READ_SR2 (0x35)  /* 读状态寄存器2 */
#define CMD_WRITE_SR2 (0x31) /* 写状态寄存器2 */
#define CMD_READ_SR3 (0x15)  /* 读状态寄存器3 */
#define CMD_WRITE_SR3 (0x11) /* 写状态寄存器3 */

#define CMD_RELEASE_POWERDOWN (0xAB) /* 释放掉电 */
#define CMD_POWERDOWN (0xB9)         /* 掉电 */

/* W25qxx_CS config */
#define W25qxx_CS_Enable IO_WritePin(&w25qxx_cs, 0)
#define W25qxx_CS_Disable IO_WritePin(&w25qxx_cs, 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
IO_TypeDef w25qxx_cs = {W25qxx_CS_GPIO_Port, W25qxx_CS_Pin};

#if W25QXX_MODE_NON_BLOCKING
enum
{
    BLOCKING,     //阻塞状态
    NON_BLOCKING, //非阻塞状态
};

volatile uint32_t W25qxxState = BLOCKING;
#endif

#if W25QXX_MODE_NON_BLOCKING
extern osSemaphoreId W25qxxBinarySemHandle;
#endif

/* Private function prototypes -----------------------------------------------*/
void W25qxx_Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
/**
 * @brief         SPI读写
 * @param  pData  数据存储区首地址
 * @param  Size   数据存储区长度
 */
void W25qxx_SPI_SendRecv(uint8_t *pData, uint16_t Size)
{
    if (HAL_SPI_TransmitReceive_DMA(&W25qxx_hspi, pData, pData, Size) != HAL_OK)
    {
        W25qxx_Error_Handler();
    }
}

#if W25QXX_MODE_NON_BLOCKING
/**
 * @brief  SPI回调函数
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (W25qxxState == NON_BLOCKING) //w25qxx运行在非阻塞状态时需要发送信号量
    {
        W25qxxState = BLOCKING;                    //w25qxx模式改为阻塞状态
        osSemaphoreRelease(W25qxxBinarySemHandle); //发送信号量
    }
}
#endif

/**
 * @brief  等待主机空闲
 */
void W25qxx_WaitMasterIdle(void)
{
    while (HAL_SPI_GetState(&W25qxx_hspi) != HAL_SPI_STATE_READY)
    {
    }
}

/**
 * @brief  等待从机空闲
 */
void W25qxx_WaitSlaveIdle(void)
{
    while ((W25qxx_ReadSR1() & (0x01))) //等待BUSY位清零
    {
    }
}

#if W25QXX_MODE_NON_BLOCKING
/**
 * @brief       等待从机一段时间
 * @param  nms  等待n个ms
 */
void W25qxx_WaitSlave(uint16_t nms)
{
    while ((W25qxx_ReadSR1() & (0x01))) //等待BUSY位清零
    {
        osDelay(nms); //交出CPU使用权n个ms
    }
}
#endif

/**
 * @brief  错误处理
 */
void W25qxx_Error_Handler(void)
{
    while (1)
    {
    }
}

/* Public functions ----------------------------------------------------------*/
/**
 * @brief  写使能，将WEL置位
 */
void W25qxx_WriteEnable(void)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[1] = {CMD_WRITE_ENABLE}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 1); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件
}

/**
 * @brief  写失能，将WEL清零
 */
void W25qxx_WriteDisable(void)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[1] = {CMD_WRITE_DISABLE}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 1); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件
}

/**
 * @brief   读状态寄存器1
 * @return  状态寄存器1读取值
 */
uint8_t W25qxx_ReadSR1(void)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲

    uint8_t buffer[2] = {CMD_READ_SR1}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 2); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

    uint8_t sr = buffer[1];

    return sr;
}

/**
 * @brief      写状态寄存器1
 * @param  sr  状态寄存器1写入值
 */
void W25qxx_WriteSR1(uint8_t sr)
{
    W25qxx_WriteEnable();    //写使能
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[2] = {CMD_WRITE_SR1}; //操作指令
    buffer[1] = sr;

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 2); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

#if W25QXX_MODE_NON_BLOCKING
    W25qxx_WaitSlave(10); //每隔10ms（典型值）检测从机是否空闲，期间交出CPU使用权
#else
    W25qxx_WaitSlaveIdle(); //等待从机空闲
#endif
}

/**
 * @brief           从Addr地址开始连续读取Flash的数据
 * @param  pBuffer  数据存储区首地址
 * @param  Addr     要读取Flash的首地址地址
 * @param  nByte    要读取的字节数
 */
void W25qxx_ReadData(uint8_t *pBuffer, uint32_t Addr, uint16_t nByte)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[4] = {CMD_READ_DATA}; //操作指令
    buffer[1] = (uint8_t)(Addr >> 16);   //发送24bit地址
    buffer[2] = (uint8_t)(Addr >> 8);
    buffer[3] = (uint8_t)(Addr);

    W25qxx_CS_Enable;                    //使能器件
    W25qxx_SPI_SendRecv(buffer, 4);      //SPI读写
    W25qxx_WaitMasterIdle();             //等待主机空闲
    W25qxx_SPI_SendRecv(pBuffer, nByte); //SPI读写

#if W25QXX_MODE_NON_BLOCKING
    W25qxxState = NON_BLOCKING;                            //w25qxx模式改为非阻塞状态
    osSemaphoreWait(W25qxxBinarySemHandle, osWaitForever); //等待信号量
#endif

    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_CS_Disable;       //失能器件
}

/**
 * @brief           从Addr地址开始连续写入Flash的少于1页（256byte）的数据
 * @param  pBuffer  数据存储区首地址
 * @param  Addr     要写入Flash的首地址地址
 * @param  nByte    要写入的字节数（最大1页）
 */
void W25qxx_PageProgram(uint8_t *pBuffer, uint32_t Addr, uint16_t nByte)
{
    W25qxx_WriteEnable();    //写使能
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[4] = {CMD_PAGE_PROGRAM}; //操作指令
    buffer[1] = (uint8_t)(Addr >> 16);      //发送24bit地址
    buffer[2] = (uint8_t)(Addr >> 8);
    buffer[3] = (uint8_t)(Addr);

    W25qxx_CS_Enable;                    //使能器件
    W25qxx_SPI_SendRecv(buffer, 4);      //SPI读写
    W25qxx_WaitMasterIdle();             //等待主机空闲
    W25qxx_SPI_SendRecv(pBuffer, nByte); //SPI读写

#if W25QXX_MODE_NON_BLOCKING
    W25qxxState = NON_BLOCKING;                            //w25qxx模式改为非阻塞状态
    osSemaphoreWait(W25qxxBinarySemHandle, osWaitForever); //等待信号量
#endif

    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_CS_Disable;       //失能器件
}

/**
 * @brief           无检验写Flash
 *                  必须确保所写的地址范围内的数据全部为0xff，否则在非0xff处写入的数据将失败！
 *                  具有自动换页功能
 * @param  pBuffer  数据存储区首地址
 * @param  Addr     要写入Flash的首地址地址
 * @param  nByte    要写入的字节数
 */
void W25qxx_WriteNoCheck(uint8_t *pBuffer, uint32_t Addr, uint16_t nByte)
{
    uint16_t PageRemain = PAGE_SIZE - Addr % PAGE_SIZE; //单页剩余可写的字节数

    if (nByte <= PageRemain)
    {
        PageRemain = nByte; //写入数据大小<剩余空间大小（即剩余空间够保存这些数据）
    }

    while (1)
    {
        W25qxx_PageProgram(pBuffer, Addr, PageRemain);

        if (nByte == PageRemain) //写入结束
        {
            break;
        }
        else //写入未结束
        {
            pBuffer += PageRemain; //下一页写入数据
            Addr += PageRemain;    //下一页写入数据地址
            nByte -= PageRemain;   //待写入字节数递减

            if (nByte > PAGE_SIZE)
            {
                PageRemain = PAGE_SIZE; //待写入1页（256字节）的数据
            }
            else
            {
                PageRemain = nByte; //待写入少于1页（256字节）的数据
            }
        }
    }
}

/**
 * @brief           从Addr地址开始连续写入Flash的数据
 * @param  pBuffer  数据存储区首地址
 * @param  Addr     要写入Flash的首地址地址
 * @param  nByte    要写入的字节数
 */
void W25qxx_WriteData(uint8_t *pBuffer, uint32_t Addr, uint16_t nByte)
{
    static uint8_t SectorBuf[SECTOR_SIZE]; //扇区buf
    uint16_t SecPos;                       //扇区位置
    uint16_t SecOff;                       //扇区偏移
    uint16_t SecRemain;                    //剩余扇区
    uint16_t i;

    SecPos = Addr / SECTOR_SIZE;      //地址所在扇区
    SecOff = Addr % SECTOR_SIZE;      //地址所在扇区的偏移
    SecRemain = SECTOR_SIZE - SecOff; //地址所在扇区剩余字节数（扇区大小4KB）

    if (nByte <= SecRemain)
    {
        SecRemain = nByte; //写入数据大小<剩余空间大小（即剩余空间够保存这些数据）
    }

    while (1)
    {
        W25qxx_ReadData(SectorBuf, SecPos * SECTOR_SIZE, SECTOR_SIZE); //读出整个扇区的内容

        for (i = 0; i < SecRemain; i++) //校验数据，是否需要擦除
        {
            if (SectorBuf[SecOff + i] != 0xff) //存储数据不为0xff，则需要擦除
            {
                break;
            }
        }

        if (i < SecRemain) //需要擦除
        {
            W25qxx_EraseSector4K(SecPos); //擦除该扇区

            for (i = 0; i < SecRemain; i++) //保存写入的数据(第1次时，是写入那扇区后面剩余的空间)
            {
                SectorBuf[SecOff + i] = pBuffer[i];
            }

            W25qxx_WriteNoCheck(SectorBuf, SecPos * SECTOR_SIZE, SECTOR_SIZE); //写入整个扇区（扇区 = 老数据 + 新写入数据）
        }
        else
        {
            W25qxx_WriteNoCheck(pBuffer, Addr, SecRemain); //不需要擦除，直接写入扇区剩余空间
        }

        if (nByte == SecRemain) //写入结束
        {
            break;
        }
        else //写入未结束
        {
            SecPos++;             //扇区地址增1
            SecOff = 0;           //偏移位置归零
            pBuffer += SecRemain; //指针偏移
            Addr += SecRemain;    //写地址偏移
            nByte -= SecRemain;   //待写入字节数递减

            if (nByte > SECTOR_SIZE)
            {
                SecRemain = SECTOR_SIZE; //待写入1扇区（4096字节）的数据
            }
            else
            {
                SecRemain = nByte; //待写入少于1扇区（4096字节）的数据
            }
        }
    }
}

/**
 * @brief              擦除扇区（4K）
 * @param  SectorAddr  扇区地址
 */
void W25qxx_EraseSector4K(uint32_t SectorAddr)
{
    SectorAddr *= SECTOR_SIZE; //扇区首地址

    W25qxx_WriteEnable();    //写使能
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[4] = {CMD_SECTOR_ERASE_4K}; //操作指令
    buffer[1] = (uint8_t)(SectorAddr >> 16);   //发送24bit地址
    buffer[2] = (uint8_t)(SectorAddr >> 8);
    buffer[3] = (uint8_t)(SectorAddr);

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 4); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

#if W25QXX_MODE_NON_BLOCKING
    W25qxx_WaitSlave(45); //每隔45ms（典型值）检测从机是否空闲，期间交出CPU使用权
#else
    W25qxx_WaitSlaveIdle(); //等待从机空闲
#endif
}

/**
 * @brief             擦除块（32K）
 * @param  BlockAddr  块地址
 */
void W25qxx_EraseBlock32K(uint32_t BlockAddr)
{
    BlockAddr *= BLOCK_SIZE_32K; //块首地址

    W25qxx_WriteEnable();    //写使能
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[4] = {CMD_BLOCK_ERASE_32K}; //操作指令
    buffer[1] = (uint8_t)(BlockAddr >> 16);    //发送24bit地址
    buffer[2] = (uint8_t)(BlockAddr >> 8);
    buffer[3] = (uint8_t)(BlockAddr);

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 4); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

#if W25QXX_MODE_NON_BLOCKING
    W25qxx_WaitSlave(120); //每隔120ms（典型值）检测从机是否空闲，期间交出CPU使用权
#else
    W25qxx_WaitSlaveIdle(); //等待从机空闲
#endif
}

/**
 * @brief             擦除块（64K）
 * @param  BlockAddr  块地址
 */
void W25qxx_EraseBlock64K(uint32_t BlockAddr)
{
    BlockAddr *= BLOCK_SIZE_64K; //块首地址

    W25qxx_WriteEnable();    //写使能
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[4] = {CMD_BLOCK_ERASE_64K}; //操作指令
    buffer[1] = (uint8_t)(BlockAddr >> 16);    //发送24bit地址
    buffer[2] = (uint8_t)(BlockAddr >> 8);
    buffer[3] = (uint8_t)(BlockAddr);

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 4); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

#if W25QXX_MODE_NON_BLOCKING
    W25qxx_WaitSlave(150); //每隔150ms（典型值）检测从机是否空闲，期间交出CPU使用权
#else
    W25qxx_WaitSlaveIdle(); //等待从机空闲
#endif
}

/**
 * @brief  擦除芯片
 */
void W25qxx_EraseChip(void)
{
    W25qxx_WriteEnable();    //写使能
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[1] = {CMD_CHIP_ERASE}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 1); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

#if W25QXX_MODE_NON_BLOCKING
    W25qxx_WaitSlave(5000); //每隔5000ms（典型值）检测从机是否空闲，期间交出CPU使用权
#else
    W25qxx_WaitSlaveIdle(); //等待从机空闲
#endif
}

/**
 * @brief  掉电
 */
void W25qxx_PowerDown(void)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[1] = {CMD_POWERDOWN}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 1); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件
}

/**
 * @brief  释放掉电
 */
void W25qxx_ReleasePowerDown(void)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲

    uint8_t buffer[1] = {CMD_RELEASE_POWERDOWN}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 1); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件
}

/**
 * @brief   读取芯片设备ID
 * @return  ID --- 8位ID号
 */
uint8_t W25qxx_ReadDeviceID(void)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[5] = {CMD_DEVICE_ID}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 5); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

    uint8_t id = buffer[4];

    return id;
}

/**
 * @brief   读取芯片Manufact ID
 * @return  ID --- 16位ID号
 */
uint16_t W25qxx_ReadManufactDeviceID(void)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[6] = {CMD_MANUFACTURER_DEVICE_ID}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 6); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

    uint16_t id = (buffer[4] << 8) + (buffer[5]);

    return id;
}

/**
 * @brief   读取芯片JEDEC ID
 * @return  ID --- 24位ID号
 */
uint32_t W25qxx_ReadJedecID(void)
{
    W25qxx_WaitMasterIdle(); //等待主机空闲
    W25qxx_WaitSlaveIdle();  //等待从机空闲

    uint8_t buffer[4] = {CMD_JEDEC_ID}; //操作指令

    W25qxx_CS_Enable;               //使能器件
    W25qxx_SPI_SendRecv(buffer, 4); //SPI读写
    W25qxx_WaitMasterIdle();        //等待主机空闲
    W25qxx_CS_Disable;              //失能器件

    uint32_t id = (buffer[1] << 16) + (buffer[2] << 8) + (buffer[3]);

    return id;
}

/**
 * @brief   获取Flash容量信息
 * @return  Flash容量
 */
uint32_t W25Qxx_GetFlashSize(void)
{
    uint32_t flash_id;
    uint32_t flash_size;

    flash_id = W25qxx_ReadJedecID();

    switch (flash_id)
    {
    case (W25Q16JV_ID + (MF_ID << 16)):
    {
        flash_size = 16 * 1024 * 1024 / 8;
        break;
    }
    case (W25Q32JV_ID + (MF_ID << 16)):
    {
        flash_size = 32 * 1024 * 1024 / 8;
        break;
    }
    case (W25Q64JV_ID + (MF_ID << 16)):
    {
        flash_size = 64 * 1024 * 1024 / 8;
        break;
    }
    case (W25Q128JV_ID + (MF_ID << 16)):
    {
        flash_size = 128 * 1024 * 1024 / 8;
        break;
    }
    default:
    {
        flash_size = 0;
        break;
    }
    }

    return flash_size;
}
