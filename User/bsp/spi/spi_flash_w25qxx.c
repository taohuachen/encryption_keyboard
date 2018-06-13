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
 *    因芯片电气特性差异(如:电压、速度等), 可能部分芯片不支持
 *
 * 2、W25QXX信息：
 * 页(Page) ------ 256Byte
 * 扇区(Sector) -- 4KByte ------ (等于16页)
 * 块(Block) ----- 64KByte ----- (等于16扇区)
 * 
 * 3、读写操作：
 * 读 ------------ 一次最大读一页(256B)
 * 写 ------------ 页
 * 擦除 ---------- 扇区、块、整个芯片
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "spi_flash_w25qxx.h"
#define W25QXX_SPI hspi2
#define W25QXX_CS_ENABLE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define W25QXX_CS_DISABLE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define W25QXX_Init() MX_SPI2_Init()
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    SPI_HandleTypeDef hspi;
    IO_TypeDef cs;
    void (*Init)(void);
};

/* Private define ------------------------------------------------------------*/
#define PAGE_SIZE (256)
#define SECTOR_SIZE (4 * 1024)
#define BLOCK_SIZE (64 * 1024)

/* JEDEC Manufacturer ID */
#define MF_ID (0xEF)

/* JEDEC Device ID */
#define W25Q16JV_ID (0x4015)
#define W25Q32JV_ID (0x4016)
#define W25Q64JV_ID (0x4017)
#define W25Q128JV_ID (0x4018)

/* Command List */
#define CMD_WRITE_ENABLE (0x06)       /* 写使能 */
#define CMD_WRITE_DISABLE (0x04)      /* 写失能 */
#define CMD_READ_STATUS_REG (0x05)    /* 读状态寄存器 */
#define CMD_WRITE_STATUS_REG (0x01)   /* 写状态寄存器 */
#define CMD_READ_DATA (0x03)          /* 读数据 */
#define CMD_FAST_READ (0x0B)          /* 快读数据 */
#define CMD_FAST_READ_DUAL (0x3B)     /* 快读数据(双数据线输出) */
#define CMD_PAGE_PROGRAM (0x02)       /* 页编程 */
#define CMD_SECTOR_ERASE_4K (0x20)    /* 擦除扇区(4K) */
#define CMD_BLOCK_ERASE_32K (0x52)    /* 擦除块(32K) */
#define CMD_BLOCK_ERASE_64K (0xD8)    /* 擦除块(64K) */
#define CMD_CHIP_ERASE (0xC7)         /* 擦除芯片 */
#define CMD_POWER_DOWN (0xB9)         /* 掉电 */
#define CMD_RELEASE_POWER_DOWN (0xAB) /* 释放掉电 */
#define CMD_DEVICE_ID (0xAB)          /* 设备ID */
#define CMD_MANUFACT_DEVICE_ID (0x90) /* Manufact ID */
#define CMD_JEDEC_ID (0x9F)           /* JEDEC ID */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * SPI读写字节
 * @param  send_data 发送字节
 * @return           接收字节
 */
static uint8_t SPI_WriteReadByte(uint8_t send_data)
{
    uint8_t recv_data;

    if (HAL_SPI_TransmitReceive_IT(&w25qxx_hspi, &send_data, &recv_data, 1) != HAL_OK)
    {
        Error_Handler();
    }
    while (HAL_SPI_GetState(&w25qxx_hspi) != HAL_SPI_STATE_READY)
    {
    }

    return recv_data;
}

/**
 * 错误处理
 */
static void Error_Handler(void)
{
    while (1)
    {
    }
}

/**
 * SPI_FLASH写使能，将WEL置位
 */
static void W25QXX_WriteEnable(void)
{
    W25QXX_CS_ENABLE;                    //使能器件
    SPI_WriteReadByte(CMD_WRITE_ENABLE); //(写使能)指令
    W25QXX_CS_DISABLE;                   //失能器件
}

/**
 * SPI_FLASH写禁止,将WEL清零
 */
static void W25QXX_WriteDisable(void)
{
    W25QXX_CS_ENABLE;                     //使能器件
    SPI_WriteReadByte(CMD_WRITE_DISABLE); //(写失能)指令
    W25QXX_CS_DISABLE;                    //失能器件
}

/**
 * 等待不忙
 */
static void W25QXX_WaitForNoBusy(void)
{
    while ((W25QXX_ReadSR() & 0x01) == 0x01) //等待BUSY位清空
    {
    }
}

/* Public functions ----------------------------------------------------------*/
/**
 * 初始化SFLASH
 */
void W25QXX_Init(void)
{
    MX_SPI1_Init();
}

