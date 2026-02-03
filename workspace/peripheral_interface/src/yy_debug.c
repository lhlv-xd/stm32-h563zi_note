/*
 * yy_debug.c
 *
 *  Created on: Feb 3, 2026
 *      Author: Yuyi_Chen
 */


#include "yy_debug.h"

// Change the level you hope.
static int current_debug_level = YY_DEBUG_LEVEL_DEBUG;

/***********************************************************************************************
 * Customization
 **********************************************************************************************/

void yy_debug_message(int level, const char *fmt, ...)
{
    if (level <= current_debug_level) {
    	char buf[1024];

		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);

		printf("%s\r\n", buf);
    }
}
