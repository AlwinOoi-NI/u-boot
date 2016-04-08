/*
 * Copied and modified from nizynqproto.h
 *
 * See zynq_common.h for Zynq common configs
 */

#ifndef __CONFIG_NILCI_H
#define __CONFIG_NILCI_H

/*
 * High Level Configuration Options
 */
#define CONFIG_NILCI /* Board */

#include <configs/zynq_common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_SYS_NO_FLASH

#include <config_cmd_default.h>
#undef CONFIG_CMD_XIMG
#define CONFIG_FIT
#define CONFIG_OF_BOARD_SETUP
//#define CONFIG_FIT_VERBOSE /* enable fit_format_{error,warning}() */
#define CONFIG_CMD_DATE		/* RTC? */
#define CONFIG_CMD_PING		/* Might be useful for debugging */
#define CONFIG_REGINFO		/* Again, debugging */
#undef CONFIG_CMD_SETGETDCR	/* README says 4xx only */
/* check for input to stop even if delay is 0 */
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_SILENT_CONSOLE
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_SET
#define CONFIG_SYS_DEVICE_NULLDEV

#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "hush> "
#undef CONFIG_SYS_MAXARGS
#define CONFIG_SYS_MAXARGS 64

#define CONFIG_CMD_UNZIP
#define CONFIG_CMD_SETEXPR
#define CONFIG_CRC32_VERIFY
#define CONFIG_CMD_MD5SUM
#define CONFIG_MD5SUM_VERIFY
#define CONFIG_CMD_SHA1SUM
#define CONFIG_SHA1SUM_VERIFY
#define CONFIG_CMD_INI
#define CONFIG_INI_CASE_INSENSITIVE

#define CONFIG_TIMESTAMP	/* print image timestamp on bootm, etc */

#ifdef CONFIG_BRINGUP
#define CONFIG_IDENT_STRING	"\nNational Instruments LCI Manufacturing"
#else
#define CONFIG_IDENT_STRING	"\nNational Instruments LCI"
#endif

#define CONFIG_PANIC_HANG /* For development/debugging */

#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT	"ni-lci> "

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING

#define CONFIG_ENV_IS_IN_UBI
#define CONFIG_ENV_UBI_PART "boot-config"
#define CONFIG_ENV_UBI_VOLUME "u-boot-env1"
#define CONFIG_ENV_UBI_VOLUME_REDUND "u-boot-env2"
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE                 0x20000
/* Old NAND env settings */
#define CONFIG_ENV_OFFSET               0xA0000
#define CONFIG_ENV_OFFSET_REDUND        0xE0000
#define CONFIG_ENV_RANGE                0x40000

#define CONFIG_CMD_SAVEENV	/* Command to save ENV to Flash */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_FPGA
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_ZYNQPL
#define CONFIG_CMD_FPGA

/* HW to use */
#define CONFIG_ZYNQ_UART0

/* Use both GEM interfaces */
#define CONFIG_NET_MULTI
#define CONFIG_ZYNQ_GEM
#define CONFIG_ZYNQ_GEM_COUNT          1

/* GEM0 on MIO */
#define CONFIG_ZYNQ_GEM0_BASE_ADDR     XPSS_GEM0_BASEADDR
#define CONFIG_ZYNQ_GEM0_PHY_ADDR      0

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

/* USB Host support */
#define CONFIG_CMD_USB
#define CONFIG_USB_HOST
#define CONFIG_USB_STORAGE
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_ETHER_SMSC95XX
#define CONFIG_USB_ULPI
#define CONFIG_USB_ULPI_VIEWPORT
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_ZYNQ
#define CONFIG_USB_EHCI_TXFIFO_THRESH 8
#define CONFIG_EHCI_IS_TDI
#define CONFIG_USB_BASE_ADDR XPSS_USB0_BASEADDR

/*
 * Physical Memory map
 *
 * Note that we only declare the lower 214MB
 * of ram, the remaining 42MB is used for usermode DMA.
 */
#define PHYS_SDRAM_1_SIZE (214 * 1024 * 1024)
#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_SCRATCH 0xFFFFF000
#define CONFIG_CMD_CACHE

/*
 * NAND Flash settings
 */
