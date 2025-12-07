/*
 * yy_shell_interface.c
 *
 *  Created on: Dec 7, 2025
 *      Author: Yuyi
 */

#include "yy_shell_interface.h"
#include "yy_customization_cmd.h"


/* Extern Variable */
volatile uint8_t tokens[TOKEN_NUMS][TOKEN_CMD_SIZE];
volatile uint8_t shellbuf[SHELLBUF_SIZE];
volatile uint8_t exec_shell_command = 0; // not exec now

/* Static Variable*/
static uint8_t historybuf[HISTORY_NUMS][SHELLBUF_SIZE];
static uint8_t history_index = 0;
static uint8_t shellbuf_index = 0;
static uint8_t is_esc = 0;
static int8_t hist_get_index = -1;

/* Static Prototype */
static uint8_t** yysh_split_line(uint8_t* command_line);
static void yysh_clear_tokens();
static void yysh_clear_shell_message();
static void yysh_set_history(uint8_t* content);
static void yysh_get_history(uint8_t* recvbuf, ARROW_KEYS_E mode);

/**
 * @brief Init shell
 */
void yy_shell_init()
{
	yy_hal_shell_init();
	SHELL_PRINTF(SHELL_HEADER);
	SHELL_PRINTF("\t--System Clock: %lu Hz"SHELL_EOL SHELL_EOL, SystemCoreClock);
}

/**
 * @brief Deinit shell
 */
void yy_shell_deinit()
{
	yy_hal_shell_deinit();
}

/**
 * @brief Processing shell commands
 * @param[in] command_line This is input from shell.
 * @note Usage:
 *          yy_shell_init();
 *          while (1) {
 *              if (exec_shell_command) {
 *                  exec_shell_command = 0;
 *                  yysh_exec(shellbuf);
 *              }
 *          }
 */
void yysh_exec(uint8_t* command_line)
{
	SHELL_DEBUG(YYSH_DEBUG, "[in %s function] \r\n", __FUNCTION__);

	/* split command line to tokens  */
	yysh_split_line(command_line);

	/* Execution tokens */
	for (uint32_t i = 0; i < yysh_cmds_size(); i++) {
		if (strcmp(tokens[0], cmds[i].name) == 0) {
			cmds[i].func(NULL);

			/* Clear tokens */
			yysh_clear_tokens();

			/* Show Prompt */
			SHELL_PRINTF(SHELL_PROMPT);
			return;
		}
	}
	/* Show Prompt */
	SHELL_PRINTF("\r"SHELL_SIGN" ");
}

/*
 * @brief Main Logic
 */
