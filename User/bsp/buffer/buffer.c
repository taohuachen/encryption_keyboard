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
#include "buffer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
 * @brief       初始化缓冲区
 * @param pbuf  指向缓冲区的指针
 * @param pdata 指向缓冲区数据片的指针
 * @param len   缓冲区长度
 * @param full  缓冲区初始化满标志，true：满，false：空
 */
void Buffer_Init(BUFFER_TypeDef *pbuf, uint8_t *pdata, uint32_t len, bool full)
{
    pbuf->Len = len;
    pbuf->EmptyCnt = ((full == true) ? (0) : (len));
    pbuf->IxRd = 0;
    pbuf->IxWr = 0;
    pbuf->DataPtr = pdata;
}

/**
 * @brief      清空缓冲区
 * @param pbuf 指向缓冲区的指针
 */
void Buffer_Clear(BUFFER_TypeDef *pbuf)
{
    pbuf->EmptyCnt = pbuf->Len;
    pbuf->IxWr = pbuf->IxRd;
}

/**
 * @brief       确定缓冲区是否空
 * @param  pbuf 指向缓冲区的指针
 * @return      true：缓冲区为空
                false：其它
 */
bool Buffer_IsEmpty(BUFFER_TypeDef *pbuf)
{
    bool empty;

    empty = ((pbuf->EmptyCnt == pbuf->Len) ? (true) : (false));
    return (empty);
}

/**
 * @brief       确定缓冲区是否满
 * @param  pbuf 指向缓冲区的指针
 * @return      true：缓冲区为满
                false：其它
 */
bool Buffer_IsFull(BUFFER_TypeDef *pbuf)
{
    bool full;

    full = ((pbuf->EmptyCnt == 0) ? (true) : (false));
    return (full);
}

/**
 * @brief       检索缓冲区长度
 * @param  pbuf 指向缓冲区的指针
 * @return      缓冲区长度
 */
uint32_t Buffer_Size(BUFFER_TypeDef *pbuf)
{
    uint32_t size;

    size = pbuf->Len;
    return (size);
}

/**
 * @brief       检索缓冲区数据片长度
 * @param  pbuf 指向缓冲区的指针
 * @return      缓冲区数据片长度
 */
uint32_t Buffer_DataLen(BUFFER_TypeDef *pbuf)
{
    uint32_t len;

    len = pbuf->Len - pbuf->EmptyCnt;
    return (len);
}

/**
 * @brief        从缓冲区读取数据
 * @param  pbuf  指向缓冲区的指针
 * @param  pdest 指向读取数据区的指针
 * @param  len   准备读取的字节数
 * @return       实际读取的字节数
 */
uint32_t Buffer_Read(BUFFER_TypeDef *pbuf, uint8_t *pdest, uint32_t len)
{
    uint32_t buf_avail;      //缓冲区数据长度
    uint32_t buf_copy_start; //缓冲区读取（0~buf_copy_start）的数据
    uint32_t buf_copy_end;   //缓冲区读取（读指针~buf_copy_end）的数据
    uint32_t buf_copy_tot;   //缓冲区实际读取的字节数
    uint32_t ix_rd;
    uint32_t ix_rd_new;
    uint32_t buf_len;  //缓冲区长度
    uint8_t *pdest_08; //指向读取数据区的指针

    buf_len = pbuf->Len;                  //buf_len赋值缓冲区长度
    buf_avail = buf_len - pbuf->EmptyCnt; //buf_avail赋值缓冲区数据长度
    pdest_08 = (uint8_t *)pdest;          //pdest_08赋值读取数据区的指针

    if (buf_avail == 0) //缓冲区无数据，退出函数
    {
        return (0); //返回读取的字节数为0
    }

    ix_rd = pbuf->IxRd;                                    //ix_rd赋值缓冲区读指针
    buf_copy_tot = DEF_MIN(buf_avail, len);                //计算将要读取的数据量
    buf_copy_end = DEF_MIN(buf_copy_tot, buf_len - ix_rd); //计算到缓冲区end将要读取的数据量
    buf_copy_start = buf_copy_tot - buf_copy_end;          //计算从缓冲区start将要读取的数据量

    if (buf_copy_start > 0) //读取的数据越过缓冲区start情况的处理
    {
        pbuf->IxRd = buf_copy_start;

        memcpy((void *)pdest_08, (void *)&pbuf->DataPtr[ix_rd], buf_copy_end);
        pdest_08 += buf_copy_end;
        memcpy((void *)pdest_08, (void *)&pbuf->DataPtr[0], buf_copy_start);
    }
    else //读取的数据没越过缓冲区start情况的处理
    {
        ix_rd_new = ix_rd + buf_copy_tot;
        if (ix_rd_new == buf_len) //读指针移到缓冲区start位置，置0
        {
            pbuf->IxRd = 0;
        }
        else
        {
            pbuf->IxRd = ix_rd_new;
        }

        memcpy((void *)pdest_08, (void *)&pbuf->DataPtr[ix_rd], buf_copy_end);
    }

    pbuf->EmptyCnt += buf_copy_tot; //增加缓冲区空余长度

    return (buf_copy_tot); //返回实际读取的字节数
}