#define CONFIG_NAND_ZYNQ
#define CONFIG_NAND_ZYNQ_USE_FSBL_TIMINGS
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_SYS_MAX_NAND_DEVICE 1
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_SYS_NAND_BASE XPSS_NAND_BASEADDR
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_RESERVE_END 524288
#define CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT "nand0=xilinx_nand"
#define MTDPARTS_DEFAULT \
	"mtdparts=xilinx_nand:" \
		"128k(fsbl)ro," \
		"512k(u-boot)ro," \
		"40M(boot-config)," \
		"-(root)"
//#define CONFIG_MTD_DEBUG
//#define CONFIG_MTD_DEBUG_VERBOSE 3

/* UBI */
#define CONFIG_RBTREE
#define CONFIG_CMD_UBI

/* UBIFS */
#define CONFIG_LZO
#define CONFIG_CMD_UBIFS

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

#ifdef CONFIG_VB8034
   #define DEVICE_CODE "0x77DE"
   #define DEVICE_DESC "NI VB-8034"
#elif CONFIG_VB8054
   #define DEVICE_CODE "0x78B4"
   #define DEVICE_DESC "NI VB-8054"
#elif CONFIG_VB8012
   #define DEVICE_CODE "0x76F6"
   #define DEVICE_DESC "NI VB-8012"
#endif


#define LCI_ENV_SETTINGS \
	"autoload=n\0" \
	"silent=1\0" \
	"bootdelay=-2\0" \
	"verify=n\0" \
	"console=ttyPS0,115200\0" \
	"fdt_high=0x17ffffff\0" \
	"initrd_high=0x17ffffff\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"verifyaddr=0x10000000\0" \
	"DeviceCode=" DEVICE_CODE "\0" \
	"TargetClass=VirtualBench\0" \
	"DeviceDesc=" DEVICE_DESC "\0" \
	"fpgaloadcmd=if ubifsload $loadaddr $imagefile; " \
		"then " \
			"configfpga=1; " \
		"elif ubifsload $loadaddr $altimagefile; " \
		"then " \
			"run switchfw; " \
			"configfpga=1; " \
		"fi; " \
		"if test -n \\\\\"$configfpga\\\\\"; then " \
			"if fpga loadmk 0 $loadaddr:fpga; " \
			"then " \
			"else " \
				"run switchfw; " \
				"ubifsload $loadaddr $imagefile; " \
				"fpga loadmk 0 $loadaddr:fpga; " \
			"fi; " \
		"fi;\0" \
	"switchfw=" \
		"oldimagefile=$imagefile; " \
		"oldrootdev=$rootdev; " \
		"imagefile=$altimagefile; " \
		"rootdev=$altrootdev; " \
		"altimagefile=$oldimagefile; " \
		"altrootdev=$oldrootdev;\0" \
	"updateenv=env export -b $loadaddr; " \
		"env default -f -a; " \
		"env import -b $loadaddr;\0" \
	"writepartitions=" \
		"nand erase.part boot-config && " \
		"ubi part boot-config && " \
		"ubi create u-boot-env1 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
		"ubi create u-boot-env2 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
		"ubi create bootfs 0x1C00000 dynamic && " \
		"ubi create config - dynamic && " \
		"nand erase.part root && " \
		"ubi part root && " \
		"ubi create rootfs1 0x3100000 dynamic && " \
		"ubi create rootfs2 0x3100000 dynamic &&" \
		"ubi create sfp1 0x3100000 dynamic &&" \
		"ubi create sfp2 0x3100000 dynamic &&" \
		"ubi create cal - dynamic; \0" \
	"writeboot=nand erase 0 " __stringify(CONFIG_ENV_OFFSET) "; " \
		"nand write $loadaddr 0 " __stringify(CONFIG_ENV_OFFSET) "; " \
		"nand read $verifyaddr 0 "  __stringify(CONFIG_ENV_OFFSET) "; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_ENV_OFFSET) ";\0" \
	"writefsbl=nand erase.part fsbl; " \
		"nand write $loadaddr fsbl; " \
		"nand read $verifyaddr fsbl; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_MTD_UBOOT_OFFSET) ";\0" \
	"writeuboot=nand erase.part u-boot; " \
		"nand write $loadaddr u-boot; " \
		"nand read $verifyaddr u-boot; " \
		"cmp.b $loadaddr $verifyaddr " \
			__stringify(CONFIG_BOARD_SIZE_LIMIT) ";\0"

