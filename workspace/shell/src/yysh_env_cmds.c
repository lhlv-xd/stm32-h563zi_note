/*
 * yysh_env_cmds.c
 *
 *  Created on: Feb 3, 2026
 *      Author: Yuyi_Chen
 */


#include "yysh_env_cmds.h"

/* Static Prototype */
static uint32_t _find_env(const char* key);


/**
 * @brief Set env to flash.
 */
void yysh_setenv(void* data)
{
	/* format is incorrect */
	if (tokens[1][0] == '\0') {
		return;
	}

	/* Find key */
	uint32_t addr = _find_env(tokens[1]);
	if (addr == 0xffffffff) {
		addr = _find_env(NULL);
		SHELL_PRINTF("space address: 0x%x\r\n", addr);
		if (addr == 0xffffffff) {
			SHELL_PRINTF("No any free space!!\r\n");
			return;
		}

	}

	/* Align */
	uint32_t aligned_addr;
	aligned_addr = addr & ~(ENV_ERASE_SIZE - 1);
	SHELL_PRINTF("aligned addr: 0x%x\r\n", aligned_addr);

	/* Process Data */
	uint8_t buf[ENV_ERASE_SIZE];
	yy_hal_shell_flash_read(aligned_addr, buf, ENV_ERASE_SIZE);

	// clear
	for (uint32_t i = addr - aligned_addr; i < addr - aligned_addr + ENV_KEY_SIZE + ENV_VALUE_SIZE; i++) {
		buf[i] = 0xff;
	}

	// set
	strncpy(&(buf[addr - aligned_addr]), tokens[1], strlen(tokens[1]) + 1);
	strncpy(&(buf[addr - aligned_addr + ENV_KEY_SIZE]), tokens[2], strlen(tokens[2]) + 1);

	/* Write to flash */
	yy_hal_shell_flash_write(aligned_addr, buf, ENV_ERASE_SIZE);
}

/**
 * @brief Get env from flash.
 */
void yysh_getenv(void* data)
{
	/* Specific key */
	if (tokens[1][0] != '\0') {
		uint32_t addr;
		addr = _find_env(tokens[1]);
		if (addr == 0xffffffff) {
			SHELL_PRINTF("Not Found!\r\n");
			return;
		}
		SHELL_PRINTF("%s = %s\r\n", (uint8_t *)(addr), (uint8_t *)(addr+ENV_KEY_SIZE));
	}

	/* Print all env */
	uint32_t start = ENV_ADDRESS;
	uint32_t end = ENV_ADDRESS + ENV_SIZE;

	for (uint32_t addr = start; addr < end; addr+= ENV_KEY_SIZE + ENV_VALUE_SIZE) {
		uint32_t* space_ptr = (uint32_t *)addr;
		if (*space_ptr == 0xffffffff) {
			continue;
		}

		SHELL_PRINTF("%s = %s\r\n", (uint8_t *)(addr), (uint8_t *)(addr+ENV_KEY_SIZE));
	}


}

/**
 * @brief Delete env from  flash.
 */
void yysh_delenv(void* data)
{
	/* format is incorrect */
	if (tokens[1][0] == '\0') {
		return;
	}

	uint32_t addr;
	addr = _find_env(tokens[1]);
	if (addr == 0xffffffff) {
		SHELL_PRINTF("Not Found!\r\n");
		return;
	}

	/* Align */
	uint32_t aligned_addr;
	aligned_addr = addr & ~(ENV_ERASE_SIZE - 1);
	SHELL_PRINTF("aligned addr: 0x%x\r\n", aligned_addr);

	/* Process Data */
	uint8_t buf[ENV_ERASE_SIZE];
	yy_hal_shell_flash_read(aligned_addr, buf, ENV_ERASE_SIZE);

	// clear
	for (uint32_t i = addr - aligned_addr; i < addr - aligned_addr + ENV_KEY_SIZE + ENV_VALUE_SIZE; i++) {
		buf[i] = 0xff;
	}

	/* Write to flash */
	yy_hal_shell_flash_write(aligned_addr, buf, ENV_ERASE_SIZE);
}


/**
 * @brief Find specific env.
 * @return env address or 0xffffffff
 */
static uint32_t _find_env(const char* key)
{
	uint32_t start = ENV_ADDRESS;
	uint32_t end = ENV_ADDRESS + ENV_SIZE;

	for (uint32_t addr = start; addr < end; addr+= ENV_KEY_SIZE + ENV_VALUE_SIZE) {
		if (key == NULL) {
			uint32_t* space_ptr = (uint32_t *)addr;
			if (*space_ptr == 0xffffffff) {
				return addr;
			}

			continue;
		}

		if (strncmp((uint8_t *)addr, key, strlen(key)) == 0) {
			SHELL_PRINTF("Found addr: 0x%x\r\n", addr);
			return addr;
		}

	}

	return 0xffffffff;
}
