/**
 * @file main.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  PY32F002A メイン
 * @version 0.1
 * @date 2025-08-23
 *
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 *
 * [免責]
 * 誰かがこの実装を使って何かあってもいかなる責任は負えません！
 * もしそれでも良いなら、「I_AGREE_TO_YOUR_DISCLAIMER」をdefineで定義して。
 * そうすると、PY32F002AF15P6のシリコンはPY32F030K16T6と同じだから
 * ROMは32KB, RAMは4KBになるし、HSIはPLLで48MHzに逓倍できる！PY32F030の色んな機能が使える！
 * Enjoy! Your 15円ARMマイコンPY32F002A Life by Chimipupu
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "py32f0xx.h"

#include "app_main.h"

/* Private define ------------------------------------------------------------*/
#if defined(I_AGREE_TO_YOUR_DISCLAIMER)
#warning I agree to the disclaimer, run PY32F002A as PY32F030, and multiply HSI to 48 MHz with PLL!
#endif

#define UART_TX_BUF_SIZE    32
#define UART_RX_BUF_SIZE    32

/* Private variables ---------------------------------------------------------*/
const uint8_t uart_test_str_buf[] = "PY32F002A Dev By Chimipupu\r\n";

uint8_t aTxBuffer[UART_TX_BUF_SIZE];
uint8_t aRxBuffer[UART_RX_BUF_SIZE];

uint8_t *TxBuff = NULL;
__IO uint16_t TxSize = 0;
__IO uint16_t TxCount = 0;

uint8_t *RxBuff = NULL;
__IO uint16_t RxSize = 0;
__IO uint16_t RxCount = 0;

__IO ITStatus UartReady = RESET;

extern uint32_t SystemCoreClock;
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_ConfigUsart(USART_TypeDef *USARTx);

#if defined(I_AGREE_TO_YOUR_DISCLAIMER)

#define BUFFER_SIZE              32

LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};

static void APP_DmaConfig(void);

const uint8_t aSRC_Const_Buffer[] = "PY32F002A DMA Test Str : ABCDEF";
const uint8_t dma_fail_str[] = "PY32F002A DMA Fail!\r\n";
uint8_t aDST_Buffer[BUFFER_SIZE];

volatile static bool s_dma_transfer_complete_flg = false;
volatile static bool s_dma_transfer_error_flg = false;
volatile static bool s_dma_transfer_fail_flg = false;

