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
#include "i2c_s_reg.h"
#include "state_machine.h"

#ifdef CALC_MATH_PI
static float math_pi_calc(uint8_t cnt);

// ガウス・ルジャンドル法による円周率の計算
static float math_pi_calc(uint8_t cnt)
{
    uint8_t i;
    float a = 1.0;
    float b = 1.0 / sqrt(2);
    float t = 1.0 / 4.0;
    float p = 1.0;

    for (i = 0; i < cnt; i++)
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

static void math_pi_print(void)
{
    volatile float pi = 0;

#ifdef CALC_MATH_PI
    pi = math_pi_calc(4);
#else
    pi = MATH_PI;
#endif

    DBG_PRINTF("pi = %f\r\n", pi);
}
#endif // CALC_MATH_PI

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
}

/**
 * @brief アプリメイン
 * 
 */
void app_main(void)
{
    // ステートマシーン メイン
    sm_main();
}