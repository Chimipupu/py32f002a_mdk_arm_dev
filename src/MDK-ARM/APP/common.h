/**
 * @file common.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief 共通ヘッダー
 * @version 0.1
 * @date 2025-11-28
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef COMMON_H
#define COMMON_H

// C Std Lib
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// PUYA Lib
#include "py32f0xx.h"


// (DEBUG)デバッグ用printf()
#define DEBUG_PRINTF_USE

#ifdef DEBUG_PRINTF_USE
#define DBG_PRINTF      printf
#else
#define DBG_PRINTF(...)  ((void)0)
#endif

// #define CALC_MATH_PI
#ifdef CALC_MATH_PI
#include <math.h>
#define MATH_PI        3.14159265358979323846
#endif // CALC_MATH_PI

#define UINT_8_MAX     255
#define UINT_16_MAX    65535
#define UINT_32_MAX    0xffffffffU  /* 4294967295U */
#define UINT_64_MAX    0xffffffffffffffffULL /* 18446744073709551615ULL */

// レジスタを8/16/32bitでR/Wするマクロ
#define REG_READ_BYTE(base, offset)         (*(volatile uint8_t  *)((base) + (offset)))
#define REG_READ_WORD(base, offset)         (*(volatile uint16_t *)((base) + (offset)))
#define REG_READ_DWORD(base, offset)        (*(volatile uint32_t *)((base) + (offset)))
#define REG_WRITE_BYTE(base, offset, val)   (*(volatile uint8_t  *)((base) + (offset)) = (val))
#define REG_WRITE_WORD(base, offset, val)   (*(volatile uint16_t *)((base) + (offset)) = (val))
#define REG_WRITE_DWORD(base, offset, val)  (*(volatile uint32_t *)((base) + (offset)) = (val))

// レジスタビット操作
#define REG_BIT_SET(reg, bit)               ((reg) |=  (1UL << (bit))) // レジスタのビットをセット
#define REG_BIT_CLR(reg, bit)               ((reg) &= ~(1UL << (bit))) // レジスタのビットをクリア
#define REG_BIT_TGL(reg, bit)               ((reg) ^=  (1UL << (bit))) // レジスタのビットをトグル
#define REG_BIT_CHK(reg, bit)               ((reg) &   (1UL << (bit))) // レジスタのビットチェック

// NOP
__attribute__( ( always_inline ) ) static inline void NOP(void)
{
    __asm__ __volatile__("nop");
}

// 割り込み禁止
__attribute__( ( always_inline ) ) static inline void _DI(void)
{
    __asm__ __volatile__("cpsid i");
}

// 割り込み許可
__attribute__( ( always_inline ) ) static inline void _EI(void)
{
    __asm__ __volatile__("cpsie i");
}

#endif // COMMON_H