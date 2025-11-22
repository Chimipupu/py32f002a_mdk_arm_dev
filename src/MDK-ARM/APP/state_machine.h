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

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    SM_INIT = 0x00, // 初期化状態
    SM_IDLE,        // アイドル状態
    SM_EXEC,        // 処理実行状態
    SM_BUSY ,       // ビジー状態
    SM_ERROR,       // エラー状態
} e_state_machine;

void sm_init(void);
void sm_main(void);

#endif // STATE_MACHINE_H