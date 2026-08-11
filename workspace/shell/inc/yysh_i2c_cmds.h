/*
 * yysh_i2c_cmds.h
 *
 *  Created on: Dec 10, 2025
 *      Author: Yuyi
 */

#ifndef SHELL_INC_YYSH_I2C_CMDS_H_
#define SHELL_INC_YYSH_I2C_CMDS_H_

#include "yy_hal_shell.h"


/* Prototype (commands) */
void yysh_i2cdump(void * data);
void yysh_i2cget(void * data);
void yysh_i2cset(void * data);
// I2C Slave
void yysh_i2cshow_mcu_page(void * data);
void yysh_i2cset_mcu_page(void * data);

#endif /* SHELL_INC_YYSH_I2C_CMDS_H_ */
