/*
 * yy_debug.h
 *
 *  Created on: Feb 3, 2026
 *      Author: Yuyi_Chen
 */

#ifndef PERIPHERAL_INTERFACE_INC_YY_DEBUG_H_
#define PERIPHERAL_INTERFACE_INC_YY_DEBUG_H_

#include "yy_hal_peripheral_interface.h"
#include "stdarg.h"

/* Prototype */
void yy_debug_message(int level, const char *fmt, ...);

/* Defined */
#define YY_DEBUG_LEVEL_NONE       0
#define YY_DEBUG_LEVEL_ERROR      1
#define YY_DEBUG_LEVEL_WARNING    2
#define YY_DEBUG_LEVEL_INFO       3
#define YY_DEBUG_LEVEL_DEBUG      4

#define yy_err(fmt, ...)   yy_debug_message(YY_DEBUG_LEVEL_ERROR,    "[ERR] %s():%d "  fmt, __func__, __LINE__, ##__VA_ARGS__)
#define yy_warn(fmt, ...)   yy_debug_message(YY_DEBUG_LEVEL_WARNING, "[WARN] %s():%d " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define yy_info(fmt, ...)   yy_debug_message(YY_DEBUG_LEVEL_INFO,    "[INFO] %s():%d " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define yy_debug(fmt, ...)   yy_debug_message(YY_DEBUG_LEVEL_DEBUG,  "[DBG] %s():%d "  fmt, __func__, __LINE__, ##__VA_ARGS__)


#endif /* PERIPHERAL_INTERFACE_INC_YY_DEBUG_H_ */
