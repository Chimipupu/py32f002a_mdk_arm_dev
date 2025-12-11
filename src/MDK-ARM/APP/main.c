/**
 * @file main.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  PY32F002A メイン
 * @version 0.1
 * @date 2025-09-28
 *
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "common.h"
#include "app_main.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

/* Private define ------------------------------------------------------------*/
#define LPTIM_CLOCK_SRC_LSI
#define RTC_ASYNCH_PREDIV        ((uint32_t)0x7FFF)

/* Private variables ---------------------------------------------------------*/
const uint8_t g_lptim_irq_str[]  = "LPTIM IRQ!\r\n";
const uint8_t g_rtc_alarm[]      = "RTC Alarm!\r\n";
const uint8_t g_dma_src_str[]    = "PY32F002A DMA Test Str : ABCDEF";
const uint8_t g_dma_fail_str[]   = "PY32F002A DMA Fail!\r\n";
const uint8_t g_end_of_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

volatile uint8_t *p_i2c_slave_buf = NULL;
volatile uint16_t g_i2c_slave_cnt = 0;
volatile uint32_t g_i2c_slave_state = I2C_STATE_READY;

extern uint32_t SystemCoreClock;

struct time_t {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
};
struct time_t RTC_TimeStruct;
struct time_t RTC_AlarmStruct;

struct date_t {
    uint8_t month;
    uint8_t day;
    uint8_t year;
};
struct date_t RTC_DateStruct;

uint32_t g_tim_cnt = 0;
uint8_t aShowTime[50] = {0};

bool g_is_rtc_alarm = false;

// volatile uint8_t g_uart_tx_buf[UART_BUF_SIZE] = {0};
// volatile uint32_t g_idx_uart_tx_buf = 0;
volatile uint8_t g_uart_rx_buf[UART_BUF_SIZE] = {0};
volatile uint32_t g_idx_uart_rx_buf = 0;
volatile bool g_uart_rx_done_flg = false;

#define BUFFER_SIZE       32
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};
uint8_t aDST_Buffer[BUFFER_SIZE];

volatile static bool s_dma_transfer_complete_flg = false;
volatile static bool s_dma_transfer_error_flg = false;
volatile static bool s_dma_transfer_fail_flg = false;

volatile static uint32_t s_lptim_cnt = 0;
volatile static bool s_is_lptim_irq = false;

volatile static uint8_t s_hsi_freq = 0;
volatile static uint8_t s_pll_freq = 0;

/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_ConfigUsart(USART_TypeDef *USARTx);
static void APP_DmaConfig(void);
static void APP_ConfigI2cSlave(void);
static void APP_SlaveReceive_IT(uint8_t *pData, uint16_t Size);
static void APP_SlaveTransmit_IT(uint8_t *pData, uint16_t Size);
static void APP_LPTIMClockconf(void);
static void APP_ConfigLPTIMOneShot(void);
static void APP_ConfigRtc(void);
static void APP_ConfigRtcAlarm(LL_RTC_AlarmTypeDef *p_rtc_alarm_config);
static void APP_ConfigRtcDate(uint8_t ,uint8_t , uint8_t);
static void APP_ShowRtcCalendar(void);
static void APP_UpadateRtcTimeStruct(void);
static void APP_UpadateRtcDateStruct(void);

void DBG_UART_PRINTF(const char *format, ...)
{
#ifdef DEBUG_PRINTF_USE
    char buffer[256];
    va_list args;
    int len;

    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    for (int i = 0; i < len && i < sizeof(buffer); i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART1));
        LL_USART_TransmitData8(USART1, (uint8_t)buffer[i]);
    }
#endif // DEBUG_PRINTF_USE
}

