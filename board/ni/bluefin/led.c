/*
 * Command to configure LEDs.
 */
#include "led.h"
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch-zynq/ttc.h>

#if CONFIG_STATUS_LED_GPIO
	#define STATUS_LED_GPIO CONFIG_STATUS_LED_GPIO
#else
	#define STATUS_LED_GPIO -1
#endif

#if CONFIG_ACTIVE_LED_GPIO
	#define ACTIVE_LED_GPIO CONFIG_ACTIVE_LED_GPIO
#else
	#define ACTIVE_LED_GPIO -1
#endif

#define LED_ON			(1 << 0)
#define LED_BLINK		(1 << 1)

static volatile int led_values[NUM_LED_IDS] = {0, 0};
static const int led_gpio[NUM_LED_IDS] = { STATUS_LED_GPIO, ACTIVE_LED_GPIO };

/*
 * The blink pattern is three pulses 1/5th of a second in length.
 *
 * The TTC input is off of the CPU_1x clock. On Bluefin, the CPU_1x
 * clock runs at 111.111MHz. We set the prescaler to 2^12. This lets
 * us represent our longest interval, 1.8 seconds, with the lowest
 * amount of error, while not overflowing the 16-bit counter.
 */

#define PRESCALE_2_N		12	/* prescale by 2^12 */
#define COUNTS_0_2_SEC		5425	/* 0.2 s / (2^12 / 111MHz) */
#define COUNTS_1_8_SEC		48828	/* 1.8 s / (2^12 / 111MHz) */

/* TTC resource to use to control blinking */
#define BLINK_TIMER		0

static const u16 pattern[] = {
	COUNTS_0_2_SEC,	/* off */
	COUNTS_0_2_SEC,	/* on */
	COUNTS_0_2_SEC,	/* off */
	COUNTS_0_2_SEC,	/* on */
	COUNTS_1_8_SEC,	/* off */
	COUNTS_0_2_SEC,	/* on */
};
volatile int pattern_index = 0;

static void set_gpio_led(led_id which, int value)
{
	if (led_gpio[which] != -1) {
		gpio_direction_output(led_gpio[which], value);
	}
}

void ttc_interval_handler(void *unused)
{
	u32 tmp;
	int id;

	/* move to the next interval in the pattern */
	++pattern_index;
	if (pattern_index >= ARRAY_SIZE(pattern))
		pattern_index = 0;

	/* write the new interval */
	zynq_ttc_set_interval(BLINK_TIMER, pattern[pattern_index]);

	/* update LEDs that are in the blink state */
	for (id = 0; id < NUM_LED_IDS; ++id) {
		if (led_values[id] & LED_BLINK) {
			set_gpio_led(id, pattern_index&1);
		}
	}
}

static int is_blinking(void)
{
	int id;
	for (id = 0; id < NUM_LED_IDS; ++id) {
		if (led_values[id] & LED_BLINK) {
			return 1;
		}
	}
	return 0;
}

void start_blinking(void)
{
	zynq_ttc_stop(BLINK_TIMER);
	pattern_index = 0;

	zynq_ttc_set_prescaler(BLINK_TIMER, PRESCALE_2_N);
	zynq_ttc_set_interval(BLINK_TIMER, pattern[pattern_index]);
	zynq_ttc_set_interval_handler(BLINK_TIMER, ttc_interval_handler, NULL);
	zynq_ttc_start(BLINK_TIMER, ZYNQ_TTC_MODE_INTERVAL);
}

void stop_blinking(void)
{
	zynq_ttc_stop(BLINK_TIMER);
	zynq_ttc_set_interval_handler(BLINK_TIMER, NULL, NULL);
}

int led_init(void)
{
	/* Initialize timer in the stop state */
	zynq_ttc_stop(BLINK_TIMER);

	/* Status LED goes on while booting. */
	configure_led(LED_ID_STATUS, LED_ACTION_ON);
	/* Active LED goes off while booting. */
	configure_led(LED_ID_ACTIVE, LED_ACTION_OFF);

	return 0;
}

