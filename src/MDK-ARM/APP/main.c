/**
 * @file main.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  PY32F002A メイン
 * @version 0.1
 * @date 2025-08-23
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <stdint.h>
#include <string.h>

#include "app_main.h"
/* Private define ------------------------------------------------------------*/
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

/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_ConfigUsart(USART_TypeDef *USARTx);

/**
  * @brief  Main program.
  * @param  None
  * @retval int
  */
int main(void)
{
    /* Configure system clock */
    APP_SystemClockConfig();

    // クロック安定待ち
    LL_mDelay(300);

    // UART初期化
    memset(aRxBuffer, 0x00, sizeof(aRxBuffer));
    memset(aTxBuffer, 0x00, sizeof(aTxBuffer));
    APP_ConfigUsart(USART1);
    // LL_USART_SendAutoBaudRateReq(USART1);
    // APP_UsartReceive_IT(USART1, (uint8_t *)aRxBuffer, 1);

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
    /* Enable HSI */
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1)
    {
    }

    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    /*Configure HSISYS as system clock source */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
    {
    }

    /* Set APB1 prescaler*/
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_Init1msTick(8000000);

    /* Update system clock global variable SystemCoreClock (can also be updated by calling SystemCoreClockUpdate function) */
    LL_SetSystemCoreClock(8000000);
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
    }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
