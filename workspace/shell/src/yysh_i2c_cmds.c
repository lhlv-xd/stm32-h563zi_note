/*
 * yysh_i2c_cmds.c
 *
 *  Created on: Dec 10, 2025
 *      Author: Yuyi
 */


#include "yysh_i2c_cmds.h"

/* Static Function */
static void show_i2c_data(uint8_t slave_address, uint8_t reg_address, uint8_t* data, uint16_t size);

/**
 * @brief Dump all I2C data.
 */
void yysh_i2cdump(void * data)
{
	/* format is incorrect */
	if (tokens[1][0] == '\0' || strncmp(tokens[1], "0x", 2) != 0) {
		return;
	}

	uint8_t ret;
	uint32_t slave_address;
	slave_address = yysh_getvalue32(tokens[1]);
	uint8_t recv_data[256];

	ret = yy_i2c_master_poll_read(slave_address, 0, recv_data, 256);
	if (ret != 0) {
		SHELL_DEBUG(YYSH_ERR, "i2cdump failed! code: %d\r\n",ret);
		return;
	}

	/* show recv_data */
	show_i2c_data(slave_address, 0, recv_data, 256);
}

/**
 * @brief Get the specified register adress data.
 */
void yysh_i2cget(void * data)
{
	/* format is incorrect */
	if (tokens[3][0] == '\0' || strncmp(tokens[1], "0x", 2) != 0 || strncmp(tokens[2], "0x", 2) != 0) {
		return;
	}

	uint8_t ret;
	uint32_t slave_address, reg_address;
	uint16_t recv_size;
	uint8_t recv_data[256];
	slave_address = yysh_getvalue32(tokens[1]);
	reg_address   = yysh_getvalue32(tokens[2]);
	recv_size     = (uint16_t) yysh_getvalue32(tokens[3]);


	ret = yy_i2c_master_poll_read(slave_address, reg_address, recv_data, recv_size);
	if (ret != 0) {
		SHELL_DEBUG(YYSH_ERR, "i2cdump failed! code: %d\r\n", ret);
		return;
	}

	/* show recv_data */
	show_i2c_data(slave_address, reg_address, recv_data, recv_size);



}

/**
 * @brief Set the specified register adress data.
 */
void yysh_i2cset(void * data)
{
	/* format is incorrect */
	if (tokens[3][0] == '\0' || strncmp(tokens[1], "0x", 2) != 0 || strncmp(tokens[2], "0x", 2) != 0) {
		return;
	}

	uint8_t ret;
	uint32_t slave_address, reg_address;
	uint8_t buf;
	slave_address = yysh_getvalue32(tokens[1]);
	reg_address   = yysh_getvalue32(tokens[2]);
	buf     = yysh_getvalue8(tokens[3]);


	ret = yy_i2c_master_poll_write(slave_address, reg_address, &buf, 1);
	if (ret != 0) {
		SHELL_DEBUG(YYSH_ERR, "i2cset failed! code: %d\r\n", ret);
		return;
	}
}

/**************************************************************************
 * I2C Slave Function
 **************************************************************************/
#define PAGE_SIZE 128
extern uint8_t LowerPage[PAGE_SIZE];
extern uint8_t Page00h[PAGE_SIZE];
extern uint8_t Page7Fh[PAGE_SIZE];


/**
 * @brief Show mcu internal page.
 */
void yysh_i2cshow_mcu_page(void * data)
{

	uint8_t pages[256];
	for (int i = 0; i < 128; i++) {
		pages[i] = LowerPage[i];
		switch (LowerPage[0x7F]) {
			case 0x00:
				pages[128+i] = Page00h[i];
				break;
			case 0x7F:
				pages[128+i] = Page7Fh[i];
				break;
			default:
				pages[128+i] = Page00h[i];
				break;
		}

	}

	show_i2c_data(0x52, 0x00, pages, 256);
}


/**
 * @brief Set the specified register adress data.
 */
void yysh_i2cset_mcu_page(void * data)
{
	/* format is incorrect */
	if (strncmp(tokens[1], "0x", 2) != 0 || strncmp(tokens[2], "0x", 2) != 0) {
		return;
	}

	uint32_t reg, value;
	reg = yysh_getvalue32(tokens[1]);
	value = yysh_getvalue32(tokens[2]);

	if (reg < 0x80) {
		LowerPage[reg] = value;
	}
	else {
		uint8_t *ptr;
		switch (LowerPage[0x7F]) {
			case 0x00:
				ptr = &Page00h[0];
				break;
			case 0x7F:
				ptr = &Page7Fh[0];
				break;
			default:
				ptr = &Page00h[0];
				break;
		}
		ptr[reg-0x80] = value;
	}






}

/**************************************************************************
 * Static Function
 **************************************************************************/

/**
 * @brief Display data in a specific format.
 */
static void show_i2c_data(uint8_t slave_address, uint8_t reg_address, uint8_t* data, uint16_t size)
{
	uint8_t index = 0;
	uint32_t start = (reg_address ) & ~(0xf);
	uint32_t total_size = (((reg_address + size) + 0xf) & ~(0xf)) - start;
	uint32_t cols = total_size / 16;

	/* x-axis labels */
	SHELL_PRINTF("  Slave Address: 0x%02x\r\n\r\n", slave_address);
	SHELL_PRINTF("            ");
	for (int i = 0; i < 16; i++ ) {
		char x_lable[2];
		sprintf(x_lable, "%x", i);
		SHELL_PRINTF("%2s ", x_lable);
	}
	SHELL_PRINTF("\r\n  ");

	for (uint32_t i = 0; i < cols; i++) {
		/* y-axis labels */
		char y_lable[10];
		sprintf(y_lable, "%08x:", start + i*16);
		SHELL_PRINTF("%9s ", y_lable);

		for (int j = 0; j < 16; j++) {
			// data
			char value[3];
			if ((start + i*16 + j) < reg_address) {
				strcpy(value, "  ");
			}
			else if ((start + i*16 + j) >= (reg_address + size) ){
				strcpy(value, "  ");
			}
			else {
				sprintf(value, "%02x", data[index++]);
			}
			SHELL_PRINTF("%2s ", value);
		}
		SHELL_PRINTF("\r\n  ");
	}
}

