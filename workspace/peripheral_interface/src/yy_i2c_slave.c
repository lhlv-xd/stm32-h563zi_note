/*
 * yy_i2c_slave.c
 *
 *  Created on: Dec 11, 2025
 *      Author: Yuyi
 */

#include "yy_i2c_slave.h"


/* ===== EEPROM-like memory ===== */
uint8_t LowerPage[PAGE_SIZE];
uint8_t Page00h[PAGE_SIZE];
uint8_t Page7Fh[PAGE_SIZE];

/* ===== Slave state ===== */
static uint8_t *pMem;               // current memory pointer
static uint8_t mem_addr;            // current memory address
static uint8_t pre_mem_addr;        // pre memory address
static uint8_t first_rx = 1;        // first received byte is address
static uint8_t active_page = 0x00;  // selected page
static uint8_t rx_byte;
static uint8_t tx_byte;

/* ===== Static Prototype ===== */
static void selectUpperPage();


/**
 * @brief i2c slave interrupt init
 */
void yy_i2c_slave_init()
{
	MX_I2C1_Init();

	/* Check if master send a message */
	if (HAL_I2C_EnableListen_IT(&i2cslaveHdl) != HAL_OK) {
		Error_Handler();
	}
}


/* =========================================================
 * Address matched callback
 * ========================================================= */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,
                          uint8_t TransferDirection,
                          uint16_t AddrMatchCode)
{
    if (hi2c->Instance != I2C1) {
        return;
    }

    /* if new transaction is comming, reset state */
    first_rx = 1;

    if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
        /* Master → Slave (Write) */
        HAL_I2C_Slave_Seq_Receive_IT(hi2c, &rx_byte, 1, I2C_FIRST_FRAME);
    }
    else {
        /* Master ← Slave (Read) */
        tx_byte = *pMem;
        HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &tx_byte, 1, I2C_FIRST_FRAME);
    }
}

/* =========================================================
 * Slave receive complete callback
 * ========================================================= */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (first_rx) {
        /* First byte is memory address */
        mem_addr = rx_byte;
        first_rx = 0;

        /* Page select */
        if (mem_addr >= 0x80) {
        	selectUpperPage();
        }
        else {
            pMem = &LowerPage[mem_addr];
        }
    }
    else {
        /* Write data (Master->Slave) */
        *pMem = rx_byte;

        pre_mem_addr = mem_addr;

        /* auto increment + rollover */
        mem_addr = (mem_addr + 1) & 0xFF;
        pMem++;


        if (pre_mem_addr == 0x7F) {
        	selectUpperPage();
        }
        else if (pre_mem_addr == 0xFF) {
        	pMem = &LowerPage[0];
        }
    }

    /* prepare to receive next byte */
    HAL_I2C_Slave_Seq_Receive_IT(hi2c, &rx_byte, 1, I2C_NEXT_FRAME);
}

/**
 * @brief select upper page
 */
static void selectUpperPage()
{
	active_page = LowerPage[127];

	if (active_page == 0x00) {
		pMem = &Page00h[mem_addr - 0x80];
	}
	else if (active_page == 0x7F) {
		pMem = &Page7Fh[mem_addr - 0x80];
	}
	else {
		LowerPage[127] = 0x00;
		pMem = &Page00h[mem_addr - 0x80];
	}
}

/* =========================================================
 * Slave transmit complete callback
 * ========================================================= */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	pre_mem_addr = mem_addr;

	/* auto increment + rollover */
	mem_addr = (mem_addr + 1) & 0xFF;
	pMem++;

	if (pre_mem_addr == 0x7F) {
		selectUpperPage();
	}
	else if (pre_mem_addr == 0xFF) {
		pMem = &LowerPage[0];
	}

    tx_byte = *pMem;

    HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &tx_byte, 1, I2C_NEXT_FRAME);
}

/* =========================================================
 * Listen complete (STOP condition)
 * ========================================================= */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_EnableListen_IT(hi2c);
}
