/*
 * Command to check if a mv88e61xx switch is plugged into itself
 */
#include <asm-generic/errno.h>
#include <common.h>
#include <command.h>
#include <linux/ctype.h>
#include <linux/mii.h>
#include <netdev.h>
#include <miiphy.h>
#include <marvell.h>
#include <i2c.h>
#include <crc.h>
#include "board.h"

/* in practice, autonegotiation takes about 3 seconds */
#define AUTONEG_TIMEOUT		(4 * CONFIG_SYS_HZ)

/* Set the 100/10 Half/Full advertisement bits for a switch port. */
static int autoneg_advertise_bits(struct mv88e61xx_config* swcfg, u32 port,
				  int bits)
{
	int err = 0;
	u16 val;

	err = mv88e61xx_phy_read(swcfg, port, MII_ADVERTISE, &val);
	if (err < 0)
		return err;
	val = (val & 0xfe1f) | (bits << 5);
	err = mv88e61xx_phy_write(swcfg, port, MII_ADVERTISE, val);

	return err;
}

/* Restart autonegotiation for a particular port */
static int autoneg_restart(struct mv88e61xx_config* swcfg, u32 port)
{
	int err = 0;
	u16 val;

	err = mv88e61xx_phy_read(swcfg, port, MII_BMCR, &val);
	if (err < 0)
		return err;
	val |= (BMCR_ANENABLE | BMCR_ANRESTART);
	err = mv88e61xx_phy_write(swcfg, port, MII_BMCR, val);

	return err;
}

/* Get the bits from the link partner. */
static int autoneg_partner_bits(struct mv88e61xx_config* swcfg, u32 port,
				int *bits)
{
	int err = 0;
	u16 val;

	err = mv88e61xx_phy_read(swcfg, port, MII_LPA, &val);
	if (err < 0)
		return err;
	*bits = (val & 0x01e0) >> 5;

	return err;
}

/* Wait for autonegotiation to complete. */
static int autoneg_wait(struct mv88e61xx_config* swcfg, u32 port1, u32 port2)
{
	u16 port1state;
	u16 port2state;
	ulong start;
	ulong now;
	int err;

	start = get_timer(0);
	for (;;) {
		now = get_timer(start);

		err = mv88e61xx_phy_read(swcfg, port1, MII_BMSR, &port1state);
		if (err < 0)
			return err;
		err = mv88e61xx_phy_read(swcfg, port2, MII_BMSR, &port2state);
		if (err < 0)
			return err;

		if ((port1state & BMSR_ANEGCOMPLETE) &&
		    (port2state & BMSR_ANEGCOMPLETE))
			return 0;

		if (now > AUTONEG_TIMEOUT)
			return -ETIMEDOUT;

		udelay(10);
	}
}

static int autoneg_exchange(struct mv88e61xx_config* swcfg, u32 port1,
			    u32 port2, int bits1, int bits2)
{
	int actual_bits1;
	int actual_bits2;
	int err;

	err = autoneg_advertise_bits(swcfg, port1, bits1);
	if (err < 0)
		return err;
	err = autoneg_advertise_bits(swcfg, port2, bits2);
	if (err < 0)
		return err;

	err = autoneg_restart(swcfg, port1);
	if (err < 0)
		return err;
	err = autoneg_restart(swcfg, port2);
	if (err < 0)
		return err;

	/* wait for autonegotiation to complete; can return -ETIMEDOUT */
	err = autoneg_wait(swcfg, port1, port2);
	if (err < 0)
		return err;

	err = autoneg_partner_bits(swcfg, port1, &actual_bits2);
	if (err < 0)
		return err;
	err = autoneg_partner_bits(swcfg, port2, &actual_bits1);
	if (err < 0)
		return err;

	return !(bits1 == actual_bits1 && bits2 == actual_bits2);
}

/*
 * Examine the current state of a port. Assuming the default configuration
 * on the switch, if a port is self-looped, then it will have link up and
 * have negotiated to gigabit.
 */
