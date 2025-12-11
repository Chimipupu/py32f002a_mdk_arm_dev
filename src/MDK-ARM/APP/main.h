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
#define HSI_FREQ            48000000

extern void APP_TransferCompleteCallback(void);
extern void APP_TransferErrorCallback(void);
extern void APP_LPTIMCallback(void);

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "py32f0xx_ll_rcc.h"
#include "py32f0xx_ll_bus.h"
#include "py32f0xx_ll_system.h"
#include "py32f0xx_ll_exti.h"
#include "py32f0xx_ll_cortex.h"
#include "py32f0xx_ll_utils.h"
#include "py32f0xx_ll_pwr.h"
#include "py32f0xx_ll_dma.h"
#include "py32f0xx_ll_gpio.h"
#include "py32f0xx_ll_lptim.h"
#include "py32f0xx_ll_i2c.h"
#include "py32f0xx_ll_spi.h"
#include "py32f0xx_ll_usart.h"
#include "py32f0xx_ll_tim.h"
#include "py32f0xx_ll_lptim.h"
#include "py32f0xx_ll_rtc.h"

#if defined(USE_FULL_ASSERT)
#include "py32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void DBG_UART_PRINTF(const char *format, ...);
void APP_ErrorHandler(void);
void APP_UsartIRQCallback(USART_TypeDef *USARTx);
/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
