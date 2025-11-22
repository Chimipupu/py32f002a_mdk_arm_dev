/**
 * @file i2c_s_reg.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief I2Cスレーブレジスタヘッダー
 * @version 0.1
 * @date 2025-11-22
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef I2C_S_REG_H
#define I2C_S_REG_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// [レジスタアドレス]
#define REG_SM_PROC_REQ      0x20
#define REG_SM_PROC_RES      0x20
#define REG_ADDR_WHO_AM_I    0xF5

typedef int8_t (*p_sm_func)(uint8_t addr, uint8_t val);

// I2Cスレーブレジスタデータテーブル構造体
typedef struct {
    uint8_t resv_rw;   // レジスタ属性 Reserved(bit = 0), R/W(bit = 1)
    uint8_t init_val;  // レジスタ初期値
    p_sm_func p_cbk;   // コールバック関数ポインタ
} i2c_s_reg_tbl_data_t;

uint8_t i2c_s_reg_read(uint8_t addr);
void i2c_s_reg_write(uint8_t addr, uint8_t val);
void i2c_s_reg_init(void);

#endif // I2C_S_REG_H