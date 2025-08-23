/**
 * @file main.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  PY32F002A メイン
 * @version 0.1
 * @date 2025-08-23
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

// #define HSI_FREQ             8000000
// #define HSI_FREQ            24000000

#if defined(I_AGREE_TO_YOUR_DISCLAIMER)
#include "py32f0xx_ll_dma.h"
#define HSI_FREQ            48000000
extern void APP_TransferCompleteCallback(void);
extern void APP_TransferErrorCallback(void);
#endif

#if !defined(I_AGREE_TO_YOUR_DISCLAIMER) && (HSI_FREQ == 48000000)
#error I agree to the disclaimer, run PY32F002A as PY32F030, and multiply HSI to 48 MHz with PLL!
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f0xx_ll_rcc.h"
#include "py32f0xx_ll_bus.h"
#include "py32f0xx_ll_system.h"
#include "py32f0xx_ll_exti.h"
#include "py32f0xx_ll_cortex.h"
#include "py32f0xx_ll_utils.h"
#include "py32f0xx_ll_pwr.h"
#include "py32f0xx_ll_gpio.h"
#include "py32f0xx_ll_usart.h"

#if defined(USE_FULL_ASSERT)
#include "py32_assert.h"
#endif /* USE_FULL_ASSERT */

#define UART_TX_BUF_SIZE    32
#define UART_RX_BUF_SIZE    32

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void APP_UsartTransmit_IT(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size);
void APP_UsartReceive_IT(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size);
void APP_ErrorHandler(void);
void APP_UsartIRQCallback(USART_TypeDef *USARTx);
/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