#if defined(CONFIG_VB8034) || defined(CONFIG_VB8054)
/* For VB8034 and VB8054, deassert ethernet phy reset right before booting.
   This gives the power rails time to stabilize */
#define LCI_BOOTCOMMAND \
	"if test -n \\\\\"$configfpga\\\\\"; then " \
		"mw.l 0xE000A044 0xF0000; "\
		"source $loadaddr:bootscript;" \
	"fi; " \
	"echo 'Boot failed!' "
#elif defined(CONFIG_VB8012)
#define LCI_BOOTCOMMAND \
	"if test -n \\\\\"$configfpga\\\\\"; then " \
		"source $loadaddr:bootscript;" \
	"fi; " \
	"echo 'Boot failed!' "
#endif

/* For VB8034 and VB8054, drive PHY reset high and enable GPIO output. */
#if defined(CONFIG_VB8034) || defined(CONFIG_VB8054)
#define LCI_PREBOOT \
	"dcache on; " \
	"mw.l 0xE000A248 0xF0000; "\
	"mw.l 0xE000A244 0xF0000; "\
	"mw.l 0xE000A044 0xD0000; "\
	"ubi part boot-config; ubifsmount ubi:bootfs; " \
	"if ubifsload $loadaddr image_1_active; " \
	"then " \
		"imagefile=lci1.itb; " \
		"rootdev=mtdblock9; " \
		"altimagefile=lci0.itb; " \
		"altrootdev=mtdblock8; " \
	"else " \
		"imagefile=lci0.itb; " \
                "rootdev=mtdblock8; " \
                "altimagefile=lci1.itb; " \
                "altrootdev=mtdblock9; " \
	"fi; " \
	"run fpgaloadcmd"
#elif defined(CONFIG_VB8012)
#define LCI_PREBOOT \
	"dcache on; " \
	"ubi part boot-config; ubifsmount ubi:bootfs; " \
	"if ubifsload $loadaddr image_1_active; " \
	"then " \
		"imagefile=lci1.itb; " \
		"rootdev=mtdblock9; " \
		"altimagefile=lci0.itb; " \
		"altrootdev=mtdblock8; " \
	"else " \
		"imagefile=lci0.itb; " \
                "rootdev=mtdblock8; " \
                "altimagefile=lci1.itb; " \
                "altrootdev=mtdblock9; " \
	"fi; " \
	"run fpgaloadcmd"
#endif

#ifndef CONFIG_BRINGUP

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS LCI_ENV_SETTINGS
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND LCI_BOOTCOMMAND
#define CONFIG_PREBOOT LCI_PREBOOT

#else /* CONFIG_BRINGUP */

#define CONFIG_CMD_ASKENV

/* don't mark the BBT bad, so MFG can check it for real */
#define CONFIG_NAND_BBT_NO_MARK