/**
 * @brief        往缓冲区写入数据
 * @param  pbuf  指向缓冲区的指针
 * @param  pdest 指向写入数据区的指针
 * @param  len   准备写入的字节数
 * @return       实际写入的字节数
 */
uint32_t Buffer_Write(BUFFER_TypeDef *pbuf, uint8_t *pdest, uint32_t len)
{
    uint32_t buf_empty_cnt;  //缓冲区空余长度
    uint32_t buf_copy_start; //缓冲区写入（0~buf_copy_start）的数据
    uint32_t buf_copy_end;   //缓冲区写入（写指针~buf_copy_end）的数据
    uint32_t ix_wr;
    uint32_t ix_wr_new;
    uint32_t buf_len;  //缓冲区长度
    uint8_t *pdest_08; //指向写入数据区的指针

    buf_len = pbuf->Len;            //buf_len赋值缓冲区长度
    buf_empty_cnt = pbuf->EmptyCnt; //buf_empty_cnt赋值缓冲区空余长度
    pdest_08 = (uint8_t *)pdest;    //pdest_08赋值写入数据区的指针

    if (buf_empty_cnt < len) //缓冲区空余长度不够，退出函数
    {
        return (0); //返回写入的字节数为0
    }

    ix_wr = pbuf->IxWr;                           //ix_wr赋值缓冲区写指针
    buf_copy_end = DEF_MIN(len, buf_len - ix_wr); //计算到缓冲区end将要写入的数据量
    buf_copy_start = len - buf_copy_end;          //计算从缓冲区start将要写入的数据量

    if (buf_copy_start > 0) //写入的数据越过缓冲区start情况的处理
    {
        pbuf->IxWr = buf_copy_start;

        memcpy((void *)&pbuf->DataPtr[ix_wr], (void *)pdest_08, buf_copy_end);
        pdest_08 += buf_copy_end;
        memcpy((void *)&pbuf->DataPtr[0], (void *)pdest_08, buf_copy_start);
    }
    else //写入的数据没越过缓冲区start情况的处理
    {
        ix_wr_new = ix_wr + len;
        if (ix_wr_new == buf_len) //写指针移到缓冲区start位置，置0
        {
            pbuf->IxWr = 0;
        }
        else
        {
            pbuf->IxWr = ix_wr_new;
        }

        memcpy((void *)&pbuf->DataPtr[ix_wr], (void *)pdest_08, buf_copy_end);
    }

    pbuf->EmptyCnt -= len; //减少缓冲区空余长度

    return (len); //返回实际写入的字节数
}

/**
 * @brief         从缓冲区读取1字节数据
 * @param  pbuf   指向缓冲区的指针
 * @param  pdatum 指向读取变量的指针
 * @return        true：成功读取1字节数据
                  false：其它
 */
bool Buffer_ReadOctet(BUFFER_TypeDef *pbuf, uint8_t *pdatum)
{
    uint32_t len;       //缓冲区长度
    uint32_t empty_cnt; //缓冲区空余长度
    uint32_t ix_rd;     //读指针

    len = pbuf->Len;            //len赋值缓冲区长度
    empty_cnt = pbuf->EmptyCnt; //empty_cnt赋值缓冲区空余长度

    if (empty_cnt == len) //缓冲区已空，无法读取数据，退出函数
    {
        return (false); //返回false
    }

    ix_rd = pbuf->IxRd;             //ix_rd赋值读指针
    *pdatum = pbuf->DataPtr[ix_rd]; //读取1字节数据
    pbuf->EmptyCnt = empty_cnt + 1; //缓冲区空余长度+1

    if (ix_rd + 1 == len) //读指针移到缓冲区start位置，置0
    {
        pbuf->IxRd = 0;
    }
    else
    {
        pbuf->IxRd = ix_rd + 1;
    }

    return (true);
}

/**
 * @brief        往缓冲区写入1字节数据
 * @param  pbuf  指向缓冲区的指针
 * @param  datum 写入变量
 * @return       true：成功写入1字节数据
                 false：其它
 */
bool Buffer_WriteOctet(BUFFER_TypeDef *pbuf, uint8_t datum)
{
    uint32_t len;       //缓冲区长度
    uint32_t empty_cnt; //缓冲区空余长度
    uint32_t ix_wr;     //写指针

    empty_cnt = pbuf->EmptyCnt; //empty_cnt赋值缓冲区空余长度

    if (empty_cnt == 0) //缓冲区已满，无法写入数据，退出函数
    {
        return (false); //返回false
    }

    len = pbuf->Len;                //len赋值缓冲区长度
    ix_wr = pbuf->IxWr;             //ix_wr赋值写指针
    pbuf->DataPtr[ix_wr] = datum;   //写入1字节数据
    pbuf->EmptyCnt = empty_cnt - 1; //缓冲区空余长度-1

    if (ix_wr + 1 == len) //写指针移到缓冲区start位置，置0
    {
        pbuf->IxWr = 0;
    }
    else
    {
        pbuf->IxWr = ix_wr + 1;
    }

    return (true);
}