static void APP_DmaConfig(void)
{
    // PY32F002AF15P6のシリコンはPY32F030のROM32KB,SRAM4KBなので、
    // ないはずのDMAがCH1~CH3の3本使える

    LL_DMA_InitTypeDef dma_initstruct = {0};

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    dma_initstruct.PeriphOrM2MSrcAddress  = (uint32_t)&g_dma_src_str;       // DMA 転送元
    dma_initstruct.MemoryOrM2MDstAddress  = (uint32_t)&aDST_Buffer;             // DMA 転送先
    dma_initstruct.Direction              = LL_DMA_DIRECTION_MEMORY_TO_MEMORY;  // DMA Mem to Mem
    dma_initstruct.Mode                   = LL_DMA_MODE_NORMAL;                 // DMA ワンショット
    dma_initstruct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_INCREMENT;            // DMA アドレスインクリメント
    dma_initstruct.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;            // DMA アドレスインクリメント
    dma_initstruct.PeriphOrM2MSrcDataSize = LL_DMA_MDATAALIGN_BYTE;             // DMA ペリ to Mem = 8bit幅転送
    dma_initstruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;             // DMA Mem to Mem = 8bit幅転送
    dma_initstruct.NbData                 = BUFFER_SIZE;                        // DMA 転送回数
    dma_initstruct.Priority               = LL_DMA_PRIORITY_HIGH;               // DMA 優先度

    // DMA CH1 初期化
    if (LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &dma_initstruct) != SUCCESS)
    {
        APP_ErrorHandler();
    }

    // DMA CH2 初期化
    // TODO

    // DMA CH3 初期化
    // TODO

    // DMA 割り込み設定
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    /* Enable DMA */
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

static void APP_ConfigI2cSlave(void)
{
    /* Enable GPIOA peripheral clock */
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    /* Enable I2C1 peripheral clock */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

    /* Configure SCL pin: Alternative function, High speed, Open-drain, Pull-up */
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure SDA pin: Alternative function, High speed, Open-drain, Pull-up */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Reset I2C */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C1);
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C1);

    /* Enable NVIC interrupt */
    NVIC_SetPriority(I2C1_IRQn, 0);
    NVIC_EnableIRQ(I2C1_IRQn);

    /* Initialize I2C */
    LL_I2C_InitTypeDef I2C_InitStruct = {0};
    I2C_InitStruct.ClockSpeed      = I2C_SPEEDCLOCK;
    I2C_InitStruct.DutyCycle       = LL_I2C_DUTYCYCLE_16_9;
    I2C_InitStruct.OwnAddress1     = I2C_ADDRESS;
    I2C_InitStruct.TypeAcknowledge = LL_I2C_NACK;
    LL_I2C_Init(I2C1, &I2C_InitStruct);

    /* Enable clock stretching */
    /* Reset value is clock stretching enabled */
    /* LL_I2C_EnableClockStretching(I2C1); */

    /* Enable general call */
    /* Reset value is general call disabled */
    /* LL_I2C_EnableGeneralCall(I2C1); */
}

static void APP_SlaveReceive_IT(uint8_t *pData, uint16_t Size)
{
    /* Clear POS bit */
    LL_I2C_DisableBitPOS(I2C1);

    /* Assign the data to be sent and the data size to global variables */
    p_i2c_slave_buf = pData;
    g_i2c_slave_cnt = Size;
    g_i2c_slave_state  = I2C_STATE_BUSY_RX;

    /* Enable acknowledge */
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

    /* Enable interrupt */
    LL_I2C_EnableIT_EVT(I2C1);
    LL_I2C_EnableIT_BUF(I2C1);
    LL_I2C_EnableIT_ERR(I2C1);
}

static void APP_SlaveTransmit_IT(uint8_t *pData, uint16_t Size)
{
    /* Clear POS bit */
    LL_I2C_DisableBitPOS(I2C1);

    /* Assign the data to be sent, the data size, and the state to global variables */
    p_i2c_slave_buf = pData;
    g_i2c_slave_cnt = Size;
    g_i2c_slave_state = I2C_STATE_BUSY_TX;

    /* Enable acknowledge */
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

    /* Enable interrupt */
    LL_I2C_EnableIT_EVT(I2C1);
    LL_I2C_EnableIT_BUF(I2C1);
    LL_I2C_EnableIT_ERR(I2C1);
}

