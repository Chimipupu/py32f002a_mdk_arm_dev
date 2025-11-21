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

static e_state_machine s_state = SM_INIT;

/**
 * @brief ステートマシーン初期化
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
    switch (s_state) {
        // 初期化状態
        case SM_INIT:
            // TODO:
            break;

        // ビジー状態
        case SM_BUSY:
            // TODO:
            break;

        // 処理実行状態
        case SM_EXEC:
            // TODO:
            break;

        // エラー状態
        case SM_ERROR:
            // TODO:
            break;

        // アイドル状態
        case SM_IDLE:
        default:
            // TODO:
            break;
    }
}