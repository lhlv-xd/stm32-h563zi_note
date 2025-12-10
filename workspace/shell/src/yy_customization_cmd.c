/*
 * yy_customization_cmd.c
 *
 *  Created on: Dec 7, 2025
 *      Author: Yuyi
 */

#include "yy_customization_cmd.h"

/* Customization */
/*
 * @brief show all commands help message
 * @details You can add your customized command help to this.
 */
static void yysh_help(void* data)
{
	const char help_msg[] =
	"yysh SHELL. \r\n"
	"  help                                           Print this help message.\r\n"
	"  reboot                                         Reboot MCU.\r\n"
	"  clear                                          Clear screen.\r\n"
	"  memread <address> <size>                       Read MCU register.\r\n"
	"  i2cdump <slave addr>                           Dump I2C data.\r\n"
	"  i2cget <slave addr> <reg addr> <size>          Read I2C data.\r\n"
	"  i2cset <slave addr> <reg addr> <data>          Write I2C data.\r\n";

	SHELL_PRINTF(help_msg);
}

/*
 * @brief clear screen
 */
static void yysh_clear(void* data)
{
	SHELL_PRINTF("\x1B[2J\x1B[H");
}

/* Add your customized callback function to this */
struct cmd cmds[] = {
	{"help",               yysh_help               },
	{"clear",              yysh_clear              },
	{"memread",            yysh_read_memory        },
	{"i2cdump",            yysh_i2cdump            },
	{"i2cget",             yysh_i2cget             },
	{"i2cset",             yysh_i2cset             },
//	{"setenv",             yysh_setenv             },
//	{"getenv",             yysh_getenv             }
};

/*
 * @brief show cmds size
 */
uint32_t yysh_cmds_size()
{
	return sizeof(cmds) / sizeof(struct cmd);
}

/*
 * @brief convert string to uint8_t
 * @note if string include 0x, the result of uint8_t is hexadecimal, or it is decimal.
 */
uint8_t yysh_getvalue8(const char* value)
{
	uint8_t ret_value;
	char* endptr;
	ret_value =(strncmp(value, "0x", 2) == 0) ?  strtol(value, &endptr, 16) : strtol(value, &endptr, 10);

	SHELL_DEBUG(YYSH_DEBUG, "yysh_getvalue8: %d\r\n", ret_value);
	return ret_value;
}

/*
 * @brief convert string to uint32_t
 * @note if string include 0x, the result of uint8_t is hexadecimal, or it is decimal.
 */
uint32_t yysh_getvalue32(const char* value)
{
	uint32_t ret_value;
	char* endptr;
	ret_value =(strncmp(value, "0x", 2) == 0) ?  strtol(value, &endptr, 16) : strtol(value, &endptr, 10);

	SHELL_DEBUG(YYSH_DEBUG, "yysh_getvalue32: 0x%x\r\n", ret_value);
	return ret_value;
}