// DMAの転送をベリファイ
void APP_TransferCompleteCallback(void)
{
    for(uint16_t i = 0 ; i < BUFFER_SIZE; i++)
    {
        if(aDST_Buffer[i] != g_dma_src_str[i])
        {
            s_dma_transfer_fail_flg = true;
            break;
        }
    }

    s_dma_transfer_complete_flg = true;
}

void APP_TransferErrorCallback(void)
{
    s_dma_transfer_error_flg = true;
}

/**
  * @brief  System clock configuration
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
    volatile ErrorStatus pll_init_status;

#if 0
    uint32_t fact_hsi_trim_val;
    uint32_t reg;

    // [工場出荷時のHSIのトリム値を読み出し]
    fact_hsi_trim_val = REG_READ_DWORD(0x1FFF0FA0, 0) & 0x00001FFF;
    reg = READ_REG(RCC->ICSCR);
    reg = reg & ~0x00001FFF;
    reg = reg | fact_hsi_trim_val;
    WRITE_REG(RCC->ICSCR, reg);
#endif

    LL_RCC_HSI_Enable();

#if (HSI_FREQ == 24000000) || (HSI_FREQ == 48000000)
    // [内部RCクロックのHSIを8MHzから24MHzに変更]
    LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);
#endif

    while(LL_RCC_HSI_IsReady() != 1)
    {
        NOP();
    }

    // [HSI@24MHzをPLLで48MHzに逓倍]
    // ※PLLはx2の2逓倍固定
    pll_init_status = LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct);
    LL_Init1msTick(HSI_FREQ);
    LL_SetSystemCoreClock(HSI_FREQ);
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);

    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
    {
        NOP();
    }

    /* Set APB1 prescaler*/
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_Init1msTick(HSI_FREQ);

    /* Update system clock global variable SystemCoreClock (can also be updated by calling SystemCoreClockUpdate function) */
    LL_SetSystemCoreClock(HSI_FREQ);

    // 内蔵RCレゾネータのLSI(32.768KHz)を有効
    LL_RCC_LSI_Enable();
    while(LL_RCC_LSI_IsReady() == 0)
    {
        NOP();
    }
}

/**
  * @brief  Configure RTC clock
  * @param  None
  * @retval None
  */
static void APP_ConfigRtc(void)
{
    LL_RTC_InitTypeDef rtc_initstruct = {0};

    /*##-1- Enable PWR clock and enable access to the backup domain #######*/
    /* To change the source clock of the RTC functionalities (LSE, LSI), you have to:
        - Enable the PWR clock
        - Enable write access to configure the RTC clock source (once after reset).
        - Reset the Backup domain
        - Configure the needed RTC clock source */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    LL_PWR_EnableBkUpAccess();

    /*##-2- Configure LSI as RTC clock source##############################*/
    /* Enable LSI */
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1)
    {
    }
    /* Reset backup domain only if LSI has not been selected as RTC clock source */
    if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
    {
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    }

    /* Enable RTC clock and RTC APB clock */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTC);
    LL_RCC_EnableRTC();

    /*##-4 Configure RTC ######################################### ############*/
    /* Configure RTC prescaler and RTC data registers */
    if (LL_RTC_DeInit(RTC) != SUCCESS) 
    {
        /* Error prompt */
        // BSP_LED_Toggle(LED_GREEN);
    }

    /* Configure prescaler and output source */
    rtc_initstruct.AsynchPrescaler = RTC_ASYNCH_PREDIV;
    rtc_initstruct.OutPutSource    = LL_RTC_CALIB_OUTPUT_NONE;
    if (LL_RTC_Init(RTC, &rtc_initstruct) != SUCCESS)
    {
        /* Error prompt */
        // BSP_LED_Toggle(LED_GREEN);
    }
}

