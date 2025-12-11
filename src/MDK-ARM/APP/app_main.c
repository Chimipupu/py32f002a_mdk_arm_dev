/**
 * @file app_main.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメイン
 * @version 0.1
 * @date 2025-08-23
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "app_main.h"

// --------------------------------
// C Std library
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

// PUYA SDK
#include "py32f0xx_ll_usart.h"
#include "py32f0xx_it.h"

// My App
#include "main.h"
#include "common.h"
#include "i2c_s_reg.h"
#include "state_machine.h"
#include "debug_test.h"

// --------------------------------
extern uint32_t SystemCoreClock;
extern bool g_uart_rx_done_flg;
extern volatile uint8_t g_uart_rx_buf[];
extern volatile uint32_t g_idx_uart_rx_buf;

#ifdef DEBUG_UART_USE
typedef void (*p_cbk)(uint8_t *p_arg);
typedef struct {
    char *p_cmd_str;
    p_cbk p_callback;
} dbg_cmd_t;

static void cmd_help(uint8_t *p_arg);
static void cmd_cls(uint8_t *p_arg);
static void cmd_dbg(uint8_t *p_arg);
static void cmd_reg(uint8_t *p_arg);

const dbg_cmd_t g_dbg_cmd_tbl[] = {
    { "help", cmd_help },
    { "cls",  cmd_cls },
    { "dbg",  cmd_dbg },
    { "reg",  cmd_reg },
};
const uint8_t g_dbg_cmd_tbl_size = sizeof(g_dbg_cmd_tbl) / sizeof(dbg_cmd_t);

static void dbg_cmd_exec(uint8_t *p_buf);
// --------------------------------
static void cmd_help(uint8_t *p_arg)
{
    uint8_t i;

    DBG_UART_PRINTF("PY32F002A Develop F/W Ver 0.1\r\n");
    DBG_UART_PRINTF("Chimipupu(https://github.com/Chimipupu)\r\n");
    DBG_UART_PRINTF("Copyright (c) 2025 Chimipupu All Rights Reserved.\r\n");
    DBG_UART_PRINTF("[DEBUG] Clock: %d MHz\r\n", SystemCoreClock / 1000000);
    DBG_UART_PRINTF("[DEBUG] Flash: 16(32) KB, SRAM: 3(4) KB\r\n");
    DBG_UART_PRINTF("Available Commands: %d\r\n", g_dbg_cmd_tbl_size);

    for(i = 0; i < g_dbg_cmd_tbl_size; i++)
    {
        DBG_UART_PRINTF("No.%d: %s\r\n", i, g_dbg_cmd_tbl[i].p_cmd_str);
    }
}

static void cmd_cls(uint8_t *p_arg)
{
    // ANSIエスケープシーケンスで画面クリア
    DBG_UART_PRINTF("\033[2J\033[H");
}

static void cmd_dbg(uint8_t *p_arg)
{
    DBG_UART_PRINTF("[DEBUG] DBG Command\r\n");
    // TODO:
}

static void cmd_reg(uint8_t *p_arg)
{
    DBG_UART_PRINTF("[DEBUG] REG Command\r\n");
    // TODO:
}

static void dbg_cmd_exec(uint8_t *p_buf)
{
    uint8_t i;
    char *p_cmd;
    char *p_arg;

    if (p_buf == NULL || *p_buf == '\0') {
        return;
    }

    p_cmd = (char *)p_buf;
    p_arg = strchr(p_cmd, ' ');

    if (p_arg != NULL) {
        *p_arg = '\0';
        p_arg++;

        while (*p_arg == ' ') {
            p_arg++;
        }
    } else {
        p_arg = NULL;
    }

    // テーブルと照合
    for(i = 0; i < g_dbg_cmd_tbl_size; i++)
    {
        if (strcmp(p_cmd, (char *)g_dbg_cmd_tbl[i].p_cmd_str) == 0)
        {
            // DBG_UART_PRINTF("[DEBUG] cmd: %s\r\n", p_cmd);
            // DBG_UART_PRINTF("[DEBUG] arg: %s\r\n", p_arg ? p_arg : "None");

            // コマンド実行
            if(p_arg == NULL) {
                g_dbg_cmd_tbl[i].p_callback(NULL);
            } else {
                g_dbg_cmd_tbl[i].p_callback((uint8_t *)p_arg);
            }
            DBG_UART_PRINTF("\n> ");
            break;
        }
    }
}
#endif // DEBUG_UART_USE

// --------------------------------

/**
 * @brief アプリメイン初期化
 * 
 */
void app_main_init(void)
{
    // I2Cスレーブレジスタ初期化
    i2c_s_reg_init();

    // ステートマシーン初期化
    sm_init();

#ifdef DEBUG_TEST
    dbg_test_init();
#endif // DEBUG_TEST
}

/**
 * @brief アプリメイン
 * 
 */
void app_main(void)
{
#ifdef DEBUG_TEST
    dbg_test_main();
#endif // DEBUG_TEST

#ifdef DEBUG_UART_USE
    // コマンド処理
    if(g_uart_rx_done_flg == true)
    {
        dbg_cmd_exec((uint8_t *)&g_uart_rx_buf[0]);
        g_uart_rx_done_flg = false;
    }
#endif // DEBUG_UART_USE

    // ステートマシーン メイン
    sm_main();
}