/**
 * 寄存器命令说明
 * BIT7 6   5   4   3   2   1   0
 * SPR  RV  TB  BP2 BP1 BP0 WEL BUSY
 * SPR:默认0,状态寄存器保护位,配合WP使用
 * TB,BP2,BP1,BP0:FLASH区域写保护设置
 * WEL:写使能锁定
 * BUSY:忙标记位(1,忙;0,空闲)
 * 默认:0x00
 */

/**
 * 读SFLASH状态寄存器
 * @return  读取字节
 */
uint8_t W25QXX_ReadSR(void)
{
    uint8_t data;

    W25QXX_CS_ENABLE;                       //使能器件
    SPI_WriteReadByte(CMD_READ_STATUS_REG); //(读状态寄存器)指令
    data = SPI_WriteReadByte(0xFF);         //读取一个字节
    W25QXX_CS_DISABLE;                      //失能器件

    return data;
}

/**
 * 写SFLASH状态寄存器
 * @param SR 写状态寄存器命令
 */
void W25QXX_WriteSR(uint8_t sr)
{
    W25QXX_WriteEnable(); //写使能
    W25QXX_WaitForNoBusy();
    W25QXX_CS_ENABLE;                        //使能器件
    SPI_WriteReadByte(CMD_WRITE_STATUS_REG); //(写状态寄存器)指令
    SPI_WriteReadByte(sr);                   //写入一个字节
    W25QXX_CS_DISABLE;                       //失能器件
    W25QXX_WaitForNoBusy();
}

/**
 * 从ReadAddr地址开始连续读取SFLASH的nByte
 * @param pBuffer  数据存储区首地址
 * @param ReadAddr 要读取SFLASH Flash的首地址地址
 * @param nByte    要读取的字节数(最大65535B = 64K 块)
 */
void W25QXX_ReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
    W25QXX_CS_ENABLE;                               //使能器件
    SPI_WriteReadByte(CMD_READ_DATA);               //(读数据)指令
    SPI_WriteReadByte((uint8_t)((ReadAddr) >> 16)); //发送24bit地址
    SPI_WriteReadByte((uint8_t)((ReadAddr) >> 8));
    SPI_WriteReadByte((uint8_t)ReadAddr);

    while (nByte--) //循环读数
    {
        *pBuffer = SPI_WriteReadByte(0xFF);
        pBuffer++;
    }

    W25QXX_CS_DISABLE; //失能器件
}

/**
 * 从ReadAddr地址开始连续快速读取SFLASH的nByte
 * @param pBuffer  数据存储区首地址
 * @param ReadAddr 要读取SFLASH Flash的首地址地址
 * @param nByte    要读取的字节数(最大65535B = 64K 块)
 */
void W25QXX_FastReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
    W25QXX_CS_ENABLE;                               //使能器件
    SPI_WriteReadByte(CMD_FAST_READ);               //(快读数据)指令
    SPI_WriteReadByte((uint8_t)((ReadAddr) >> 16)); //发送24bit地址
    SPI_WriteReadByte((uint8_t)((ReadAddr) >> 8));
    SPI_WriteReadByte((uint8_t)ReadAddr);
    SPI_WriteReadByte(0xFF); //等待8个时钟

    while (nByte--) //循环读数
    {
        *pBuffer = SPI_WriteReadByte(0xFF);
        pBuffer++;
    }

    W25QXX_CS_DISABLE; //失能器件
}

/**
 * 在SFLASH内写入少于1页(256个字节)的数据
 * @param pBuffer   写入数据区首地址
 * @param WriteAddr 要写入Flash的地址
 * @param nByte     要写入的字节数(最大1页)
 */
void W25QXX_WritePage(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    W25QXX_WriteEnable(); //写使能
    W25QXX_WaitForNoBusy();
    W25QXX_CS_ENABLE;                                //使能器件
    SPI_WriteReadByte(CMD_PAGE_PROGRAM);             //(页编程)指令
    SPI_WriteReadByte((uint8_t)((WriteAddr) >> 16)); //发送24bit地址
    SPI_WriteReadByte((uint8_t)((WriteAddr) >> 8));
    SPI_WriteReadByte((uint8_t)WriteAddr);

    while (nByte--)
    {
        SPI_WriteReadByte(*pBuffer);
        pBuffer++;
    }

    W25QXX_CS_DISABLE;
    W25QXX_WaitForNoBusy(); //等待空闲（等待写入结束）
}

/**
 * 无检验写SFLASH
   必须确保所写的地址范围内的数据全部为0xFF,否则在非0xFF处写入的数据将失败!
   具有自动换页功能
 * @param pBuffer   写入数据区首地址
 * @param WriteAddr 要写入Flash的地址
 * @param nByte     要写入的字节数
 */