/**
 * @brief RTCアラーム設定関数
 * 
 * @param p_rtc_alarm_config RTCアラーム構造体ポインタ
 */
static void APP_ConfigRtcAlarm(LL_RTC_AlarmTypeDef *p_rtc_alarm_config)
{
    if (LL_RTC_ALARM_Init(RTC, LL_RTC_FORMAT_BIN, p_rtc_alarm_config) != SUCCESS)   
    {
        /* Error prompt */
        // BSP_LED_Toggle(LED_GREEN);
    }

    /* Disable write protection of RTC registers */
    LL_RTC_DisableWriteProtection(RTC);

    /* Clear alarm interrupt flag */
    LL_RTC_ClearFlag_ALR(RTC);

    /* Enable alarm interrupt */
    LL_RTC_EnableIT_ALR(RTC);

    /* Enable write protection of RTC registers */
    LL_RTC_EnableWriteProtection(RTC);

    /*##-6- Configure RTC NVIC ###############################*/
    NVIC_SetPriority(RTC_IRQn, 0x00);
    NVIC_EnableIRQ(RTC_IRQn);

    /*##-7- Exit Initialization Mode #######################################*/
    if (LL_RTC_ExitInitMode(RTC) != SUCCESS)
    {
        /* Error prompt */
        // BSP_LED_Toggle(LED_GREEN);
    }
}

/**
  * @brief  Configure Date
  * @param  fYear：year
  * @param  fMonth：month
  * @param  fDate：day
  * @retval None
  */
static void APP_ConfigRtcDate(uint8_t fDate , uint8_t fMonth , uint8_t fYear)
{
    RTC_DateStruct.day   = fDate;
    RTC_DateStruct.month = fMonth;
    RTC_DateStruct.year  = fYear;
}

/**
  * @brief  Show date and time
  * @param  None
  * @retval None
  */
static void APP_ShowRtcCalendar(void)
{
    APP_UpadateRtcTimeStruct();
    APP_UpadateRtcDateStruct();

    sprintf((char*)aShowTime,"%.2d/%.2d/%.2d %.2d:%.2d:%.2d\r\n",
                (2000 + RTC_DateStruct.year),
                RTC_DateStruct.month,
                RTC_DateStruct.day,
                RTC_TimeStruct.hour,
                RTC_TimeStruct.min,
                RTC_TimeStruct.sec
            );
}

/**
  * @brief  Update time
  * @param  None
  * @retval None
  */
static void APP_UpadateRtcTimeStruct(void)
{
    g_tim_cnt = LL_RTC_TIME_Get(RTC);
    RTC_TimeStruct.hour = (g_tim_cnt/3600);
    RTC_TimeStruct.min  = (g_tim_cnt % 3600) / 60;
    RTC_TimeStruct.sec  = (g_tim_cnt % 3600) % 60;
}

/**
  * @brief  Update date
  * @param  None
  * @retval None
  */
static void APP_UpadateRtcDateStruct(void)
{
    /* Update date when the time is 23:59:59 */
    if (g_tim_cnt == 0x0001517FU)
    {
        if(RTC_DateStruct.day == g_end_of_month[RTC_DateStruct.month -1])
        {
            RTC_DateStruct.day = 1U;
            RTC_DateStruct.month += 1U;
        }
        else
        {
            RTC_DateStruct.day = RTC_DateStruct.day + 0x1U;
        }
    }
}

/**
  * @brief  USART configuration.
  * @param  USARTx：USART module, can be USART1
  * @retval None
  */