void yysh_shell_interface(uint8_t received_data)
{
	SHELL_DEBUG(YYSH_DEBUG, "[%c, %d]", received_data, received_data);

	/* Xmodem */
#if XMODEM_enable
	if (is_xmodem) {
		xmodembuf[xmodembuf_index++] = received_data;
		xmodem_mode = XMODEM_RECV_DATA;
		if (xmodembuf_index == 133) {
			xmodem_mode = XMODEM_RECV_OK;

			/* reset */
			xmodembuf_index = 0;
		}

		if (xmodembuf[0] == 0x04) {
			xmodem_mode = XMODEM_EOT;
		}
		return;
	}
#endif // XMODEM_enable
	/* Execute Command */
	if (received_data == '\r' || received_data == '\n') {
		/* No command, show prompt */
		if (shellbuf_index == 0) {
			SHELL_PRINTF(SHELL_PROMPT);
			return;
		}

		/* Deal with shellbuf */
		shellbuf[shellbuf_index] = '\0';

		/* Reset parameters */
		shellbuf_index = 0;
		hist_get_index = -1;

		/* Finish */
		exec_shell_command = 1;
		SHELL_PRINTF("\r\n");
		SHELL_DEBUG(YYSH_DEBUG, "\r\nFinish Command: %s (len: %d)", shellbuf, strlen(shellbuf));

		// reset mcu
		if (strcmp(shellbuf, "reboot") == 0) {
			__disable_irq(); // 關閉中斷，避免中途干擾
			SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos)
					   | SCB_AIRCR_SYSRESETREQ_Msk;
			while (1);
		}

		/* Record History */
		yysh_set_history(shellbuf);

		return;
	}

	/* Backspace  */
	if (received_data == 0x08) {
		if (shellbuf_index == 0) {
			return;
		}

		shellbuf[shellbuf_index--] = '\0';
		SHELL_PRINTF("\b \b");
		return;
	}

	/* ESC */
	if (received_data == 0x1b || is_esc) {
		is_esc = 1;
		if (received_data != 0x1b) {
			is_esc = 0;
		}

		switch (received_data) {
			case 0x5b: // ASCII: [
				is_esc = 1;
				break;
			/* PgUp */
			case 0x41:
				SHELL_DEBUG(YYSH_DEBUG, "Up,");
				yysh_clear_shell_message();

				yysh_get_history(shellbuf, PgUp);
				shellbuf_index = strlen(shellbuf);
				shellbuf[shellbuf_index] = '\0';

				SHELL_PRINTF("%s", shellbuf);
				break;
			/* PgDn */
			case 0x42:
				SHELL_DEBUG(YYSH_DEBUG, "Down,");
				yysh_clear_shell_message();

				yysh_get_history(shellbuf, PgDn);
				shellbuf_index = strlen(shellbuf);
				shellbuf[shellbuf_index] = '\0';

				SHELL_PRINTF("%s", shellbuf);
				break;
			/* Left */
			case 0x44:
				SHELL_DEBUG(YYSH_DEBUG, "Left,");
				break;
			/* Right */
			case 0x43:
				SHELL_DEBUG(YYSH_DEBUG, "Right,");
				break;
			default:
				break;
		}

		return;
	}

	/* Tab */
	if (received_data == 0x09) {
		for (uint32_t i = 0; i < yysh_cmds_size(); i++) {
			if (strncmp(cmds[i].name, shellbuf, shellbuf_index) == 0) {
				strcpy(shellbuf, cmds[i].name);
				shellbuf_index = strlen(cmds[i].name);

				//show
				yysh_clear_shell_message();
				SHELL_PRINTF("%s", shellbuf);
			}
		}
		return;
	}

	/* Check Size if bigger than SHELLBUF_SIZE */
	if (shellbuf_index >= SHELLBUF_SIZE - 1) {
		SHELL_DEBUG(YYSH_ERR, "Exceeds the character limit\r\n");
		return;
	}


	/* Show Character */
	SHELL_PRINTF("%c", received_data);

	/* Add character to shellbuf */
	shellbuf[shellbuf_index++] = received_data;
}

/**
 * @brief split commands to tokens
 * @param[in] command_line This is input from shell.
 */
static uint8_t** yysh_split_line(uint8_t* command_line)
{
	/* Declare */
	uint8_t cmd_buf[SHELLBUF_SIZE];
	const char* delimiter = " \t\r\n";
	char* substr;
	uint8_t tokens_index = 0;

	/* Split */
	strcpy(cmd_buf, command_line);
	substr = strtok(cmd_buf, delimiter);

	do {
		strcpy(tokens[tokens_index++], substr);
		substr = strtok(NULL, delimiter);
	} while(substr);


	return tokens;
}

/**
 * @brief Clear tokens to '\0'
 */
static void yysh_clear_tokens()
{
	for(uint8_t i = 0; i < TOKEN_NUMS; i++)
	{
		for(uint8_t j = 0; j < TOKEN_CMD_SIZE; j++)
		{
			tokens[i][j] = '\0';
		}
	}

}


/**
 * @brief Clear shell output message
 */
static void yysh_clear_shell_message()
{
	SHELL_PRINTF("\r");
	uint32_t size = strlen(shellbuf) + strlen(SHELL_PROMPT);
	size = (size > SHELLBUF_SIZE) ? SHELLBUF_SIZE : size;
	for (uint32_t i = 0; i < size; i++) {
		SHELL_PRINTF(" ");
	}
	SHELL_PRINTF("\r"SHELL_SIGN" ");
}


/**
 * @brief Set history command to historybuf
 */
static void yysh_set_history(uint8_t* content)
{
	strcpy(historybuf[history_index++], content);
	if (history_index >= HISTORY_NUMS) {
		history_index = 0;
	}
}

/**
 * @brief Get history command
 */
static void yysh_get_history(uint8_t* recvbuf, ARROW_KEYS_E mode)
{
	if (hist_get_index == -1) {
		hist_get_index = history_index;
	}

	int8_t _hist_index;
	if (mode == PgUp) {
		_hist_index = (hist_get_index == 0) ? (HISTORY_NUMS - 1) : (hist_get_index - 1);

	}
	else {
		_hist_index = (hist_get_index == HISTORY_NUMS) ? (0) : (hist_get_index + 1);
	}

	// if around
	if (_hist_index != history_index) {
		hist_get_index = _hist_index;
	}

	strcpy(recvbuf, historybuf[hist_get_index]);
}



