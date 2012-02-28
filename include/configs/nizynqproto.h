/*
 * Hacked together,
 * hopefully functional.
 *
 * See zynq_common.h for Zynq common configs
 */

#ifndef __CONFIG_NIZYNQPROTO_H
#define __CONFIG_NIZYNQPROTO_H

/*
 * High Level Configuration Options
 */
#define CONFIG_NIZYNQPROTO /* Board */

#include <configs/zynq_common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_SYS_NO_FLASH

#include <config_cmd_default.h>
#undef CONFIG_CMD_XIMG
#define CONFIG_FIT
//#define CONFIG_FIT_VERBOSE /* enable fit_format_{error,warning}() */
#define CONFIG_CMD_DATE		/* RTC? */
#define CONFIG_CMD_PING		/* Might be useful for debugging */
#define CONFIG_REGINFO		/* Again, debugging */
#undef CONFIG_CMD_SETGETDCR	/* README says 4xx only */
/* check for input to stop even if delay is 0 */
#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "hush> "

#define CONFIG_CMD_UNZIP
#define CONFIG_CMD_SETEXPR
#define CONFIG_CRC32_VERIFY
#define CONFIG_CMD_MD5SUM
#define CONFIG_MD5SUM_VERIFY
#define CONFIG_CMD_SHA1SUM
#define CONFIG_SHA1SUM_VERIFY

#define CONFIG_TIMESTAMP	/* print image timestamp on bootm, etc */

#define CONFIG_IDENT_STRING	"\nNational Instruments Zynq Prototype"
#define CONFIG_PANIC_HANG /* For development/debugging */

#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT	"ni-zynq> "

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING

#define CONFIG_ENV_IS_NOWHERE

/* HW to use */
#define CONFIG_ZYNQ_UART1
#define CONFIG_ZYNQ_GEM
#define CONFIG_NET_MULTI
#define CONFIG_XGMAC_PHY_ADDR 0
#define CONFIG_CMD_MII
#define CONFIG_MII
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_LINK_LOCAL
#define CONFIG_BOOTP_SERVERIP
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_NETCONSOLE

#define CONFIG_CMD_I2C
/* not really used */
#define CONFIG_SYS_I2C_SPEED 400
#define CONFIG_HARD_I2C
#define CONFIG_ZYNQ_I2C
#define CONFIG_ZYNQ_I2C_CTLR_0

/*
 * Physical Memory map
 */
#define PHYS_SDRAM_1_SIZE (512 * 1024 * 1024)

/*
 * NAND Flash settings
 */
#define CONFIG_NAND_ZYNQ
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_SYS_MAX_NAND_DEVICE 1
#define CONFIG_SYS_NAND_BASE XPSS_NAND_BASEADDR
#define CONFIG_MTD_DEVICE
//#define CONFIG_MTD_DEBUG
//#define CONFIG_MTD_DEBUG_VERBOSE 3

/* Secure Digital */
#define CONFIG_MMC

#ifdef CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_SDHCI
#define CONFIG_MMC_SDMA
#define CONFIG_ZYNQ_SDHCI
#define CONFIG_CMD_MMC
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_DOS_PARTITION
#endif

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"autoload=n\0" \
	"ncoutport=7865\0" \
	"ncinport=8473\0" \
	"ncip=255.255.255.255\0" \
	"nc=setenv stdout nc;setenv stdin nc\0" \
	"sc=setenv stdout serial;setenv stdin serial\0" \
	"fdt_high=0x17ffffff\0" \
	"initrd_high=0x17ffffff\0" \
	"kernel_size=0x140000\0"	\
	"ramdisk_size=0x200000\0"	\
	"nand_kernel_size=0x400000\0"	\
	"nand_ramdisk_size=0x400000\0"	\
	"norboot=echo Copying Linux from NOR flash to RAM...;" \
		"cp 0xE2100000 0x8000 ${kernel_size};" \
		"cp 0xE2600000 0x1000000 0x8000;" \
		"echo Copying ramdisk...;" \
		"cp 0xE3000000 0x800000 ${ramdisk_size};" \
		"go 0x8000\0" \
	"qspiboot=echo Copying Linux from QSPI flash to RAM...;" \
		"cp 0xFC100000 0x8000 ${kernel_size};" \
		"cp 0xFC600000 0x1000000 0x8000;" \
		"echo Copying ramdisk...;" \
		"cp 0xFC800000 0x800000 ${ramdisk_size};" \
		"go 0x8000\0" \
	"sdboot=echo Copying Linux from SD to RAM...;" \
		"mmcinfo;" \
		"fatload mmc 0 0x8000 zImage;" \
		"fatload mmc 0 0x1000000 devicetree.dtb;" \
		"fatload mmc 0 0x800000 ramdisk8M.image.gz;" \
		"go 0x8000\0" \
	"nandboot=echo Copying Linux from NAND flash to RAM...;" \
		"nand read 0x8000 0x200000 ${nand_kernel_size};" \
		"nand read 0x1000000 0x700000 0x20000;" \
		"echo Copying ramdisk...;" \
		"nand read 0x800000 0x900000 ${nand_ramdisk_size};" \
		"go 0x8000\0" \
	"jtagboot=echo TFTPing Linux to RAM...;" \
		"tftp 0x8000 zImage;" \
		"tftp 0x1000000 devicetree.dtb;" \
		"tftp 0x800000 ramdisk8M.image.gz;" \
		"go 0x8000\0"

#endif /* __CONFIG_NIZYNQPROTO_H */