static void APP_ConfigUsart(USART_TypeDef *USARTx)
{
    /*Enable clock, initialize pins, enable NVIC interrupt*/

    /*Enable GPIOA clock*/
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    /*Enable USART1 clock*/
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

    /*GPIOA configuration*/
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Select pin 2*/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    /* Select alternate function mode */
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    /* Select output speed */
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    /* Select push-pull output mode */
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    /*Pull-up*/
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    /* Select USART1 function */
    GPIO_InitStruct.Alternate = LL_GPIO_AF1_USART1;
    /* Initialize GPIOA */
    LL_GPIO_Init(GPIOA,&GPIO_InitStruct);

    /*Select pin 3*/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
    /* Select USART1 function */
    GPIO_InitStruct.Alternate = LL_GPIO_AF1_USART1;
    /* Initialize GPIOA */
    LL_GPIO_Init(GPIOA,&GPIO_InitStruct);

    /*Set USART1 interrupt priority*/
    NVIC_SetPriority(USART1_IRQn,0);

    /*Enable USART1 interrupt*/
    NVIC_EnableIRQ(USART1_IRQn);

    /*Configure USART*/
    LL_USART_InitTypeDef USART_InitStruct = {0};
    /*Set baud rate*/
    USART_InitStruct.BaudRate = 115200;
    /*Set data width*/
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    /* Set stop bits */
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    /* Set parity */
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    /* Initialize USART */
    LL_USART_Init(USARTx, &USART_InitStruct);

    /* Configure as full duplex asynchronous mode */
    LL_USART_ConfigAsyncMode(USARTx);

    // 割り込み有効化
    LL_USART_EnableIT_RXNE(USARTx);
    // LL_USART_EnableIT_TXE(USARTx);

    /* Configure auto baud rate detection */
    // LL_USART_SetAutoBaudRateMode(USARTx, LL_USART_AUTOBAUDRATE_ONFALLINGEDGE);
    // LL_USART_EnableAutoBaudRate(USARTx);

    /* Enable UART module */
    LL_USART_Enable(USARTx);
}

/**
  * @brief  USART interrupt callback function
  * @param  USARTx：USART module, can be USART1
  * @retval None
  */
void APP_UsartIRQCallback(USART_TypeDef *USARTx)
{
    uint8_t tmp;

    if ((LL_USART_IsActiveFlag_RXNE(USARTx) != RESET) && (LL_USART_IsEnabledIT_RXNE(USARTx) != RESET))
    {
        tmp = LL_USART_ReceiveData8(USARTx);

        if ( ((tmp >= '0') && (tmp <= '9')) || // 数字か
            ((tmp >= 'a') && (tmp <= 'z')) ||  // 小文字か
            ((tmp >= 'A') && (tmp <= 'Z')) ||  // 大文字か
            (tmp == ' ') || (tmp == '!') || (tmp == '?') || (tmp == '#')
            )
        {
            g_uart_rx_buf[g_idx_uart_rx_buf]= tmp;
            g_idx_uart_rx_buf = (g_idx_uart_rx_buf + 1) % UART_BUF_SIZE;
        }
        else if (tmp == '\r' || tmp == '\n' || tmp == '\0') {
            g_uart_rx_done_flg = true;
        } else {
            // その他の文字は無視
        }
    }
}

/**
 * @brief 低消費電力タイマーLPTIMのクロック設定関数
 * 
 */
static void APP_LPTIMClockconf(void)
{
#ifdef LPTIM_CLOCK_SRC_LSI
    // 低消費電力タイマーLPTIMのクロック源を内蔵RCレゾネータのLSI(32.768KHz)に設定
    LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSI);
#endif
}

/**
 * @brief 低電力タイマLPTIMの初期化
 * 
 */
static void APP_ConfigLPTIMOneShot(void)
{
#ifdef LPTIM_CLOCK_SRC_LSI
    LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV1);
#else
    LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV128);
#endif // LPTIM_CLOCK_SRC_LSI

    LL_LPTIM_SetUpdateMode(LPTIM1, LL_LPTIM_UPDATE_MODE_ENDOFPERIOD);
    LL_LPTIM_EnableIT_ARRM(LPTIM1);
    LL_LPTIM_Enable(LPTIM1);

