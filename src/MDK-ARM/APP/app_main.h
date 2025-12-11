/**
 * @file app_main.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメインのヘッダ
 * @version 0.1
 * @date 2025-08-23
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// --------------------------------
// [コンパイルスイッチ]
// NOTE: UARTでのprintf()用　※ただこれはROMを7KBも食う大食い
#define DEBUG_UART_USE

// NOTE: テスト関連
// #define DEBUG_TEST
// --------------------------------
void app_main_init(void);
void app_main(void);

#endif // APP_MAIN_H