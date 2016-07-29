/*
 * Command to configure LEDs.
 */
#include "led.h"
#include <asm/gpio.h>

static int led_values[2] = {0, 0};

int configure_led(led_id id, led_action action)
{
	int value;

	if (action == LED_ACTION_ON)
		value = 1;
	else if (action == LED_ACTION_OFF)
		value = 0;
	else if (action == LED_ACTION_TOGGLE)
		value = !led_values[id];

	if (id == LED_ID_STATUS)
	{
#if CONFIG_STATUS_LED_GPIO
		gpio_direction_output(CONFIG_STATUS_LED_GPIO, value);
#endif
	}
	else if (id == LED_ID_ACTIVE)
	{
#if CONFIG_ACTIVE_LED_GPIO
		gpio_direction_output(CONFIG_ACTIVE_LED_GPIO, value);
#endif
	}

	led_values[id] = value;

	return 0;
}

int do_led (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	led_action action;
	led_id id;

	if (argc != 3)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "status") == 0)
		id = LED_ID_STATUS;
	else if (strcmp(argv[1], "active") == 0)
		id = LED_ID_ACTIVE;
	else
		return CMD_RET_USAGE;

	if (strcmp(argv[2], "on") == 0)
		action = LED_ACTION_ON;
	else if (strcmp(argv[2], "off") == 0)
		action = LED_ACTION_OFF;
	else if (strcmp(argv[2], "toggle") == 0)
		action = LED_ACTION_TOGGLE;
	else
		return CMD_RET_USAGE;

	return configure_led(id, action);
}

U_BOOT_CMD(
	led, 3, 1, do_led,
	"led configuration",
	"<status|active> <on|off|toggle>\n"
);
