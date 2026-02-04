/*
 * yysh_env_cmds.h
 *
 *  Created on: Feb 3, 2026
 *      Author: Yuyi_Chen
 */

#ifndef SHELL_INC_YYSH_ENV_CMDS_H_
#define SHELL_INC_YYSH_ENV_CMDS_H_

#include "yy_hal_shell.h"

/* Definition (It must be the same as gmtk_partition.h) */
#define ENV_ADDRESS 0x8180000
#define ENV_SIZE 0x2000
#define ENV_KEY_SIZE 32
#define ENV_VALUE_SIZE 96
#define ENV_NUMS (ENV_SIZE / (ENV_KEY_SIZE + ENV_VALUE_SIZE)) // Integer
#define ENV_ERASE_SIZE (8 * 1024)


/* Prototype */
void yysh_setenv(void* data);
void yysh_getenv(void* data);
void yysh_delenv(void* data);

#endif /* SHELL_INC_YYSH_ENV_CMDS_H_ */
