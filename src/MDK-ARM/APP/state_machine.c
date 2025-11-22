/**
 * @file state_machine.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ステートマシーンのヘッダ
 * @version 0.1
 * @date 2025-11-21
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "state_machine.h"
#include "i2c_s_reg.h"

static e_state_machine s_state = SM_INIT;

/**
 * @br1ef ステートマシーン初期化
 * 
 */
void sm_init(void)
{
    s_state = SM_INIT;
}

/**
 * @brief ステートマシーン メイン
 * 
 */
void sm_main(void)
{
    uint8_t reg;

    switch (s_state) {
        // 初期化状態
        case SM_INIT:
            reg = i2c_s_reg_read(REG_ADDR_WHO_AM_I);
        #if 1
            // (DEBUG)
            i2c_s_reg_write(REG_SM_PROC_REQ, 0x01);
        #endif
            s_state = SM_IDLE;
            break;

        // ビジー状態
        case SM_BUSY:
            // TODO:
            break;

        // 処理実行状態
        case SM_EXEC:
            __asm volatile("nop");
            break;

        // エラー状態
        case SM_ERROR:
            // TODO:
            break;

        // アイドル状態
        case SM_IDLE:
        default:
            // SM_PROC_REQレジスタをポーリング
            reg = i2c_s_reg_read(REG_SM_PROC_REQ);
            if(reg) {
                s_state = SM_EXEC;
            }
            break;
    }
}