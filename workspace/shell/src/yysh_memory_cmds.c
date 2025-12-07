/*
 * yysh_memory_cmds.c
 *
 *  Created on: Dec 8, 2025
 *      Author: Yuyi
 */

#include "yysh_memory_cmds.h"

/* Static Prototype */
static void memory_printf(uint32_t addr, uint32_t size);



/*
 * @brief print value of memory address
 *        Usage: > memread <address> <size>
 */
void yysh_read_memory(void * data)
{
	/* format is incorrect */
	if (tokens[2][0] == '\0' || strncmp(tokens[1], "0x", 2) != 0) {
		return;
	}

	uint32_t addr, size;
	addr = yysh_getvalue32(tokens[1]);
	size = yysh_getvalue32(tokens[2]);

	memory_printf(addr, size);
}



/*
 * @brief print memory to screen
 */
static void memory_printf(uint32_t addr, uint32_t size)
{
/*
  > memread 0x20000000 100

             0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
  20000000: 00 48 e8 01 0f 00 00 00 01 00 00 00 cc b5 00 08
  20000010: 01 97 00 08 d4 b5 00 08 41 97 00 08 dc b5 00 08
  20000020: bd 9d 00 08 ff 00 00 00 00 00 00 00 03 00 00 00
  20000030: 68 04 00 20 38 00 00 20 00 00 00 00 68 04 00 20
  20000040: d0 04 00 20 38 05 00 20 00 00 00 00 00 00 00 00
  20000050: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  20000060: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*/

	uint32_t start = (addr ) & ~(0xf);
	uint32_t total_size = (((addr + size) + 0xf) & ~(0xf)) - start;
	uint32_t cols = total_size / 16;

	/* Printf Message */
	/* x-axis labels */
	SHELL_PRINTF("            ");
	for (int i = 0; i < 16; i++ ) {
		char x_lable[2];
		sprintf(x_lable, "%x", i);
		SHELL_PRINTF("%2s ", x_lable);
	}
	SHELL_PRINTF("\r\n  ");


	uint8_t *ptr = (uint8_t *)start;
	for (uint32_t i = 0; i < cols; i++) {
		/* y-axis labels */
		char y_lable[10];
		sprintf(y_lable, "%08x:", start + i*16);
		SHELL_PRINTF("%9s ", y_lable);

		for (int j = 0; j < 16; j++) {
			// data
			char value[3];
			sprintf(value, "%02x", *ptr++);
			SHELL_PRINTF("%2s ", value);
		}
		SHELL_PRINTF("\r\n  ");
	}
}
