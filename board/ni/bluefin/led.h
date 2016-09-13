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
   LED_ACTION_TOGGLE,
   LED_ACTION_BLINK
} led_action;

typedef enum {
   LED_ID_STATUS,
   LED_ID_ACTIVE
} led_id;

#define NUM_LED_IDS 2

int led_init(void);
int configure_led(led_id id, led_action action);

#endif
