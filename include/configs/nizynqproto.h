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
#define CONFIG_DEVICE_CODE 7659

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

#define CONFIG_ENV_IS_IN_NAND
/* Writing to OOB 0 is apparently not compatible with BootROM */
//#define CONFIG_ENV_OFFSET_OOB
/* Not needed when OOB is used */
#define CONFIG_ENV_OFFSET		0xA0000
#define CONFIG_ENV_OFFSET_REDUND	0xE0000
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE			0x20000
#define CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE
#define CONFIG_ENV_RANGE		0x40000

#define CONFIG_CMD_SAVEENV	/* Command to save ENV to Flash */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_FPGA
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_ZYNQPL
#define CONFIG_CMD_FPGA

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
#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_SCRATCH 0xFFFFF000
#define CONFIG_CMD_CACHE
#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH

/*
 * NAND Flash settings
 */
#define CONFIG_NAND_ZYNQ
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_SYS_MAX_NAND_DEVICE 1
#define CONFIG_SYS_NAND_BASE XPSS_NAND_BASEADDR
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT "nand0=xilinx_nand"
#define MTDPARTS_DEFAULT \
	"mtdparts=xilinx_nand:" \
		"128k(fsbl)ro," \
		"512k(u-boot)ro," \
		"256k(u-boot-env1)," \
		"256k(u-boot-env2)," \
		"60M(boot-config)," \
		"-(root)"
//#define CONFIG_MTD_DEBUG
//#define CONFIG_MTD_DEBUG_VERBOSE 3

#define CONFIG_MTD_UBOOT_OFFSET		0x20000
#define CONFIG_BOARD_SIZE_LIMIT		524288
#define CONFIG_BOOTFS_VOLUME_SIZE	0x3600000

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

#undef CONFIG_SYS_LOAD_ADDR
#define CONFIG_SYS_LOAD_ADDR 0x8000000
#define CONFIG_LOADADDR CONFIG_SYS_LOAD_ADDR

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
	"TargetClass=cRIO\0" \
	"DeviceDesc=cRIO-Zynq\0" \
	"DeviceCode=0x" __stringify(CONFIG_DEVICE_CODE) "\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"verifyaddr=0x10000000\0" \
	"sdboot=echo Copying Safemode from SD to RAM...; " \
		"mmcinfo; " \
		"fatload mmc 0 $loadaddr linux_safemode.itb; " \
		"source $loadaddr:bootscript;\0" \
	"boot_safemode=" \
		"if ubifsload $loadaddr linux_safemode.itb && " \
		"imi $loadaddr; then " \
			"setenv verify n; " \
			"source $loadaddr:bootscript; " \
		"else " \
			"echo $safemode_err; " \
		"fi;\0" \
	"safemode_err=Failed to find a valid safemode image.\0" \
	"updateenv=env export -b $loadaddr; " \
		"env default -f; " \
		"env import -b $loadaddr;\0" \
	"writepartitions=" \
		"nand erase.part boot-config && " \
		"ubi part boot-config && " \
		"ubi create u-boot-env1 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
		"ubi create u-boot-env2 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
		"ubi create bootfs " __stringify(CONFIG_BOOTFS_VOLUME_SIZE) " dynamic && " \
		"ubi create config - dynamic && " \
		"nand erase.part root && " \
		"ubi part root && " \
		"ubi create rootfs - dynamic;\0" \
	"writeboot=nand erase 0 " __stringify(CONFIG_ENV_OFFSET) "; " \
		"nand write $loadaddr 0 " __stringify(CONFIG_ENV_OFFSET) "; " \
		"nand read $verifyaddr 0 " __stringify(CONFIG_ENV_OFFSET) "; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_ENV_OFFSET) ";\0" \
	"writefsbl=nand erase.part fsbl; " \
		"nand write $loadaddr fsbl; " \
		"nand read $verifyaddr fsbl; " \
		"cmp.b $loadaddr $verifyaddr 0x20000;\0" \
	"writeuboot=nand erase.part u-boot; " \
		"nand write $loadaddr u-boot; " \
		"nand read $verifyaddr u-boot; " \
		"cmp.b $loadaddr $verifyaddr " \
			__stringify(CONFIG_BOARD_SIZE_LIMIT) ";\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND	\
	"run boot_safemode; "

#define CONFIG_PREBOOT \
	"dcache on; " \
	"ubi part boot-config; " \

#endif /* __CONFIG_NIZYNQPROTO_H */
