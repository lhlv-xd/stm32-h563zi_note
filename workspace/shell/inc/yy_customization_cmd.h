/*
 * yy_customization_cmd.h
 *
 *  Created on: Dec 7, 2025
 *      Author: Yuyi
 */

#ifndef SHELL_INC_YY_CUSTOMIZATION_CMD_H_
#define SHELL_INC_YY_CUSTOMIZATION_CMD_H_

/* Standard Header */
#include <stdint.h>
#include <string.h>

/* Shell Header */
#include "yy_shell_interface.h"

/* [Customization Header] Customized Shell CMD Header */
#include "yysh_memory_cmds.h"
#include "yysh_i2c_cmds.h"
#include "yysh_env_cmds.h"
#include "yysh_flash_cmds.h"

/* Structure */
struct cmd {
	char *name;
	void (*func)(void* data);
};

/* Macro */


/* Extern */
extern struct cmd cmds[];


/* Prototype */
uint8_t yysh_getvalue8(const char* value);
uint32_t yysh_getvalue32(const char* value);
uint32_t yysh_cmds_size();

// Customized command

#endif /* SHELL_INC_YY_CUSTOMIZATION_CMD_H_ */
