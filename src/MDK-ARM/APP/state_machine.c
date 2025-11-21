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

uint8_t sm_reg[0xFF] = {0};
static uint8_t s_reg_addr = 0;
static e_state_machine s_state = SM_INIT;

static int8_t dmy_func(uint8_t addr, uint8_t val);
static int8_t dmy_func(uint8_t addr, uint8_t val)
{
    int8_t ret = 0;

    // TODO:

    return ret;
}

const sm_reg_t g_sm_func_tbl[] = {
    /* Addr: 0 */ {0x00, 0x00, dmy_func},
    /* Addr: 1 */ {0x00, 0x00, dmy_func},
    /* Addr: 2 */ {0x00, 0x00, dmy_func},
    /* Addr: 3 */ {0x00, 0x00, dmy_func},
    /* Addr: 4 */ {0x00, 0x00, dmy_func},
    /* Addr: 5 */ {0x00, 0x00, dmy_func},
    /* Addr: 6 */ {0x00, 0x00, dmy_func},
    /* Addr: 7 */ {0x00, 0x00, dmy_func},
    /* Addr: 8 */ {0x00, 0x00, dmy_func},
    /* Addr: 9 */ {0x00, 0x00, dmy_func},
    /* Addr: 10 */ {0x00, 0x00, dmy_func},
    /* Addr: 11 */ {0x00, 0x00, dmy_func},
    /* Addr: 12 */ {0x00, 0x00, dmy_func},
    /* Addr: 13 */ {0x00, 0x00, dmy_func},
    /* Addr: 14 */ {0x00, 0x00, dmy_func},
    /* Addr: 15 */ {0x00, 0x00, dmy_func},
    /* Addr: 16 */ {0x00, 0x00, dmy_func},
    /* Addr: 17 */ {0x00, 0x00, dmy_func},
    /* Addr: 18 */ {0x00, 0x00, dmy_func},
    /* Addr: 19 */ {0x00, 0x00, dmy_func},
    /* Addr: 20 */ {0x00, 0x00, dmy_func},
    /* Addr: 21 */ {0x00, 0x00, dmy_func},
    /* Addr: 22 */ {0x00, 0x00, dmy_func},
    /* Addr: 23 */ {0x00, 0x00, dmy_func},
    /* Addr: 24 */ {0x00, 0x00, dmy_func},
    /* Addr: 25 */ {0x00, 0x00, dmy_func},
    /* Addr: 26 */ {0x00, 0x00, dmy_func},
    /* Addr: 27 */ {0x00, 0x00, dmy_func},
    /* Addr: 28 */ {0x00, 0x00, dmy_func},
    /* Addr: 29 */ {0x00, 0x00, dmy_func},
    /* Addr: 30 */ {0x00, 0x00, dmy_func},
    /* Addr: 31 */ {0x00, 0x00, dmy_func},
    /* Addr: 32 */ {0x00, 0x00, dmy_func},
    /* Addr: 33 */ {0x00, 0x00, dmy_func},
    /* Addr: 34 */ {0x00, 0x00, dmy_func},
    /* Addr: 35 */ {0x00, 0x00, dmy_func},
    /* Addr: 36 */ {0x00, 0x00, dmy_func},
    /* Addr: 37 */ {0x00, 0x00, dmy_func},
    /* Addr: 38 */ {0x00, 0x00, dmy_func},
    /* Addr: 39 */ {0x00, 0x00, dmy_func},
    /* Addr: 40 */ {0x00, 0x00, dmy_func},
    /* Addr: 41 */ {0x00, 0x00, dmy_func},
    /* Addr: 42 */ {0x00, 0x00, dmy_func},
    /* Addr: 43 */ {0x00, 0x00, dmy_func},
    /* Addr: 44 */ {0x00, 0x00, dmy_func},
    /* Addr: 45 */ {0x00, 0x00, dmy_func},
    /* Addr: 46 */ {0x00, 0x00, dmy_func},
    /* Addr: 47 */ {0x00, 0x00, dmy_func},
    /* Addr: 48 */ {0x00, 0x00, dmy_func},
    /* Addr: 49 */ {0x00, 0x00, dmy_func},
    /* Addr: 50 */ {0x00, 0x00, dmy_func},
    /* Addr: 51 */ {0x00, 0x00, dmy_func},
    /* Addr: 52 */ {0x00, 0x00, dmy_func},
    /* Addr: 53 */ {0x00, 0x00, dmy_func},
    /* Addr: 54 */ {0x00, 0x00, dmy_func},
    /* Addr: 55 */ {0x00, 0x00, dmy_func},
    /* Addr: 56 */ {0x00, 0x00, dmy_func},
    /* Addr: 57 */ {0x00, 0x00, dmy_func},
    /* Addr: 58 */ {0x00, 0x00, dmy_func},
    /* Addr: 59 */ {0x00, 0x00, dmy_func},
    /* Addr: 60 */ {0x00, 0x00, dmy_func},
    /* Addr: 61 */ {0x00, 0x00, dmy_func},
    /* Addr: 62 */ {0x00, 0x00, dmy_func},
    /* Addr: 63 */ {0x00, 0x00, dmy_func},
    /* Addr: 64 */ {0x00, 0x00, dmy_func},
    /* Addr: 65 */ {0x00, 0x00, dmy_func},
    /* Addr: 66 */ {0x00, 0x00, dmy_func},
    /* Addr: 67 */ {0x00, 0x00, dmy_func},
    /* Addr: 68 */ {0x00, 0x00, dmy_func},
    /* Addr: 69 */ {0x00, 0x00, dmy_func},
    /* Addr: 70 */ {0x00, 0x00, dmy_func},
    /* Addr: 71 */ {0x00, 0x00, dmy_func},
    /* Addr: 72 */ {0x00, 0x00, dmy_func},
    /* Addr: 73 */ {0x00, 0x00, dmy_func},
    /* Addr: 74 */ {0x00, 0x00, dmy_func},
    /* Addr: 75 */ {0x00, 0x00, dmy_func},
    /* Addr: 76 */ {0x00, 0x00, dmy_func},
    /* Addr: 77 */ {0x00, 0x00, dmy_func},
    /* Addr: 78 */ {0x00, 0x00, dmy_func},
    /* Addr: 79 */ {0x00, 0x00, dmy_func},
    /* Addr: 80 */ {0x00, 0x00, dmy_func},
    /* Addr: 81 */ {0x00, 0x00, dmy_func},
    /* Addr: 82 */ {0x00, 0x00, dmy_func},
    /* Addr: 83 */ {0x00, 0x00, dmy_func},
    /* Addr: 84 */ {0x00, 0x00, dmy_func},
    /* Addr: 85 */ {0x00, 0x00, dmy_func},
    /* Addr: 86 */ {0x00, 0x00, dmy_func},
    /* Addr: 87 */ {0x00, 0x00, dmy_func},
    /* Addr: 88 */ {0x00, 0x00, dmy_func},
    /* Addr: 89 */ {0x00, 0x00, dmy_func},
    /* Addr: 90 */ {0x00, 0x00, dmy_func},
    /* Addr: 91 */ {0x00, 0x00, dmy_func},
    /* Addr: 92 */ {0x00, 0x00, dmy_func},
    /* Addr: 93 */ {0x00, 0x00, dmy_func},
    /* Addr: 94 */ {0x00, 0x00, dmy_func},
    /* Addr: 95 */ {0x00, 0x00, dmy_func},
    /* Addr: 96 */ {0x00, 0x00, dmy_func},
    /* Addr: 97 */ {0x00, 0x00, dmy_func},
    /* Addr: 98 */ {0x00, 0x00, dmy_func},
    /* Addr: 99 */ {0x00, 0x00, dmy_func},
    /* Addr: 100 */ {0x00, 0x00, dmy_func},
    /* Addr: 101 */ {0x00, 0x00, dmy_func},
    /* Addr: 102 */ {0x00, 0x00, dmy_func},
    /* Addr: 103 */ {0x00, 0x00, dmy_func},
    /* Addr: 104 */ {0x00, 0x00, dmy_func},
    /* Addr: 105 */ {0x00, 0x00, dmy_func},
    /* Addr: 106 */ {0x00, 0x00, dmy_func},
    /* Addr: 107 */ {0x00, 0x00, dmy_func},
    /* Addr: 108 */ {0x00, 0x00, dmy_func},
    /* Addr: 109 */ {0x00, 0x00, dmy_func},
    /* Addr: 110 */ {0x00, 0x00, dmy_func},
    /* Addr: 111 */ {0x00, 0x00, dmy_func},
    /* Addr: 112 */ {0x00, 0x00, dmy_func},
    /* Addr: 113 */ {0x00, 0x00, dmy_func},
    /* Addr: 114 */ {0x00, 0x00, dmy_func},
    /* Addr: 115 */ {0x00, 0x00, dmy_func},
    /* Addr: 116 */ {0x00, 0x00, dmy_func},
    /* Addr: 117 */ {0x00, 0x00, dmy_func},
    /* Addr: 118 */ {0x00, 0x00, dmy_func},
    /* Addr: 119 */ {0x00, 0x00, dmy_func},
    /* Addr: 120 */ {0x00, 0x00, dmy_func},
    /* Addr: 121 */ {0x00, 0x00, dmy_func},
    /* Addr: 122 */ {0x00, 0x00, dmy_func},
    /* Addr: 123 */ {0x00, 0x00, dmy_func},
    /* Addr: 124 */ {0x00, 0x00, dmy_func},
    /* Addr: 125 */ {0x00, 0x00, dmy_func},
    /* Addr: 126 */ {0x00, 0x00, dmy_func},
    /* Addr: 127 */ {0x00, 0x00, dmy_func},
    /* Addr: 128 */ {0x00, 0x00, dmy_func},
    /* Addr: 129 */ {0x00, 0x00, dmy_func},
    /* Addr: 130 */ {0x00, 0x00, dmy_func},
    /* Addr: 131 */ {0x00, 0x00, dmy_func},
    /* Addr: 132 */ {0x00, 0x00, dmy_func},
    /* Addr: 133 */ {0x00, 0x00, dmy_func},
    /* Addr: 134 */ {0x00, 0x00, dmy_func},
    /* Addr: 135 */ {0x00, 0x00, dmy_func},
    /* Addr: 136 */ {0x00, 0x00, dmy_func},
    /* Addr: 137 */ {0x00, 0x00, dmy_func},
    /* Addr: 138 */ {0x00, 0x00, dmy_func},
    /* Addr: 139 */ {0x00, 0x00, dmy_func},
    /* Addr: 140 */ {0x00, 0x00, dmy_func},
    /* Addr: 141 */ {0x00, 0x00, dmy_func},
    /* Addr: 142 */ {0x00, 0x00, dmy_func},
    /* Addr: 143 */ {0x00, 0x00, dmy_func},
    /* Addr: 144 */ {0x00, 0x00, dmy_func},
    /* Addr: 145 */ {0x00, 0x00, dmy_func},
    /* Addr: 146 */ {0x00, 0x00, dmy_func},
    /* Addr: 147 */ {0x00, 0x00, dmy_func},
    /* Addr: 148 */ {0x00, 0x00, dmy_func},
    /* Addr: 149 */ {0x00, 0x00, dmy_func},
    /* Addr: 150 */ {0x00, 0x00, dmy_func},
    /* Addr: 151 */ {0x00, 0x00, dmy_func},
    /* Addr: 152 */ {0x00, 0x00, dmy_func},
    /* Addr: 153 */ {0x00, 0x00, dmy_func},
    /* Addr: 154 */ {0x00, 0x00, dmy_func},
    /* Addr: 155 */ {0x00, 0x00, dmy_func},
    /* Addr: 156 */ {0x00, 0x00, dmy_func},
    /* Addr: 157 */ {0x00, 0x00, dmy_func},
    /* Addr: 158 */ {0x00, 0x00, dmy_func},
    /* Addr: 159 */ {0x00, 0x00, dmy_func},
    /* Addr: 160 */ {0x00, 0x00, dmy_func},
    /* Addr: 161 */ {0x00, 0x00, dmy_func},
    /* Addr: 162 */ {0x00, 0x00, dmy_func},
    /* Addr: 163 */ {0x00, 0x00, dmy_func},
    /* Addr: 164 */ {0x00, 0x00, dmy_func},
    /* Addr: 165 */ {0x00, 0x00, dmy_func},
    /* Addr: 166 */ {0x00, 0x00, dmy_func},
    /* Addr: 167 */ {0x00, 0x00, dmy_func},
    /* Addr: 168 */ {0x00, 0x00, dmy_func},
    /* Addr: 169 */ {0x00, 0x00, dmy_func},
    /* Addr: 170 */ {0x00, 0x00, dmy_func},
    /* Addr: 171 */ {0x00, 0x00, dmy_func},
    /* Addr: 172 */ {0x00, 0x00, dmy_func},
    /* Addr: 173 */ {0x00, 0x00, dmy_func},
    /* Addr: 174 */ {0x00, 0x00, dmy_func},
    /* Addr: 175 */ {0x00, 0x00, dmy_func},
    /* Addr: 176 */ {0x00, 0x00, dmy_func},
    /* Addr: 177 */ {0x00, 0x00, dmy_func},
    /* Addr: 178 */ {0x00, 0x00, dmy_func},
    /* Addr: 179 */ {0x00, 0x00, dmy_func},
    /* Addr: 180 */ {0x00, 0x00, dmy_func},
    /* Addr: 181 */ {0x00, 0x00, dmy_func},
    /* Addr: 182 */ {0x00, 0x00, dmy_func},
    /* Addr: 183 */ {0x00, 0x00, dmy_func},
    /* Addr: 184 */ {0x00, 0x00, dmy_func},
    /* Addr: 185 */ {0x00, 0x00, dmy_func},
    /* Addr: 186 */ {0x00, 0x00, dmy_func},
    /* Addr: 187 */ {0x00, 0x00, dmy_func},
    /* Addr: 188 */ {0x00, 0x00, dmy_func},
    /* Addr: 189 */ {0x00, 0x00, dmy_func},
    /* Addr: 190 */ {0x00, 0x00, dmy_func},
    /* Addr: 191 */ {0x00, 0x00, dmy_func},
    /* Addr: 192 */ {0x00, 0x00, dmy_func},
    /* Addr: 193 */ {0x00, 0x00, dmy_func},
    /* Addr: 194 */ {0x00, 0x00, dmy_func},
    /* Addr: 195 */ {0x00, 0x00, dmy_func},
    /* Addr: 196 */ {0x00, 0x00, dmy_func},
    /* Addr: 197 */ {0x00, 0x00, dmy_func},
    /* Addr: 198 */ {0x00, 0x00, dmy_func},
    /* Addr: 199 */ {0x00, 0x00, dmy_func},
    /* Addr: 200 */ {0x00, 0x00, dmy_func},
    /* Addr: 201 */ {0x00, 0x00, dmy_func},
    /* Addr: 202 */ {0x00, 0x00, dmy_func},
    /* Addr: 203 */ {0x00, 0x00, dmy_func},
    /* Addr: 204 */ {0x00, 0x00, dmy_func},
    /* Addr: 205 */ {0x00, 0x00, dmy_func},
    /* Addr: 206 */ {0x00, 0x00, dmy_func},
    /* Addr: 207 */ {0x00, 0x00, dmy_func},
    /* Addr: 208 */ {0x00, 0x00, dmy_func},
    /* Addr: 209 */ {0x00, 0x00, dmy_func},
    /* Addr: 210 */ {0x00, 0x00, dmy_func},
    /* Addr: 211 */ {0x00, 0x00, dmy_func},
    /* Addr: 212 */ {0x00, 0x00, dmy_func},
    /* Addr: 213 */ {0x00, 0x00, dmy_func},
    /* Addr: 214 */ {0x00, 0x00, dmy_func},
    /* Addr: 215 */ {0x00, 0x00, dmy_func},
    /* Addr: 216 */ {0x00, 0x00, dmy_func},
    /* Addr: 217 */ {0x00, 0x00, dmy_func},
    /* Addr: 218 */ {0x00, 0x00, dmy_func},
    /* Addr: 219 */ {0x00, 0x00, dmy_func},
    /* Addr: 220 */ {0x00, 0x00, dmy_func},
    /* Addr: 221 */ {0x00, 0x00, dmy_func},
    /* Addr: 222 */ {0x00, 0x00, dmy_func},
    /* Addr: 223 */ {0x00, 0x00, dmy_func},
    /* Addr: 224 */ {0x00, 0x00, dmy_func},
    /* Addr: 225 */ {0x00, 0x00, dmy_func},
    /* Addr: 226 */ {0x00, 0x00, dmy_func},
    /* Addr: 227 */ {0x00, 0x00, dmy_func},
    /* Addr: 228 */ {0x00, 0x00, dmy_func},
    /* Addr: 229 */ {0x00, 0x00, dmy_func},
    /* Addr: 230 */ {0x00, 0x00, dmy_func},
    /* Addr: 231 */ {0x00, 0x00, dmy_func},
    /* Addr: 232 */ {0x00, 0x00, dmy_func},
    /* Addr: 233 */ {0x00, 0x00, dmy_func},
    /* Addr: 234 */ {0x00, 0x00, dmy_func},
    /* Addr: 235 */ {0x00, 0x00, dmy_func},
    /* Addr: 236 */ {0x00, 0x00, dmy_func},
    /* Addr: 237 */ {0x00, 0x00, dmy_func},
    /* Addr: 238 */ {0x00, 0x00, dmy_func},
    /* Addr: 239 */ {0x00, 0x00, dmy_func},
    /* Addr: 240 */ {0x00, 0x00, dmy_func},
    /* Addr: 241 */ {0x00, 0x00, dmy_func},
    /* Addr: 242 */ {0x00, 0x00, dmy_func},
    /* Addr: 243 */ {0x00, 0x00, dmy_func},
    /* Addr: 244 */ {0x00, 0x00, dmy_func},
    /* Addr: 245 */ {0x00, 0x00, dmy_func},
    /* Addr: 246 */ {0x00, 0x00, dmy_func},
    /* Addr: 247 */ {0x00, 0x00, dmy_func},
    /* Addr: 248 */ {0x00, 0x00, dmy_func},
    /* Addr: 249 */ {0x00, 0x00, dmy_func},
    /* Addr: 250 */ {0x00, 0x00, dmy_func},
    /* Addr: 251 */ {0x00, 0x00, dmy_func},
    /* Addr: 252 */ {0x00, 0x00, dmy_func},
    /* Addr: 253 */ {0x00, 0x00, dmy_func},
    /* Addr: 254 */ {0x00, 0x00, dmy_func},
    /* Addr: 255 */ {0x00, 0x00, dmy_func},
};

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
            // TODO: I2Cマスターからのアクセス レジスタアドレス取得
            // s_reg_addr = get_reg_addr();
            break;
    }
}