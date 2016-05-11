/*
 * Command to (re)configure the mv88e61xx switch
 */
#include <common.h>
#include <command.h>
#include <linux/ctype.h>
#include <netdev.h>

int do_switchcfg(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	struct mv88e61xx_config switchcfg = {
		.name = "zynq_gem0",
		.vlancfg = MV88E61XX_VLANCFG_DEFAULT, /*ROUTER,*/
		.rgmii_delay = MV88E61XX_RGMII_DELAY_EN,
		.led_init = MV88E61XX_LED_INIT_EN,
		.mdip = MV88E61XX_MDIP_REVERSE,
		.portstate = MV88E61XX_PORTSTT_FORWARDING,
		.cpuport = (1 << 0),
		.ports_enabled = 0x07
	};

	switchcfg.name = "zynq_gem0";
	if (argc < 1 || (strncmp(argv[1], "default", 6) == 0)) {
		switchcfg.vlancfg = MV88E61XX_VLANCFG_DEFAULT;
	} else {
		switchcfg.vlancfg = MV88E61XX_VLANCFG_ROUTER;
	}

	if (argc < 2) {
		switchcfg.cpuport = (1 << 0);
	} else {
		switchcfg.cpuport = (1 << simple_strtoul(argv[2], NULL, 0));
	}

	if (argc < 3) {
		switchcfg.ports_enabled = 0x07;
	} else {
		switchcfg.ports_enabled = simple_strtoul(argv[3], NULL, 0);
	}

        mv88e61xx_switch_initialize(&switchcfg);

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

