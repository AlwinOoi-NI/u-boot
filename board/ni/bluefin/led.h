/*
 * Bluefin LEDs.
 */

#ifndef _BLUEFIN_LED_H_
#define _BLUEFIN_LED_H_

#include <common.h>
#include <command.h>
#include <linux/ctype.h>

typedef enum {
   LED_ACTION_ON,
   LED_ACTION_OFF,
   LED_ACTION_TOGGLE
} led_action;

typedef enum {
   LED_ID_STATUS,
   LED_ID_ACTIVE
} led_id;

int configure_led(led_id id, led_action action);

#endif
