/**
 * @file i2c_slave_handler.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief I2Cスレーブ割り込みハンドラ
 * @version 0.1
 * @date 2025-11-29
 *
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 *
 */

#include "common.h"
#include "py32f0xx_ll_i2c.h"

extern volatile uint8_t *p_i2c_slave_buf;
extern volatile uint16_t g_i2c_slave_cnt;
extern volatile uint32_t g_i2c_slave_state;

volatile uint8_t g_req_reg_addr = 0;
static uint8_t s_rx_buf[256] = {0};

// アドレス割り込みの有無
static bool is_addr_int = false;

// 要求アドレス取得有無
static bool is_req_addr = false;

static void I2C_Slave_IRQ_Callback(void);
static void I2C_Slave_IRQ_Callback_NACK(void);

static void I2C_Slave_IRQ_Callback(void)
{
    static uint8_t *p_buf = NULL;
    volatile uint8_t dmmy;

    /* [アドレス割り込み] */
    if ((LL_I2C_IsActiveFlag_ADDR(I2C1) == 1) && (LL_I2C_IsEnabledIT_EVT(I2C1) == 1))
    {
        memset(&s_rx_buf[0], 0x00, sizeof(s_rx_buf));
        p_buf = &s_rx_buf[0];
        is_req_addr = false;
        is_addr_int = true;
        LL_I2C_ClearFlag_ADDR(I2C1);
    }
    /* [STOP割り込み] */
    else if (LL_I2C_IsActiveFlag_STOP(I2C1) == 1)
    {
        LL_I2C_DisableIT_EVT(I2C1);
        LL_I2C_DisableIT_BUF(I2C1);
        LL_I2C_DisableIT_ERR(I2C1);

        LL_I2C_ClearFlag_STOP(I2C1);

        LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);

        /* Data not fully received, receive data failed */
        if (g_i2c_slave_cnt != 0U)
        {
            if ((LL_I2C_IsActiveFlag_BTF(I2C1) == 1))
            {
                dmmy = LL_I2C_ReceiveData8(I2C1);
            }

            if ((LL_I2C_IsActiveFlag_RXNE(I2C1) == 1))
            {
                dmmy = LL_I2C_ReceiveData8(I2C1);
            }

            // 受信失敗
            if (g_i2c_slave_cnt != 0U)
            {
                /* Handle data reception failure */
            }
        }

        if (g_i2c_slave_state == I2C_STATE_BUSY_RX)
        {
            g_i2c_slave_state = I2C_STATE_READY;
        }
    }
    /* Slave Transmit */
    else if (g_i2c_slave_state == I2C_STATE_BUSY_TX)
    {
        /* Set TXE flag, BTF flag is not set */
        if ((LL_I2C_IsActiveFlag_TXE(I2C1) == 1) && (LL_I2C_IsEnabledIT_BUF(I2C1) == 1) && (LL_I2C_IsActiveFlag_BTF(I2C1) == 0))
        {
            LL_I2C_TransmitData8(I2C1, s_rx_buf[g_req_reg_addr]);
        }
        /* Set BTF flag */
        else if ((LL_I2C_IsActiveFlag_BTF(I2C1) == 1) && (LL_I2C_IsEnabledIT_EVT(I2C1) == 1))
        {
            // TODO:
        }
    }
    /* Slave Receive */
    else
    {
        /* Set RXNE flag, BTF flag is not set */
        if ((LL_I2C_IsActiveFlag_RXNE(I2C1) == 1) && (LL_I2C_IsEnabledIT_BUF(I2C1) == 1) && (LL_I2C_IsActiveFlag_BTF(I2C1) == 0))
        {
            if((is_addr_int == true) && (is_req_addr == false)) {
                // 要求されてるレジスタのアドレス
                g_req_reg_addr = LL_I2C_ReceiveData8(I2C1);
            } else {
                *p_buf = LL_I2C_ReceiveData8(I2C1);
                p_buf++;
            }
        }
        /* Set BTF flag */
        else if ((LL_I2C_IsActiveFlag_BTF(I2C1) == 1) && (LL_I2C_IsEnabledIT_EVT(I2C1) == 1))
        {
            // TODO:
        }
    }
}

static void I2C_Slave_IRQ_Callback_NACK(void)
{
    if ((LL_I2C_IsActiveFlag_AF(I2C1) == 1) && (LL_I2C_IsEnabledIT_ERR(I2C1) == 1))
    {
        if ((g_i2c_slave_cnt == 0) && (g_i2c_slave_state == I2C_STATE_BUSY_TX))
        {
            LL_I2C_DisableIT_EVT(I2C1);
            LL_I2C_DisableIT_BUF(I2C1);
            LL_I2C_DisableIT_ERR(I2C1);

            LL_I2C_ClearFlag_AF(I2C1);

            LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);

            g_i2c_slave_state = I2C_STATE_READY;
        }
    }
}

/**
 * @brief I2Cスレーブ割り込みハンドラ
 *
 */
void I2C1_IRQHandler(void)
{
    I2C_Slave_IRQ_Callback();
    I2C_Slave_IRQ_Callback_NACK();
}