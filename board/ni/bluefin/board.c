/*
 * Just to satisfy init routines..
 */

#include <common.h>
#include <asm/arch/nand.h>
#include <asm/arch/sdhci.h>
#include <asm/io.h>
#include <environment.h>
#include <jffs2/load_kernel.h>
#include <miiphy.h>
#include <netdev.h>
#include <serial.h>
#include <zynqpl.h>
#include <usb/ulpi.h>
#include <asm/gpio.h>
#include "led.h"

#define BOOT_MODE_REG     (XPSS_SYS_CTRL_BASEADDR + 0x25C)
#define BOOT_MODES_MASK    0x0000000F
#define QSPI_MODE         (0x00000001)            /**< QSPI */
#define NOR_FLASH_MODE    (0x00000002)            /**< NOR  */
#define NAND_FLASH_MODE   (0x00000004)            /**< NAND */
#define SD_MODE           (0x00000005)            /**< Secure Digital card */
#define JTAG_MODE	  (0x00000000)		  /**< JTAG */

DECLARE_GLOBAL_DATA_PTR;

/* Common IO for xgmac and xnand */
/* Data Memory Barrier */
#define SYNCHRONIZE_IO dmb()

void XIo_Out32(u32 OutAddress, u32 Value)
{
	*(volatile u32 *) OutAddress = Value;
	SYNCHRONIZE_IO;
}

u32 XIo_In32(u32 InAddress)
{
	volatile u32 temp = *(volatile u32 *)InAddress;
	SYNCHRONIZE_IO;
	return temp;
}

#ifdef CONFIG_FPGA
Xilinx_desc fpga = XILINX_XC7Z010_DESC(0);
#endif

struct serial_device *default_serial_console(void)
{
	return &null_serial_device;
}

#if !defined(CONFIG_MFG)
static int reset_button_down()
{
	/* Reset button is active-low. */
	return gpio_get_value(CONFIG_RESET_BUTTON_GPIO) == 0;
}

/*
 * Check the state of the reset button. Will also wait for up to
 * 5 seconds to determine if we should force recovery mode.
 *
 * Sets the "bootmode" environment variable.
 */
static void check_reset_button(void)
{
	ulong start;
	ulong holdtime = 0;
	ulong maxholdtime = 5 * CONFIG_SYS_HZ;

	gpio_direction_input(CONFIG_RESET_BUTTON_GPIO);

	start = get_timer(0);

	while (reset_button_down()) {
		holdtime = get_timer(start);

		if (holdtime > maxholdtime) {
			/* start blinking */
			configure_led(LED_ID_STATUS, LED_ACTION_BLINK);
			break;
		}

		udelay(100);
	}

	/* spin until user lets go of the button */
	while (reset_button_down())
		udelay(100);

	if (holdtime > maxholdtime)
		setenv("bootmode", "recovery");
	else
		setenv("bootmode", "normal");
}
#endif

int board_init(void)
{
	led_init();

#ifdef CONFIG_FPGA
	fpga_init();
	fpga_add(fpga_xilinx, &fpga);
#endif

	return 0;
}

#if defined(CONFIG_USE_IRQ)

/*
 * This really should live in zynq_gpio, except the u-boot gpio
 * framework doesn't really have infrastructure for interrupt-based
 * triggering, either in the version we have this developed against
 * (2012.10) or what is currently upstream (2016.09). In order to
 * ease future merges, this is separate for now.
 */
#define GPIO_INT_MASK_1 (XPSS_GPIO_BASEADDR + 0x024C)
#define GPIO_INT_EN_1   (XPSS_GPIO_BASEADDR + 0x0250)
#define GPIO_INT_DIS_1  (XPSS_GPIO_BASEADDR + 0x0254)
#define GPIO_INT_STAT_1 (XPSS_GPIO_BASEADDR + 0x0258)
#define GPIO_INT_TYPE_1 (XPSS_GPIO_BASEADDR + 0x025C)
#define GPIO_INT_POL_1  (XPSS_GPIO_BASEADDR + 0x0260)
#define GPIO_INT_ANY_1  (XPSS_GPIO_BASEADDR + 0x0264)

/* Everything assumes bank 1 because assumptions make code easier! */
#if !(CONFIG_RESET_BUTTON_GPIO >= 32 && CONFIG_RESET_BUTTON_GPIO <= 63)
	#error CONFIG_RESET_BUTTON_GPIO expected to be on MIO Bank 1
#endif

/* line index within bank register */
#define RESET_BUTTON_LINE	(CONFIG_RESET_BUTTON_GPIO % 32)

static void gpio_irq_handler(void *unused)
{
	u32 mask;

	mask = readl(GPIO_INT_STAT_1);
	if (mask & (1U << RESET_BUTTON_LINE)) {
		if (gpio_get_value(CONFIG_RESET_BUTTON_GPIO) == 0) {
			disable_interrupts();
			reset_cpu(0);
		}
	}

	/* acknowledge everything on bank 1 */
	writel(mask, GPIO_INT_STAT_1);
}