static int possibly_selflooped(struct mv88e61xx_config* swcfg, u32 port)
{
	const u16 expected_mask = (MIIM_88E1xxx_PHYSTAT_GBIT |
				   MIIM_88E1xxx_PHYSTAT_SPDDONE |
				   MIIM_88E1xxx_PHYSTAT_LINK);
	u16 status;
	int err;

	err = mv88e61xx_phy_read(swcfg, port, MIIM_88E1xxx_PHY_STATUS,
				 &status);
	if (err < 0)
		return err;

	return ((status & expected_mask) == expected_mask);
}

/*
 * Generate a random pattern. We use both the serial number (fixed,
 * but should differ per device) and the LSBs from the temp sensor
 * as entropy input.
 */
static u32 random_pattern(void)
{
	const uchar tmp451 = 0x4C;
	int i;
	union {
		struct {
			u32 serial;
			u16 noise;
			u16 timer;
		};
		unsigned char bytes[8];
	} entropy;
	u32 pattern;

	/* Set the fastest conversion rate (32/sec) */
	i2c_reg_write(tmp451, 0x0A, 0x09);
	/* Disable the digital filter */
	i2c_reg_write(tmp451, 0x24, 0x00);
	/* Configure continuous conversion, mask ~ALERT */
	i2c_reg_write(tmp451, 0x09, 0x80);

	/*
	 * Get the serial number, if set. If it's not set, then just pick
	 * an arbitrary value.
	 */
	if (getenv("serial#") != NULL)
		entropy.serial = simple_strtoul(getenv("serial#"), NULL, 16);
	else
		entropy.serial = 0xCAB00D1E; /* just pick something */

	/*
	 * Gather some entropy from the low-order bits from temperature
	 * sensor readings. We get four bits per iteration, sixteen total.
	 */
	for (i = 0; i < 4; ++i) {
		u8 remote_lo;
		u8 local_lo;

		/* Each conversion takes 0.03125 s */
		udelay(31250);

		remote_lo = i2c_reg_read(tmp451, 0x10);
		local_lo = i2c_reg_read(tmp451, 0x15);

		/*
		 * The remote reading is the noisiest, so grab the bottom 3
		 * LSBs from that reading. Mix in the bottom LSB of the
		 * local reading.
		 */
		entropy.noise = (entropy.noise << 4) |
				((remote_lo & 0x70) >> 4) |
				((local_lo & 0x10) >> 1);
	};

	/*
	 * Between the first autonegotiation and I2C latencies there's some
	 * timing varation in how long it takes us to get here.
	 * Mix the low-order bits of our current timer counter in too.
	 */
	entropy.timer = (u16)get_timer(0);

	/*
	 * Hash the input buffer to try and get an even distribution of
	 * values out.
	 */
	pattern = crc32(0, entropy.bytes, 8);

	/*
	 * There are some patterns that we *don't* want; specifically,
	 * we want to try and avoid bytes where both nibbles are 0xf;
	 * if both ports are using 0xf as the pattern that doesn't look
	 * any different from the "typical" external device that wants
	 * to autonegotiate all 10 and 100MBps modes. So, reroll (just
	 * using the standard random number generator) if we end up with
	 * one of those patterns.
	 */
	while (((pattern & 0xFF000000) == 0xFF000000) ||
	       ((pattern & 0x00FF0000) == 0x00FF0000) ||
	       ((pattern & 0x0000FF00) == 0x0000FF00) ||
	       ((pattern & 0x000000FF) == 0x000000FF))
	{
		pattern = rand_r(&pattern);
	}

	return pattern;
}

/*
 * Check for a self-loop between two ports of the switch.
 * Returns 0 if no loop, 1 if loop present, negative value on error.
 */