#if defined(CONFIG_FIELDDAQ)
/*
 * FieldDAQ has a bicolor status LED, so configure_led needs to handle some
 * special mapping to set up the two led_values slots.
 * STATUS_LED_GPIO -> Red Status LED
 * ACTIVE_LED_GPIO -> Green Status LED
 *
 * LED_ACTION_OFF   -> Both Off
 * LED_ACTION_ON    -> Both On ("yellow")
 * LED_ACTION_BLINK -> Blinking Red
 */
int configure_led(led_id id, led_action action)
{
	int was_blinking = is_blinking();
	int should_now_blink;

	if (id != LED_ID_STATUS)
		return 0;

	if (action == LED_ACTION_ON) {
		led_values[LED_ID_STATUS] = LED_ON;
		led_values[LED_ID_ACTIVE] = LED_ON;
	} else if (action == LED_ACTION_OFF) {
		led_values[LED_ID_STATUS] = 0;
		led_values[LED_ID_ACTIVE] = 0;
	} else if (action == LED_ACTION_TOGGLE) {
		led_values[LED_ID_STATUS] = !led_values[LED_ID_STATUS];
		led_values[LED_ID_ACTIVE] = !led_values[LED_ID_ACTIVE];
	} else if (action == LED_ACTION_BLINK) {
		led_values[LED_ID_STATUS] = LED_BLINK;
		led_values[LED_ID_ACTIVE] = 0;
	} else {
		printf("bad led action\n");
		return -1;
	}

	set_gpio_led(LED_ID_STATUS, led_values[LED_ID_STATUS] & LED_ON);
	set_gpio_led(LED_ID_ACTIVE, led_values[LED_ID_ACTIVE] & LED_ON);

	/* figure out if we need to change the timer state */
	should_now_blink = is_blinking();
	if (!was_blinking && should_now_blink)
		start_blinking();
	else if (was_blinking && !should_now_blink)
		stop_blinking();

	return 0;

}
#else
/*
 * Assumes a device with discrete "status" and "active" LEDs, like Bluefin.
 */
int configure_led(led_id id, led_action action)
{
	int value;
	int was_blinking = is_blinking();
	int should_now_blink;

	if (id >= NUM_LED_IDS) {
		printf("bad led id\n");
		return -1;
	}

	if (action == LED_ACTION_ON)
		value = LED_ON;
	else if (action == LED_ACTION_OFF)
		value = 0;
	else if (action == LED_ACTION_TOGGLE)
		value = !led_values[id];
	else if (action == LED_ACTION_BLINK)
		value = LED_BLINK;
	else {
		printf("bad led action\n");
		return -1;
	}

	set_gpio_led(id, value & LED_ON);
	led_values[id] = value;

	/* figure out if we need to change the timer state */
	should_now_blink = is_blinking();
	if (!was_blinking && should_now_blink)
		start_blinking();
	else if (was_blinking && !should_now_blink)
		stop_blinking();

	return 0;
}
#endif

int do_led (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	led_action action;
	led_id id;

	if (argc != 3)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "status") == 0)
		id = LED_ID_STATUS;
#if !defined(CONFIG_FIELDDAQ)
	else if (strcmp(argv[1], "active") == 0)
		id = LED_ID_ACTIVE;
#endif
	else
		return CMD_RET_USAGE;

	if (strcmp(argv[2], "on") == 0)
		action = LED_ACTION_ON;
	else if (strcmp(argv[2], "off") == 0)
		action = LED_ACTION_OFF;
	else if (strcmp(argv[2], "toggle") == 0)
		action = LED_ACTION_TOGGLE;
	else if (strcmp(argv[2], "blink") == 0)
		action = LED_ACTION_BLINK;
	else
		return CMD_RET_USAGE;

	return configure_led(id, action);
}

U_BOOT_CMD(
	led, 3, 1, do_led,
	"led configuration",
	"<status|active> <on|off|toggle|blink>\n"
);