static int enable_reset_button_int(void)
{
	u32 tmp;

	irq_install_handler(XPSS_GPIO_INT_ID, gpio_irq_handler, NULL);

	/* Reset button is an input. */
	gpio_direction_input(CONFIG_RESET_BUTTON_GPIO);

	/* set edge-sensitive (1) */
	tmp = readl(GPIO_INT_TYPE_1);
	tmp |= (1U << RESET_BUTTON_LINE);
	writel(tmp, GPIO_INT_TYPE_1);

	/* set edge sensitivity (single edge) */
	tmp = readl(GPIO_INT_ANY_1);
	tmp &= ~(1U << RESET_BUTTON_LINE);
	writel(tmp, GPIO_INT_ANY_1);

	/* set polarity (falling edge) */
	tmp = readl(GPIO_INT_POL_1);
	tmp &= ~(1U << RESET_BUTTON_LINE);
	writel(tmp, GPIO_INT_POL_1);

	/* enable interrupt */
	tmp = readl(GPIO_INT_EN_1);
	tmp |= (1U << RESET_BUTTON_LINE);
	writel(tmp, GPIO_INT_EN_1);

	return 0;
}

#endif /* defined(CONFIG_USE_IRQ) */

int board_late_init (void)
{
#if defined(CONFIG_MFG)
	char serial[12] = "";
	char ethaddr[20] = "";

	/* Preserve serial# and ethaddr if possible. */
	if (getenv("serial#") != NULL)
		strncat(serial, getenv("serial#"), sizeof(serial));
	if (getenv("ethaddr") != NULL)
		strncat(ethaddr, getenv("ethaddr"), sizeof(ethaddr));

	set_default_env("Default env required for auto-bringup.\n");

	if (strlen(serial) != 0)
		setenv("serial#", serial);
	if (strlen(ethaddr) != 0)
		setenv("ethaddr", ethaddr);
#endif

#if !defined(CONFIG_MFG)
	/* Figure out if we should boot normally or do recovery. */
	check_reset_button();
#endif

#if defined(CONFIG_USE_IRQ)
	enable_reset_button_int();
#endif

	return 0;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int retval;

	printf("board_eth_init: initializing zynq gem\n");

	retval = zynq_gem_initialize(bis);

	return retval;
}
#endif

#ifdef CONFIG_CMD_NAND

#ifndef CONFIG_SYS_NAND_SELF_INIT
#error CONFIG_SYS_NAND_SELF_INIT is required!
#endif

#if CONFIG_SYS_MAX_NAND_DEVICE > 1
#error Only 1 NAND device is supported!
#endif

static struct nand_chip nand_chip;

void board_nand_init()
{
	struct mtd_info *mtd = &nand_info[0];
	loff_t off;
	loff_t size;
#if !defined(CONFIG_MFG)
	struct mtd_device *dev;
	u8 part_num;
	struct part_info *part;
#endif
	mtd->priv = &nand_chip;
	nand_chip.IO_ADDR_R = (void  __iomem *)CONFIG_SYS_NAND_BASE;
	nand_chip.IO_ADDR_W = (void  __iomem *)CONFIG_SYS_NAND_BASE;

	zynq_nand_init(&nand_chip);
	nand_register(0);

	/*
	 * Unlock the entire flash for manufacturing
	 */
#if defined(CONFIG_MFG)
	off = 0;
#else
	if (mtdparts_init() || find_dev_and_part("u-boot", &dev, &part_num, &part))
		off = CONFIG_MTD_UBOOT_OFFSET + CONFIG_BOARD_SIZE_LIMIT;
	else
		off = part->size + part->offset;
#endif
	size = nand_info[0].size - off;

	nand_unlock(&nand_info[0], off, size, 0);
}
#endif

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;

	return 0;
}

/*
 * OK, and resets too.
 */
void reset_cpu(ulong addr)
{
	/* unlock SLCR */
	out_le32(XPSS_SYS_CTRL_BASEADDR | XPSS_SLCR_UNLOCK, XPSS_SLCR_UNLOCK_KEY);
	/* Tickle soft reset bit */
	out_le32(XPSS_SYS_CTRL_BASEADDR | XPSS_SLCR_PSS_RST_CTRL, 1);

	while(1) {;}
}

#if defined(CONFIG_OF_BOARD_SETUP)
void ft_board_setup(void *blob, bd_t *bd)
{
	const char *product_id_str;
	u16 product_id_val;

	/* Add the product ID to the device tree */
	product_id_str = getenv("DeviceCode");
	if (product_id_str != NULL) {
		product_id_val = simple_strtoul(product_id_str, NULL, 0);
		fdt_find_and_setprop(blob, "/", "DeviceCode",
			&product_id_val, sizeof(product_id_val), 1);
	}
}
#endif

#if defined(CONFIG_MV88E61XX_SWITCH)
void reset_phy(void)
{
#define PORTINDEX(x)	(1 << x)
	/*
	 * Configure and initialize switch.
	 * Bluefin has port 0 connected to the CPU, and ports 1 and 2
	 * are exposed externally.
	 */
	struct mv88e61xx_config swcfg = {
		.name = "zynq_gem0",
		.vlancfg = MV88E61XX_VLANCFG_DEFAULT,
		.rgmii_delay = MV88E61XX_RGMII_DELAY_EN,
		.led_init = MV88E61XX_LED_INIT_DIS,
		.mdip = MV88E61XX_MDIP_NOCHANGE,
		.portstate = MV88E61XX_PORTSTT_FORWARDING,
		.rmgmt = MV88E61XX_RMGMT_EN,
		.cpuport = PORTINDEX(0),
		.ports_enabled = PORTINDEX(0)|PORTINDEX(1)|PORTINDEX(2)
	};

	mv88e61xx_switch_initialize(&swcfg);
}
#endif /* CONFIG_MV88E61XX_SWITCH */
