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

static float s_math_pi;
uint8_t s_tx_data_buf[16];

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
 * @brief アプリメイン初期化
 * 
 */
void app_main_init(void)
{
    memset(s_tx_data_buf, 0x00, sizeof(s_tx_data_buf));
    s_math_pi = 0;
}

/**
 * @brief アプリメイン
 * 
 */
void app_main(void)
{
    volatile int len;

    s_math_pi = app_math_pi_calc(3);
    len = sprintf((char *)&s_tx_data_buf[0], "pi=%.8f\r\n", s_math_pi);
    APP_UsartTransmit_IT(USART1, s_tx_data_buf, len);
}