void W25QXX_WriteNoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    uint16_t PageRemain = 256 - WriteAddr % 256; //单页剩余可写的字节数

    if (nByte <= PageRemain)
    {
        PageRemain = nByte; //不大于256个字节
    }

    while (1)
    {
        W25QXX_WritePage(pBuffer, WriteAddr, PageRemain);
        if (nByte == PageRemain) //写入结束
        {
            break;
        }
        else //写入未结束
        {
            pBuffer += PageRemain;   //下一页写入数据
            WriteAddr += PageRemain; //下一页写入数据地址
            nByte -= PageRemain;     //待写入字节数递减

            if (nByte > 256)
            {
                PageRemain = 256; //待写入1页(256字节)的数据
            }
            else
            {
                PageRemain = nByte; //待写入少于1页(256字节)的数据
            }
        }
    }
}

/**
 * 从ReadAddr地址开始连续写入nByte到SFLASH中
 * @param pBuffer   写入数据区首地址
 * @param WriteAddr 要写入Flash的地址
 * @param nByte     要写入的字节数(最大65535B = 64K 块)
 */
void W25QXX_WriteNByte(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    static uint8_t SectorBuf[4096]; //扇区buf
    uint32_t SecPos;                //扇区位置
    uint16_t SecOff;                //扇区偏移
    uint16_t SecRemain;             //剩余扇区
    uint16_t i;

    SecPos = WriteAddr / 4096; //地址所在扇区(0~511)
    SecOff = WriteAddr % 4096; //地址所在扇区的偏移
    SecRemain = 4096 - SecOff; //地址所在扇区剩余字节数(扇区大小4096B=4KB)

    if (nByte <= SecRemain)
    {
        SecRemain = nByte; //写入数据大小 < 剩余空间大小 (即剩余空间够保存这些数据)
    }

    while (1)
    {
        W25QXX_ReadNByte(SectorBuf, SecPos * 4096, 4096); //读出整个扇区的内容
        for (i = 0; i < SecRemain; i++)                   //校验数据,是否需要擦除
        {
            if (SectorBuf[SecOff + i] != 0xFF) //存储数据不为0xFF 则需要擦除
            {
                break;
            }
        }
        if (i < SecRemain) //需要擦除
        {
            W25QXX_EraseSector4K(SecPos);   //擦除该扇区
            for (i = 0; i < SecRemain; i++) //保存写入的数据(第1次时，是写入那扇区后面剩余的空间)
            {
                SectorBuf[SecOff + i] = pBuffer[i];
            }
            W25QXX_WriteNoCheck(SectorBuf, SecPos * 4096, 4096); //写入整个扇区（扇区 = 老数据 + 新写入数据）
        }
        else
        {
            W25QXX_WriteNoCheck(pBuffer, WriteAddr, SecRemain); //不需要擦除,直接写入扇区剩余空间
        }

        if (nByte == SecRemain) //写入结束
        {
            W25QXX_WriteDisable(); //写失能, 退出写
            break;
        }
        else //写入未结束
        {
            SecPos++;               //扇区地址增1
            SecOff = 0;             //偏移位置归零
            pBuffer += SecRemain;   //指针偏移
            WriteAddr += SecRemain; //写地址偏移
            nByte -= SecRemain;     //待写入字节数递减
            if (nByte > 4096)
            {
                SecRemain = 4096; //待写入1扇区(4096字节)的数据
            }
            else
            {
                SecRemain = nByte; //待写入少于1扇区(4096字节)的数据
            }
        }
    }
}

/**
 * 擦除扇区
 * @param SectorAddr 扇区地址
 */
void W25QXX_EraseSector4K(uint32_t SectorAddr)
{
    SectorAddr *= (4 * 1024); //扇区首地址
    W25QXX_WriteEnable();     //写使能
    W25QXX_WaitForNoBusy();
    W25QXX_CS_ENABLE;                                 //使能器件
    SPI_WriteReadByte(CMD_SECTOR_ERASE_4K);           //(擦除扇区4K)指令
    SPI_WriteReadByte((uint8_t)((SectorAddr) >> 16)); //擦除地址
    SPI_WriteReadByte((uint8_t)((SectorAddr) >> 8));
    SPI_WriteReadByte((uint8_t)SectorAddr);
    W25QXX_CS_DISABLE;
    W25QXX_WaitForNoBusy(); //等待擦除完成
}

/**
 * 擦除块,擦除块需要一定时间
 * @param BlockAddr 块地址
 */