int switch_check_self_loop(struct mv88e61xx_config *swcfg, u32 prt1, u32 prt2)
{
	u32 pattern;
	u16 port1state;
	u16 port2state;
	int i;
	int err;
	int hasselfloop;

	/* Make sure ports are valid */
	if (!(swcfg->ports_enabled & (1 << prt1)) ||
	    !(swcfg->ports_enabled & (1 << prt2))) {
		printf("ports not enabled\n");
		return 0;
	}

	/*
	 * In the typical path, we check very shortly after initializing the
	 * switch for the first time; thus we have to wait for negotiation to
	 * complete the first time, or for it to time out (which would indicate
	 * that one or both ports are disconnected).
	 */
	err = autoneg_wait(swcfg, prt1, prt2);
	if (err == -ETIMEDOUT) {
		printf("links did not autonegotiate\n");
		return 0;
	} else if (err < 0) {
		return err;
	}

	/*
	 * Look at the current state. Assuming the default configuration on
	 * the switch, if we're self-looped, then both devices will have link
	 * up, and we'll have autonegotiated to gigabit.
	 */
	if (!possibly_selflooped(swcfg, prt1) ||
	    !possibly_selflooped(swcfg, prt2)) {
		printf("link not correct for both ports\n");
		return 0;
	}

	/*
	 * A multi-port device will prefer to be master. A single-port device
	 * will prefer to be slave. If we are connected to ourselves, one port
	 * must be a master and one port must be a slave. It can't be the case
	 * that both are masters or both are slaves.
	 */
	err = mv88e61xx_phy_read(swcfg, prt1, MII_STAT1000, &port1state);
	if (err < 0)
		return err;
	err = mv88e61xx_phy_read(swcfg, prt2, MII_STAT1000, &port2state);
	if (err < 0)
		return err;
	if ((port1state & PHY_1000BTSR_MSCR) == (port2state & PHY_1000BTSR_MSCR)) {
		printf("master/slave relationship precludes self-loop (%c/%c)\n",
			(port1state & PHY_1000BTSR_MSCR) ? 'm' : 's',
			(port2state & PHY_1000BTSR_MSCR) ? 'm' : 's');
		return 0;
	}

	/*
	 * Generate the random pattern. This takes a bit of time due to
	 * collecting some entropy from the temperature sensor, so we want
	 * to do it after we've ruled out some of the simple cases above.
	 */
	pattern = random_pattern();

	/*
	 * For each byte of the pattern, exchange the high and low
	 * nibbles. If we are our own link partner, then we should get the
	 * result from the opposite port. If we're not our own link partner,
	 * then we'll just get whatever the link partner advertises (usually
	 * that they support all of 10/100 Half/Full)
	 */
	for (i = 0; i < sizeof(u32); ++i) {
		int bits1 = (pattern >> ((i*8))) & 0xf;
		int bits2 = (pattern >> ((i*8)+4)) & 0xf;

		err = autoneg_exchange(swcfg, 1, 2, bits1, bits2);
		/* If err < 0, fatal error. If err > 0, no match. */
		if (err != 0)
			break;
	}

	if (err < 0) {
		printf("Got error %d during self-loop test\n", err);
	} else if (err > 0) {
		printf("No self-loop\n");
		hasselfloop = 0;
	} else {
		printf("Self-loop detected\n");
		hasselfloop = 1;
	}

	/* Reset the state of the ports */
	err = autoneg_advertise_bits(swcfg, 1, 0xf);
	if (err < 0)
		return err;
	err = autoneg_advertise_bits(swcfg, 2, 0xf);
	if (err < 0)
		return err;
	err = autoneg_restart(swcfg, 1);
	if (err < 0)
		return err;
	err = autoneg_restart(swcfg, 2);
	if (err < 0)
		return err;

	return hasselfloop;
}

int do_switchloop(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	u32 port1 = 1;
	u32 port2 = 2;
	int ret;

	if (argc > 1)
		port1 = simple_strtoul(argv[1], NULL, 10);
	if (argc > 2)
		port2 = simple_strtoul(argv[2], NULL, 10);

	ret = switch_check_self_loop(&bluefin_topaz_config, port1, port2);

	if (ret == 1)
		return CMD_RET_SUCCESS;
	else
		return CMD_RET_FAILURE;
}

U_BOOT_CMD(
	switchloop,   3,   0,   do_switchloop,
	"check for ethernet switch self-loop.",
	"<port1> <port2>\n"
	" port1 - index of first port\n"
	" port2 - index of second port"
);
