/*
 * yy_shell_interface.h
 *
 *  Created on: Dec 7, 2025
 *      Author: Yuyi
 */

#ifndef SHELL_INC_YY_SHELL_INTERFACE_H_
#define SHELL_INC_YY_SHELL_INTERFACE_H_

#include "yy_hal_shell.h"

/* Defined */
// shell message
#define SHELL_EOL "\r\n"
#define SHELL_HEADER SHELL_EOL"MCU Info: "SHELL_EOL\
					 "\t--Compiled: "__DATE__" "__TIME__""SHELL_EOL

// shell config
#define SHELLBUF_SIZE 128
#define SHELL_SIGN   ">"
#define SHELL_PROMPT SHELL_EOL SHELL_SIGN" "

#define TOKEN_NUMS 8
#define TOKEN_CMD_SIZE 20
#define HISTORY_NUMS 4

// shell printf
#define SHELL_PRINTF(format, ...) yy_hal_shell_show(format, ##__VA_ARGS__)

/* Shell Debug */
typedef enum {
	YYSH_ERR,
	YYSH_WARNING,
	YYSH_NOTICE,
	YYSH_INFO,
	YYSH_DEBUG,
}yysh_shell_debug_e;


#define YYSH_SHELL_DEBUG_LEVEL YYSH_ERR
#define SHELL_DEBUG(debug, format, ...) { \
	if (debug <= YYSH_SHELL_DEBUG_LEVEL) { \
		SHELL_PRINTF(SHELL_EOL"[%s():%d]"format SHELL_EOL, __func__, __LINE__, ##__VA_ARGS__); \
	}}




/* Extern */
extern volatile uint8_t tokens[TOKEN_NUMS][TOKEN_CMD_SIZE];
extern volatile uint8_t exec_shell_command;
extern volatile uint8_t shellbuf[SHELLBUF_SIZE];

/* Enumerate */
typedef enum {
	PgUp,
	PgDn
}ARROW_KEYS_E;

/* Prototype */
// Init
void yy_shell_init();
void yy_shell_deinit();

/* Prototype */
void yysh_shell_interface(uint8_t received_data);
void yysh_exec(uint8_t* command_line);

#endif /* SHELL_INC_YY_SHELL_INTERFACE_H_ */
