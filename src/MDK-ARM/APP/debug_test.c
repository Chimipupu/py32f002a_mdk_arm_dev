/**
 * @file debug_test.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグ関連
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "debug_test.h"

// --------------------------------
// C Std library
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// PUYA SDK
// #include "py32f0xx_ll_dma.h"

// My App
#include "common.h"
#include "app_main.h"

// --------------------------------
#define LOOPS           10
#define TOTAL_DIGITS    (LOOPS * 4)
#define PI_BUF_SIZE     ((TOTAL_DIGITS * 10) / 3 + 2)
// --------------------------------
typedef int8_t (*p_func)(void);

typedef struct {
    char *p_test_name;
    p_func p_test_func;
} dbg_test_t;

static int8_t math_test(void);

const dbg_test_t g_dbg_test_tbl[] = {
    { "Math", math_test },
    { NULL, NULL }
};
const uint8_t g_dbg_test_tbl_size = sizeof(g_dbg_test_tbl) / sizeof(dbg_test_t);

static void calc_pi_spigot(void);
// --------------------------------
/**
 * @brief 円周率の計算(スピゴット・アルゴリズム)
 * @note 整数のみで円周率の各桁を順番に計算
 */
static void calc_pi_spigot(void)
{
    int pi[PI_BUF_SIZE];
    int i, k;
    int b, d;
    int c = 0;

    for (i = 0; i < PI_BUF_SIZE; i++)
    {
        pi[i] = 2000;
    }

    for (k = 0; k < LOOPS; k++)
    {
        d = 0;
        i = PI_BUF_SIZE - 1;

        while (i >= 0)
        {
            d += pi[i] * 10000;
            b = 2 * i + 1;
            pi[i] = d % b;
            d /= b;
            if (i > 0) {
                d *= i;
            }
            i--;
        }

        int val = c + d / 10000;
        if (k == 0) {
            DBG_PRINTF("%d.%03d", val / 1000, val % 1000);
        } else {
            DBG_PRINTF("%04d", val);
        }
        c = d % 10000;
    }
    DBG_PRINTF("...\r\n");
}

static int8_t math_test(void)
{
    // 円周率を計算(スピゴット・アルゴリズム)
    // NOTE: LOOPS = 10だと「3.141592653589793238462643383279502884197...」と表示されるはず
    calc_pi_spigot();
    return TEST_OK;
}

void dbg_test_init(void)
{
    // TODO
}

void dbg_test_main(void)
{
    int8_t ret;
    static uint8_t s_test = 0;
    static bool is_test_end = false;

    if (is_test_end != true) {
        if (g_dbg_test_tbl[s_test].p_test_name != NULL) {
            DBG_PRINTF("[DEBUG] %s Test: Start\r\n", g_dbg_test_tbl[s_test].p_test_name);
            ret = g_dbg_test_tbl[s_test].p_test_func();
            DBG_PRINTF("[DEBUG] Test Result: %s\r\n", (ret == TEST_OK) ? "OK" : "NG");
            s_test++;
        } else {
            is_test_end = true;
        }
    }
}