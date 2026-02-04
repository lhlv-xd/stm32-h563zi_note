/*
 * yysh_flash_cmds.c
 *
 *  Created on: Feb 3, 2026
 *      Author: Yuyi_Chen
 */


#include "yysh_flash_cmds.h"

/**
 * @brief Erase Flash
 * @note  e.g. $ flasherase 0x8100000 2   ## Erase two sectors from 0x8100000.
 */
void yysh_erase_flash(void* data)
{
	/* format is incorrect */
	if (tokens[2][0] == '\0' || strncmp(tokens[1], "0x", 2) != 0) {
		return;
	}

	uint32_t addr, sector_nums;
	addr = yysh_getvalue32(tokens[1]);
	sector_nums = yysh_getvalue32(tokens[2]);

	uint32_t size = sector_nums * FLASH_SECTOR_SIZE;
	uint8_t status;
	status = yy_hal_shell_flash_erase(addr, size);
	if (status != 0) {
		SHELL_PRINTF("Erase 0x%x-0x%x is failed!\r\n", addr, addr+size-1);
		return;
	}
	SHELL_PRINTF("Erase 0x%x-0x%x is successful!\r\n", addr, addr+size-1);

}