#undef CONFIG_EXTRA_ENV_SETTINGS
/* For VB-8034 and VB-8054, drive PHY reset high, set GPIO enable and set the direction.
* Unlock the nand in preboot only for these two devices. */
#if defined(CONFIG_VB8034) || defined(CONFIG_VB8054)
#define CONFIG_EXTRA_ENV_SETTINGS \
	LCI_ENV_SETTINGS \
	"realbootcmd="LCI_BOOTCOMMAND"\0" \
	"realpreboot="LCI_PREBOOT"\0" \
	"bringup=" \
		"dcache off &&" \
		"env default -f -a;" \
		"setenv silent;" \
		"if test -z \\\\\"${serial\\\\#}\\\\\"; then " \
			"setenv serial# DEADBEEF; " \
		"fi; " \
		"if test -n \\\\\"${serial\\\\#}\\\\\" && " \
		"setenv bootcmd \"$realbootcmd\" && " \
		"setenv realbootcmd && " \
		"setenv preboot \"$realpreboot\" && " \
		"setenv realpreboot && " \
		"mw.l 0xE000A248 0xF0000 && "\
		"mw.l 0xE000A244 0xF0000 && "\
		"mw.l 0xE000A044 0xF0000 && "\
		"nand unlock && " \
		"setenv ethaddr 00:0B:AD:BA:AD:DB && "\
		"setenv ipaddr 192.168.1.2 && " \
		"setenv netmask 255.255.255.0 && " \
		"setenv serverip 192.168.1.1 && " \
		"echo '*******' && " \
		"echo '*******  Installing BOOT_NAND.BIN...' && " \
		"echo '*******' && " \
		"tftpboot BOOT.BIN && " \
		"run writeboot && " \
		"echo '*******' && " \
		"echo '*******  Partitioning NAND flash...' && " \
		"echo '*******' && " \
		"run writepartitions && " \
		"saveenv && saveenv &&" \
		"echo '*******' && " \
		"echo '*******  Installing bootfs.ubifs...' && " \
		"echo '*******' && " \
		"ubi part boot-config && " \
		"tftpboot bootfs.ubifs && " \
		"ubi write $loadaddr bootfs $filesize && " \
		"echo '*******' && " \
		"echo '*******  Installing root.squashfs...' && " \
		"echo '*******' && " \
		"ubi part root && " \
		"tftpboot root.squashfs && " \
		"ubi write $loadaddr rootfs1 $filesize && " \
		"echo '*******' && " \
		"echo '*******  Installing sfp.iso...' && " \
		"echo '*******' && " \
		"tftpboot sfp.iso && " \
		"ubi write $loadaddr sfp1 $filesize && " \
		"echo '*******' && " \
		"echo '*******  Set bootmode DIP switches to (on)(on)(off)(on) NAND boot mode and reboot.'; then " \
		"else " \
			"echo '*******' && " \
			"echo '*******  Auto-bringup failed!'; " \
		"fi;"
#elif defined(CONFIG_VB8012)
#define CONFIG_EXTRA_ENV_SETTINGS \
	LCI_ENV_SETTINGS \
	"realbootcmd="LCI_BOOTCOMMAND"\0" \
	"realpreboot="LCI_PREBOOT"\0" \
	"bringup=" \
		"env default -f -a;" \
		"setenv silent;" \
		"if test -z \\\\\"${serial\\\\#}\\\\\"; then " \
			"setenv serial# DEADBEEF; " \
		"fi; " \
		"if test -n \\\\\"${serial\\\\#}\\\\\" && " \
		"setenv bootcmd \"$realbootcmd\" && " \
		"setenv realbootcmd && " \
		"setenv preboot \"$realpreboot\" && " \
		"setenv realpreboot && " \
		"mw.l 0x40120050 2 && " \
		"mw.l 0x40101010 1 && " \
		"usb start &&" \
		"usb reset &&" \
		"setenv ipaddr 192.168.1.2 && " \
		"setenv netmask 255.255.255.0 && " \
		"setenv serverip 192.168.1.1 && " \
		"echo '*******' && " \
		"echo '*******  Installing BOOT_NAND.BIN...' && " \
		"echo '*******' && " \
		"tftpboot BOOT.BIN && " \
		"run writeboot && " \
		"echo '*******' && " \
		"echo '*******  Partitioning NAND flash...' && " \
		"echo '*******' && " \
		"run writepartitions && " \
		"saveenv && saveenv &&" \
		"echo '*******' && " \
		"echo '*******  Installing bootfs.ubifs...' && " \
		"echo '*******' && " \
		"ubi part boot-config && " \
		"tftpboot bootfs.ubifs && " \
		"ubi write $loadaddr bootfs $filesize && " \
		"echo '*******' && " \
		"echo '*******  Installing root.squashfs...' && " \
		"echo '*******' && " \
		"ubi part root && " \
		"tftpboot root.squashfs && " \
		"ubi write $loadaddr rootfs1 $filesize && " \
		"echo '*******' && " \
		"echo '*******  Installing sfp.iso...' && " \
		"echo '*******' && " \
		"tftpboot sfp.iso && " \
		"ubi write $loadaddr sfp1 $filesize && " \
		"echo '*******' && " \
		"echo '*******  Set bootmode DIP switches to (on)(on)(off)(on) NAND boot mode and reboot.'; then " \
		"else " \
			"echo '*******' && " \
			"echo '*******  Auto-bringup failed!'; " \
		"fi;"
#endif

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND	\
       "env default -f -a;" \
       "setenv silent;"

#endif

#endif /* __CONFIG_NILCI_H */