#ifdef LPTIM_CLOCK_SRC_LSI
    // (LSI 32.768KHz/1分周)/32768 = 1Hz = 1000ms
    LL_LPTIM_SetAutoReload(LPTIM1, 32768);
#else
    // (48MHz/128分周)/375000 = 1Hz = 1000ms
    LL_LPTIM_SetAutoReload(LPTIM1, 375000);
#endif // LPTIM_CLOCK_SRC_LSI

    // LPTIMのIRQ割り込み
    NVIC_EnableIRQ(LPTIM1_IRQn);
    NVIC_SetPriority(LPTIM1_IRQn, 0);

    LL_LPTIM_StartCounter(LPTIM1,LL_LPTIM_OPERATING_MODE_ONESHOT);
}

/**
 * @brief 低消費電力タイマーLPTIM割り込みハンドラ コールバック関数
 * 
 */
void APP_LPTIMCallback(void)
{
    s_lptim_cnt = (s_lptim_cnt + 1) % UINT_32_MAX;

    s_is_lptim_irq = true;

    // LPTIM再起動
    APP_ConfigLPTIMOneShot();
}

/**
  * @brief  メイン関数
  * @param  None
  * @retval int
  */
int main(void)
{
    // 標準ライブラリ関連初期化
    setbuf(stdout, NULL);

    // クロック初期化
    APP_SystemClockConfig();
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1);
    s_hsi_freq = LL_RCC_HSI_GetFreq() / 1000000;
    s_pll_freq = s_hsi_freq * 2;

    // UART初期化
    APP_ConfigUsart(USART1);

#if 0
    // I2C初期化
    APP_ConfigI2cSlave();

    // LPTIM初期化
    APP_LPTIMClockconf();
    APP_ConfigLPTIMOneShot();

    // DMA初期化
    memset(aDST_Buffer, 0x00, sizeof(aDST_Buffer));
    APP_DmaConfig();

    // RTC初期化
    LL_RTC_TimeTypeDef rtc_time_config;
    APP_ConfigRtc();
    APP_ConfigRtcDate(1, 1, 26);   // 日、月、年
    rtc_time_config.Hours      = 0;
    rtc_time_config.Minutes    = 0;
    rtc_time_config.Seconds    = 0;
    if (LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &rtc_time_config) != SUCCESS) {
        // RTC初期化エラー
    }

    // RTCアラーム設定
    LL_RTC_AlarmTypeDef rtc_alarm_config;
    rtc_alarm_config.AlarmTime.Hours      = 0;
    rtc_alarm_config.AlarmTime.Minutes    = 3;
    rtc_alarm_config.AlarmTime.Seconds    = 0;
    APP_ConfigRtcAlarm(&rtc_alarm_config);
#endif

    // アプリ初期化
    app_main_init();

    DBG_UART_PRINTF("PY32F002A Develop By Chimipupu\r\n");
    DBG_UART_PRINTF("HSI = %d MHz, PLL Freq = %d MHz\r\n", s_hsi_freq, s_pll_freq);

#if 0
    // 起動からの時間(秒単位)
    DBG_UART_PRINTF("Execution Time : %d sec\r\n", s_lptim_cnt);

    // RTCの時刻表示
    APP_ShowRtcCalendar();
    DBG_UART_PRINTF("%s\r\n", aShowTime);
#endif

    while (1)
    {
#if 0
        // LPTIMチェック
        if(s_is_lptim_irq != false) {
            s_is_lptim_irq = false;
        }

        // RTCアラームチェック
        if(g_is_rtc_alarm != false) {
            g_is_rtc_alarm = false;
        }
#endif
        // アプリメイン
        app_main();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
    /* infinite loop */
    while (1)
    {
        NOP();
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
        for example: DBG_UART_PRINTF("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* infinite loop */
    while (1)
    {
        NOP();
    }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
