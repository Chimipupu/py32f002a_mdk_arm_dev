/**
 * @file i2c_s_reg.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief I2Cスレーブレジスタ
 * @version 0.1
 * @date 2025-11-22
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "i2c_s_reg.h"
#include "common.h"

static int8_t dmy_cbk(uint8_t addr, uint8_t val);

// アクセスされているレジスタアドレス
volatile static uint8_t s_proc_reg_addr = 0;

// I2Cスレーブレジスタ 256個
volatile uint8_t g_i2c_s_reg[256] = {0};

// I2Cスレーブレジスタデータテーブル
volatile const i2c_s_reg_tbl_data_t g_i2c_s_reg_data_tbl[256] = {
    // [0x00H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x00: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x01: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x02: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x03: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x04: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x05: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x06: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x07: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x08: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x09: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x0A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x0B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x0C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x0D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x0E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x0F: (TODO) Reserved

    // [0x10H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x10: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x11: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x12: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x13: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x14: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x15: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x16: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x17: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x18: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x19: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x1A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x1B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x1C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x1D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x1E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x1F: (TODO) Reserved

    // [0x20H番台]
    {0xFF, 0x00, dmy_cbk}, // Addr 0x20: SM_PROC_REQレジスタ(WO)
    {0xFF, 0x00, dmy_cbk}, // Addr 0x21: SM_PROC_RESレジスタ(RO)
    {0x00, 0x00, dmy_cbk}, // Addr 0x22: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x23: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x24: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x25: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x26: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x27: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x28: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x29: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x2A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x2B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x2C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x2D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x2E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x2F: (TODO) Reserved

    // [0x30H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x30: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x31: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x32: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x33: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x34: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x35: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x36: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x37: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x38: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x39: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x3A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x3B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x3C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x3D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x3E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x3F: (TODO) Reserved

    // [0x40H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x40: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x41: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x42: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x43: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x44: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x45: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x46: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x47: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x48: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x49: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x4A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x4B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x4C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x4D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x4E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x4F: (TODO) Reserved

    // [0x50H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x50: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x51: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x52: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x53: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x54: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x55: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x56: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x57: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x58: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x59: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x5A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x5B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x5C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x5D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x5E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x5F: (TODO) Reserved

    // [0x60H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x60: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x61: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x62: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x63: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x64: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x65: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x66: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x67: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x68: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x69: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x6A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x6B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x6C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x6D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x6E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x6F: (TODO) Reserved

    // [0x70H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x70: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x71: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x72: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x73: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x74: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x75: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x76: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x77: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x78: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x79: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x7A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x7B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x7C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x7D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x7E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x7F: (TODO) Reserved

    // [0x80H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x80: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x81: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x82: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x83: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x84: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x85: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x86: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x87: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x88: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x89: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x8A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x8B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x8C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x8D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x8E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x8F: (TODO) Reserved

    // [0x90H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0x90: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x91: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x92: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x93: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x94: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x95: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x96: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x97: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x98: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x99: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x9A: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x9B: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x9C: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x9D: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x9E: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0x9F: (TODO) Reserved

    // [0xA0H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0xA0: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA1: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA2: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA3: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA4: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA5: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA6: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA7: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA8: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xA9: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xAA: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xAB: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xAC: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xAD: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xAE: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xAF: (TODO) Reserved

    // [0xB0H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0xB0: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB1: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB2: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB3: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB4: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB5: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB6: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB7: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB8: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xB9: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xBA: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xBB: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xBC: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xBD: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xBE: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xBF: (TODO) Reserved

    // [0xC0H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0xC0: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC1: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC2: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC3: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC4: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC5: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC6: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC7: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC8: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xC9: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xCA: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xCB: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xCC: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xCD: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xCE: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xCF: (TODO) Reserved

    // [0xD0H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0xD0: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD1: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD2: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD3: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD4: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD5: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD6: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD7: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD8: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xD9: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xDA: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xDB: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xDC: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xDD: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xDE: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xDF: (TODO) Reserved

    // [0xE0H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0xE0: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE1: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE2: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE3: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE4: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE5: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE6: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE7: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE8: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xE9: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xEA: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xEB: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xEC: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xED: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xEE: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xEF: (TODO) Reserved

    // [0xF0H番台]
    {0x00, 0x00, dmy_cbk}, // Addr 0xF0: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xF1: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xF2: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xF3: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xF4: (TODO) Reserved
    {0xFF, 0xC7, dmy_cbk}, // Addr 0xF5: Who am Iレジスタ(RO)
    {0x00, 0x00, dmy_cbk}, // Addr 0xF6: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xF7: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xF8: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xF9: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xFA: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xFB: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xFC: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xFD: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xFE: (TODO) Reserved
    {0x00, 0x00, dmy_cbk}, // Addr 0xFF: (TODO) Reserved
};

static int8_t dmy_cbk(uint8_t addr, uint8_t val)
{
    int8_t ret = 0;
    // NOP
    return ret;
}

/**
 * @brief I2Cスレーブレジスタ読み出しAPI
 * 
 * @param addr レジスタアドレス(0x00~0xFF)
 * @return uint8_t 読み出し値
 */
uint8_t i2c_s_reg_read(uint8_t addr)
{
    uint8_t reg = 0;

    if((addr >= 0x00) && (addr <= 0xFF)) {
        reg = g_i2c_s_reg[addr] & g_i2c_s_reg_data_tbl[addr].resv_rw;

        if(addr == REG_SM_PROC_REQ) {
            g_i2c_s_reg[addr] = 0x00;
        }
    }

    return reg;
}

/**
 * @brief I2Cスレーブレジスタ書き込みAPI
 * 
 * @param addr レジスタアドレス(0x00~0xFF)
 * @param val 書き込み値
 */
void i2c_s_reg_write(uint8_t addr, uint8_t val)
{
    if((addr >= 0x00) && (addr <= 0xFF)) {
        g_i2c_s_reg[addr] = val & g_i2c_s_reg_data_tbl[addr].resv_rw;
    }
}

/**
 * @brief I2Cスレーブレジスタ初期化関数
 * 
 */
void i2c_s_reg_init(void)
{
    uint16_t i;

    // レジスタの初期値書き込み
    for (i = 0; i <= 0xFF; i++)
    {
        g_i2c_s_reg[i] = g_i2c_s_reg_data_tbl[i].init_val;
    }
}