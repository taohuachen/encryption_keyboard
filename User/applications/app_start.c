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
#include "app.h"

/* Variables -----------------------------------------------------------------*/
osThreadId MasterTaskHandle;
osThreadId KeyboardTaskHandle;
osThreadId DisplayTaskHandle;

osMessageQId ModeQueueHandle;
osMessageQId AddrQueueHandle;
osMessageQId KeyQueueHandle;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void MX_USB_DEVICE_Init(void);

/* Private functions ---------------------------------------------------------*/
/**
 * 硬件初始化
 */
static void BspInit(void)
{
    MX_USB_DEVICE_Init();
}

/**
 * 创建任务
 */
static void TaskCreate(void)
{
    osThreadDef(MasterTask, MasterTaskFunc, osPriorityNormal, 0, 128);
    MasterTaskHandle = osThreadCreate(osThread(MasterTask), NULL);

    osThreadDef(KeyboardTask, KeyboardTaskFunc, osPriorityNormal, 0, 128);
    KeyboardTaskHandle = osThreadCreate(osThread(KeyboardTask), NULL);

    osThreadDef(DisplayTask, DisplayTaskFunc, osPriorityNormal, 0, 128);
    DisplayTaskHandle = osThreadCreate(osThread(DisplayTask), NULL);
}

/**
 * 创建通信工具
 */
static void ComunicationCreate(void)
{
    osMessageQDef(ModeQueue, 1, uint8_t);
    ModeQueueHandle = osMessageCreate(osMessageQ(ModeQueue), NULL);

    osMessageQDef(AddrQueue, 1, uint8_t);
    AddrQueueHandle = osMessageCreate(osMessageQ(AddrQueue), NULL);

    osMessageQDef(KeyQueue, 10, uint8_t);
    KeyQueueHandle = osMessageCreate(osMessageQ(KeyQueue), NULL);
}

/* Public functions ----------------------------------------------------------*/
void StartTaskFunc(void const *argument)
{
    BspInit();            //硬件初始化
    TaskCreate();         //创建任务
    ComunicationCreate(); //创建通信工具

    for (;;)
    {
        printf("StartTask is running!\n");

        uint8_t mode = ModeRead();
        uint8_t addr = AddrRead();

        osMessagePut(ModeQueueHandle, mode, osWaitForever);
        osMessagePut(AddrQueueHandle, addr, osWaitForever);

        LED_Toggle();

        osDelay(500);
    }
}
