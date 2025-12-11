/**
 * @file debug_test.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグ関連
 * @version 0.1
 * @date 2025-12-10
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef DEBUG_TEST_H
#define DEBUG_TEST_H

#define TEST_NG     (-1)
#define TEST_OK     (0)

// --------------------------------
// [コンパイルスイッチ]
// NOTE: テスト関連
// #define DEBUG_TEST
// --------------------------------
void dbg_test_init(void);
void dbg_test_main(void);

#endif // DEBUG_TEST_H