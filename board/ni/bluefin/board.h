/*
 * Bluefin-related board stuff shared between compilation units.
 */

#ifndef _BLUEFIN_BOARD_H_
#define _BLUEFIN_BOARD_H_

#include <common.h>
#include <linux/ctype.h>
#include <netdev.h>

extern struct mv88e61xx_config bluefin_topaz_config;
#define PORTINDEX(x)    (1 << x)

#endif
