/*
 * Just to satisfy init routines..
 */

#include <common.h>
#include <asm/arch/nand.h>
#include <asm/arch/sdhci.h>
#include <asm/io.h>
#include <environment.h>
#include <i2c.h>
#include <miiphy.h>
#include <netdev.h>
#include <zynqpl.h>

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
Xilinx_desc fpga = XILINX_XC7Z020_DESC(0);
#endif

int board_init(void)
{
	/* temporary hack to clear pending irqs before Linux as it 
	   will hang Linux */

	out_le32(0xe0001014, 0x26d);

	/* temporary hack to take USB out of reset til the is fixed
	   in Linux */

	out_le32(0xe000a204, 0x80);
	out_le32(0xe000a208, 0x80);
	out_le32(0xe000a040, 0x80);
	out_le32(0xe000a040, 0x00);
	out_le32(0xe000a040, 0x80);

	icache_enable();

#ifdef CONFIG_FPGA
	fpga_init();
	fpga_add(fpga_xilinx, &fpga);
#endif

	return 0;
}

int board_late_init (void)
{
#ifdef CONFIG_BRINGUP
	char serial[11] = "";

	if (getenv("serial#") != NULL)
		strcpy(serial, getenv("serial#"));
	set_default_env("Default env required for auto-bringup.\n");
	if (strlen(serial) != 0)
		setenv("serial#", serial);
#endif

	return 0;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int retval;
	u8 tmp;
	int phy_addr;
	const char *name = "zynq_gem0";
	u16 regval;

	/*
	 * Take eth0 phy, eth1 phy, usb phy, usb hub, and external UART
	 * out of reset
	 */
	tmp = 0x00;
	i2c_write(0x40, 0x03, 1, &tmp, 1);

	retval = zynq_gem_initialize(bis);

	/* nizynqproto has a Marvell Gigabit PHY on gem0 */
	phy_addr = zynq_gem_get_phyaddr(name);

	/* Page 2 */
	miiphy_write(name, phy_addr, 22, 2);

	/* Disable the unused 125 MHz clock from the phy */
	miiphy_write(name, phy_addr, 16, 0x444E);

	/* Override the primary phy's drive strength to maximum */
	miiphy_write(name, phy_addr, 24, 0xBF4F);

	/* Page 0 */
	miiphy_write(name, phy_addr, 22, 0);

	/* Copper specific control register 1 */
	miiphy_read(name, phy_addr, 16, &regval);
	regval |= (7 << 12); /* max number of gigabit attempts */
	regval |= (1 << 11); /* enable downshift */
	miiphy_write(name, phy_addr, 16, regval);

	return retval;
}
#endif

#ifdef CONFIG_CMD_MMC
int board_mmc_init(bd_t *bd)
{
	return zynq_sdhci_init(XPSS_SDIO0_BASEADDR, 100000000, 0, SDHCI_QUIRK_NO_CD);
}
#endif

#ifdef CONFIG_CMD_NAND
int board_nand_init(struct nand_chip *nand_chip)
{
	return zynq_nand_init(nand_chip);
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
