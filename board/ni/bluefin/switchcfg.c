/*
 * Command to (re)configure the mv88e61xx switch
 */
#include <common.h>
#include <command.h>
#include <linux/ctype.h>
#include <netdev.h>
#include "board.h"

int do_switchcfg(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 1 || (strncmp(argv[1], "default", 6) == 0)) {
		bluefin_topaz_config.vlancfg = MV88E61XX_VLANCFG_DEFAULT;
	} else {
		bluefin_topaz_config.vlancfg = MV88E61XX_VLANCFG_ROUTER;
	}

	if (argc < 2) {
		bluefin_topaz_config.cpuport = (1 << 0);
	} else {
		bluefin_topaz_config.cpuport = (1 << simple_strtoul(argv[2], NULL, 0));
	}

	if (argc < 3) {
		bluefin_topaz_config.ports_enabled = PORTINDEX(2)|PORTINDEX(1)|PORTINDEX(0);
	} else {
		bluefin_topaz_config.ports_enabled = simple_strtoul(argv[3], NULL, 0);
	}

	mv88e61xx_switch_initialize(&bluefin_topaz_config);

	return 0;
}

U_BOOT_CMD(
	switchcfg,   4,   0,   do_switchcfg,
	"configure the switch.",
	"<role> <cpuport> <portsenabled>\n"
	"role           - 'default' or 'router'\n"
	"cpuport        - which port to use for cpu (default 0)\n"
	"portsenabled   - bitmask for enabled ports (default 0x07)"
);

