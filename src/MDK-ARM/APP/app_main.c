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
#include "main.h"

#define SM_TX_DATA_BUF_SIZE     64

typedef enum {
    SM_ERROR        = 0xFF, // エラー状態
    SM_NONE         = 0x00, // 状態なし
    SM_IDLE         = 0x10, // アイドル状態
    SM_BUSY         = 0x20, // ビジー状態
    SM_PROC_END     = 0x30, // 処理完了状態
    SM_MATH_CALC    = 0x40, // 円周率計算状態
} e_state_machine;

static float s_math_pi;
static uint8_t s_tx_data_buf[SM_TX_DATA_BUF_SIZE] = {0};
static e_state_machine s_state = SM_NONE;

static void sm_init(void);
static void sm_main(void);

// ガウス・ルジャンドル法による円周率の計算
float app_math_pi_calc(uint32_t cnt)
{
    float a = 1.0;
    float b = 1.0 / sqrt(2);
    float t = 1.0 / 4.0;
    float p = 1.0;

    for (uint32_t i = 0; i < cnt; i++)
    {
        float an = (a + b) / 2.0;
        float bn = sqrt(a * b);
        float tn = t - p * pow(a - an, 2);
        float pn = 2.0 * p;

        a = an;
        b = bn;
        t = tn;
        p = pn;
    }

    float pi = pow(a + b, 2) / (4.0 * t);

    return pi;
}

/**
 * @brief ステートマシーン初期化
 * 
 */
void sm_init(void)
{
    s_state = SM_MATH_CALC;
}

/**
 * @brief ステートマシーン メイン
 * 
 */
void sm_main(void)
{
    switch (s_state)
    {
        case SM_ERROR:
            break;

        case SM_BUSY:
            break;

        case SM_PROC_END:
            printf("%s\r\n", s_tx_data_buf);
            memset(s_tx_data_buf, 0x00, sizeof(s_tx_data_buf));
            s_state = SM_IDLE;
            break;

        case SM_MATH_CALC:
            s_math_pi = app_math_pi_calc(4);
            sprintf((char *)&s_tx_data_buf[0], "pi = %f", s_math_pi);
            s_state = SM_PROC_END;
            break;

        case SM_NONE:
        case SM_IDLE:
        default:
            s_state = SM_MATH_CALC;
            break;
    }
}

/**
 * @brief アプリメイン初期化
 * 
 */
void app_main_init(void)
{
    memset(s_tx_data_buf, 0x00, sizeof(s_tx_data_buf));
}

/**
 * @brief アプリメイン
 * 
 */
void app_main(void)
{
    sm_main();
}