void W25QXX_EraseBlock32K(uint32_t BlockAddr)
{
    BlockAddr *= (32 * 1024); //块首地址
    W25QXX_WriteEnable();     //写使能
    W25QXX_WaitForNoBusy();
    W25QXX_CS_ENABLE;                                //使能器件
    SPI_WriteReadByte(CMD_BLOCK_ERASE_32K);          //(擦除块32K)指令
    SPI_WriteReadByte((uint8_t)((BlockAddr) >> 16)); //擦除地址
    SPI_WriteReadByte((uint8_t)((BlockAddr) >> 8));
    SPI_WriteReadByte((uint8_t)BlockAddr);
    W25QXX_CS_DISABLE;
    W25QXX_WaitForNoBusy(); //等待擦除完成
}

/**
 * 擦除块,擦除块需要一定时间
 * @param BlockAddr 块地址
 */
void W25QXX_EraseBlock64K(uint32_t BlockAddr)
{
    BlockAddr *= (64 * 1024); //块首地址
    W25QXX_WriteEnable();     //写使能
    W25QXX_WaitForNoBusy();
    W25QXX_CS_ENABLE;                                //使能器件
    SPI_WriteReadByte(CMD_BLOCK_ERASE_64K);          //(擦除块64K)指令
    SPI_WriteReadByte((uint8_t)((BlockAddr) >> 16)); //擦除地址
    SPI_WriteReadByte((uint8_t)((BlockAddr) >> 8));
    SPI_WriteReadByte((uint8_t)BlockAddr);
    W25QXX_CS_DISABLE;
    W25QXX_WaitForNoBusy(); //等待擦除完成
}

/**
 * 擦除整个芯片(整片擦除时间较长)
 */
void W25QXX_EraseChip(void)
{
    W25QXX_WriteEnable(); //写使能
    W25QXX_WaitForNoBusy();
    W25QXX_CS_ENABLE;                  //使能器件
    SPI_WriteReadByte(CMD_CHIP_ERASE); //(擦除芯片)指令
    W25QXX_CS_DISABLE;
    W25QXX_WaitForNoBusy(); //等待芯片擦除结束
}

/**
 * 进入掉电模式
 */
void W25QXX_PowerDown(void)
{
    W25QXX_CS_ENABLE;                  //使能器件
    SPI_WriteReadByte(CMD_POWER_DOWN); //(掉电)指令
    W25QXX_CS_DISABLE;                 //失能器件
}

/**
 * 掉电唤醒
 */
void W25QXX_WAKEUP(void)
{
    W25QXX_CS_ENABLE;                          //使能器件
    SPI_WriteReadByte(CMD_RELEASE_POWER_DOWN); //(掉电唤醒)指令
    W25QXX_CS_DISABLE;                         //失能器件
}

/**
 * 读取芯片设备ID
 * @return  ID --- 8位ID号
 */
uint8_t W25QXX_ReadDeviceID(void)
{
    uint8_t ID = 0;
    W25QXX_CS_ENABLE; //使能器件

    SPI_WriteReadByte(CMD_DEVICE_ID); //(设备ID)指令
    SPI_WriteReadByte(0x00);
    SPI_WriteReadByte(0x00);
    SPI_WriteReadByte(0x00);

    ID = SPI_WriteReadByte(0xFF); //读取ID

    W25QXX_CS_DISABLE; //失能器件

    return ID;
}

/**
 * 读取芯片Manufact ID
 * @return  ID --- 16位ID号
 */
uint16_t W25QXX_ReadManufactDeviceID(void)
{
    uint16_t ID = 0;
    W25QXX_CS_ENABLE; //使能器件

    SPI_WriteReadByte(CMD_MANUFACT_DEVICE_ID); //(Manufact ID)指令
    SPI_WriteReadByte(0x00);
    SPI_WriteReadByte(0x00);
    SPI_WriteReadByte(0x00);

    ID |= SPI_WriteReadByte(0xFF) << 8; //读取ID
    ID |= SPI_WriteReadByte(0xFF);

    W25QXX_CS_DISABLE; //失能器件

    return ID;
}

/**
 * 读取芯片JEDEC ID
 * @return  ID --- 24位ID号
 */
uint32_t W25QXX_ReadJedecID(void)
{
    uint32_t ID = 0;
    W25QXX_CS_ENABLE; //使能器件

    SPI_WriteReadByte(CMD_JEDEC_ID); //(JEDEC ID)指令

    ID |= SPI_WriteReadByte(0xFF) << 16; //读取ID
    ID |= SPI_WriteReadByte(0xFF) << 8;
    ID |= SPI_WriteReadByte(0xFF);

    W25QXX_CS_DISABLE; //失能器件

    return ID;
}