static void APP_DmaConfig(void)
{
    // PY32F002AF15P6のシリコンはPY32F030K16T6なので、
    // DMAはCH1~CH3の3本が使える

    LL_DMA_InitTypeDef dma_initstruct = {0};

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    dma_initstruct.PeriphOrM2MSrcAddress  = (uint32_t)&aSRC_Const_Buffer;       // DMA 転送元
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

// DMAの転送をベリファイ
void APP_TransferCompleteCallback(void)
{
    for(uint16_t i = 0 ; i < BUFFER_SIZE; i++)
    {
        if(aDST_Buffer[i] != aSRC_Const_Buffer[i])
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
#endif

/**
  * @brief  Main program.
  * @param  None
  * @retval int
  */
int main(void)
{
    /* Configure system clock */
    APP_SystemClockConfig();

    // UART初期化
    memset(aRxBuffer, 0x00, sizeof(aRxBuffer));
    memset(aTxBuffer, 0x00, sizeof(aTxBuffer));

    APP_ConfigUsart(USART1);
    // LL_USART_SendAutoBaudRateReq(USART1);
    // APP_UsartReceive_IT(USART1, (uint8_t *)aRxBuffer, 1);

#if defined(I_AGREE_TO_YOUR_DISCLAIMER)
    // DMA初期化
    memset(aDST_Buffer, 0x00, sizeof(aDST_Buffer));
    APP_DmaConfig();

    // DMA転送完了待ち
    LL_mDelay(1);

    // DMA転送エラー確認
    if((s_dma_transfer_error_flg != false) || (s_dma_transfer_fail_flg != false)) {
        while (1)
        {
            APP_UsartTransmit_IT(USART1, (uint8_t *)dma_fail_str, sizeof(dma_fail_str));
            LL_mDelay(100);
        }
    }
#endif

    // アプリ初期化
    app_main_init();

    while (1)
    {
        APP_UsartTransmit_IT(USART1, (uint8_t *)uart_test_str_buf, sizeof(uart_test_str_buf));

        // アプリメイン
        app_main();

        LL_mDelay(1000);
    }
}

/**
  * @brief  System clock configuration
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
    uint32_t fact_hsi_trim_val;

#if 0
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
    // [内部RCクロックのHSIを8MHzら24MHzに変更]
    LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);
#endif

    while(LL_RCC_HSI_IsReady() != 1)
    {
        NOP();
    }

#if defined(I_AGREE_TO_YOUR_DISCLAIMER)
    // [HSI@24MHzをPLLで48MHzに逓倍]
    LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct);

    LL_Init1msTick(HSI_FREQ);

    LL_SetSystemCoreClock(HSI_FREQ);
#else
    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    /*Configure HSISYS as system clock source */
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
#endif
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

    /* Enable UART module */
    LL_USART_Enable(USARTx);
    /* Configure auto baud rate detection */
    LL_USART_SetAutoBaudRateMode(USARTx, LL_USART_AUTOBAUDRATE_ONFALLINGEDGE);
    LL_USART_EnableAutoBaudRate(USARTx);
}

/**
  * @brief  USART transmit function.
  * @param  USARTx：USART module, can be USART1
  * @param  pData：transmit buffer
  * @param  Size：Size of the transmit buffer
  * @retval None
  */
void APP_UsartTransmit_IT(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size)
{
    TxBuff = pData;
    TxSize = Size;
    TxCount = Size;

    /*Enable transmit data register empty interrupt*/
    LL_USART_EnableIT_TXE(USARTx);
}

/**
  * @brief  USART receive function.
  * @param  USARTx：USART module, can be USART1
  * @param  pData：transmit buffer
  * @param  Size：Size of the transmit buffer
  * @retval None
  */
void APP_UsartReceive_IT(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size)
{
    RxBuff = pData;
    RxSize = Size;
    RxCount = Size;

    /*Enable parity error interrupt*/
    LL_USART_EnableIT_PE(USARTx);

    /*Enable error interrupt*/
    LL_USART_EnableIT_ERROR(USARTx);

    /*Enable receive data register not empty interrupt*/
    LL_USART_EnableIT_RXNE(USARTx);
}

/**
  * @brief  USART interrupt callback function
  * @param  USARTx：USART module, can be USART1
  * @retval None
  */
void APP_UsartIRQCallback(USART_TypeDef *USARTx)
{
    /*Receive data register not empty*/
    uint32_t errorflags = (LL_USART_IsActiveFlag_PE(USARTx) | LL_USART_IsActiveFlag_FE(USARTx) |\
                        LL_USART_IsActiveFlag_ORE(USARTx) | LL_USART_IsActiveFlag_NE(USARTx));
    if (errorflags == RESET)
    {
        if ((LL_USART_IsActiveFlag_RXNE(USARTx) != RESET) && (LL_USART_IsEnabledIT_RXNE(USARTx) != RESET))
        {
            *RxBuff = LL_USART_ReceiveData8(USARTx);
            RxBuff++;

            if (--RxCount == 0U)
            {
                LL_USART_DisableIT_RXNE(USARTx);
                LL_USART_DisableIT_PE(USARTx);
                LL_USART_DisableIT_ERROR(USARTx);
            }
            return;
        }
    }

    /*Receive error occurred*/
    if (errorflags != RESET)
    {
    /* Used for auto baud rate detection */
    if ((LL_USART_IsActiveFlag_RXNE(USARTx) != RESET) && (LL_USART_IsEnabledIT_RXNE(USARTx) != RESET))
    {
        *RxBuff = LL_USART_ReceiveData8(USARTx);
        RxBuff++;
        if (--RxCount == 0U)
        {
            LL_USART_DisableIT_RXNE(USARTx);
            LL_USART_DisableIT_PE(USARTx);
            LL_USART_DisableIT_ERROR(USARTx);
        }
        return;
    }
    }
    /*Transmit data register empty*/
    if ((LL_USART_IsActiveFlag_TXE(USARTx) != RESET) && (LL_USART_IsEnabledIT_TXE(USARTx) != RESET))
    {
        LL_USART_TransmitData8(USARTx, *TxBuff);
        TxBuff++;
        if (--TxCount == 0U)
        {
            LL_USART_DisableIT_TXE(USARTx);
            LL_USART_EnableIT_TC(USARTx);
        }
        return;
    }

    /*Transmission complete*/
    if ((LL_USART_IsActiveFlag_TC(USARTx) != RESET) && (LL_USART_IsEnabledIT_TC(USARTx) != RESET))
    {
        LL_USART_DisableIT_TC(USARTx);
        return;
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
        for example: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* infinite loop */
    while (1)
    {
        NOP();
    }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
