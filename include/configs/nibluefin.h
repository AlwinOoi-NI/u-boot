/*
 * See zynq_common.h for Zynq common configs
 */

#ifndef __CONFIG_NIBLUEFIN_H
#define __CONFIG_NIBLUEFIN_H

/*
 * High Level Configuration Options
 */
#define CONFIG_TARGET_CLASS "DAQmx Device"

#if defined(CONFIG_BLUEFIN)
#define CONFIG_DEVICE_CODE "78B9"
#define CONFIG_DEVICE_DESC "cDAQ-918x"
#define CONFIG_STATUS_LED_GPIO   46
#define CONFIG_ACTIVE_LED_GPIO   47
#define CONFIG_RESET_BUTTON_GPIO 48
#elif defined(CONFIG_DORITO)
#define CONFIG_DEVICE_CODE "78BF"
#define CONFIG_DEVICE_DESC "ENET-63xx"
#define CONFIG_RESET_BUTTON_GPIO 47
#else
#error "Unknown CONFIG_DEVICE_DESC"
#endif

#define CONFIG_PREFIXED_DEVICE_DESC "NI " CONFIG_DEVICE_DESC
#define CONFIG_NI_BOARD_NAME CONFIG_DEVICE_DESC

#define CONFIG_CPU_FREQ_HZ 666666666

#include <configs/zynq_common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_SYS_NO_FLASH

#include <config_cmd_default.h>
#undef CONFIG_CMD_XIMG
#define CONFIG_FIT
#define CONFIG_OF_BOARD_SETUP
#define CONFIG_CMD_PING		/* Might be useful for debugging */
#define CONFIG_REGINFO		/* Again, debugging */
#undef CONFIG_CMD_SETGETDCR	/* README says 4xx only */
/* check for input to stop even if delay is 0 */
#define CONFIG_ZERO_BOOTDELAY_CHECK
#undef CONFIG_BOOTDELAY
/* -1 to Disable autoboot, -2 to force boot */
#define CONFIG_BOOTDELAY	-2
#define CONFIG_SILENT_CONSOLE
#define CONFIG_SILENT_U_BOOT_ONLY
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_SET
#define CONFIG_SYS_DEVICE_NULLDEV
#define CONFIG_BAUDRATE_UPDATE_ON_RELOC

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
#define CONFIG_CMD_GPIO

#define CONFIG_TIMESTAMP	/* print image timestamp on bootm, etc */

#ifdef CONFIG_MFG
#define CONFIG_NI_BOARD_NAME_SUFFIX " Manufacturing"
#else
#define CONFIG_NI_BOARD_NAME_SUFFIX
#endif

#ifdef CONFIG_PROTO
#define CONFIG_NI_BOARD_NAME_PREFIX " Prototype"
#else
#define CONFIG_NI_BOARD_NAME_PREFIX
#endif

#define CONFIG_IDENT_STRING \
	"\nNational Instruments" \
	CONFIG_NI_BOARD_NAME_PREFIX " " \
	CONFIG_NI_BOARD_NAME \
	CONFIG_NI_BOARD_NAME_SUFFIX

#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT	"U-Boot> "

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING

#define CONFIG_ENV_IS_IN_UBI
#define CONFIG_ENV_UBI_PART "boot-config"
#define CONFIG_ENV_UBI_VOLUME "u-boot-env1"
#define CONFIG_ENV_UBI_VOLUME_REDUND "u-boot-env2"
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE			0x20000

#define CONFIG_CMD_SAVEENV	/* Command to save ENV to Flash */
#define CONFIG_CMD_ENV_FLAGS

#define CONFIG_FPGA
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_ZYNQPL
#define CONFIG_CMD_FPGA

/* HW to use */
#define CONFIG_SERIAL_MULTI
/* Both Bluefin and Dorito use UART1 for serial debug */
#if defined(CONFIG_BLUEFIN) || defined(CONFIG_DORITO)
#define CONFIG_ZYNQ_UART1
#define CONFIG_CONSOLE_UBOOT_DEV "zynquart1"
#endif

#if defined(CONFIG_BLUEFIN)
/* Bluefin only has UART1 enabled, so use first serial port. */
#define CONFIG_CONSOLE_LINUX_DEV "ttyPS0"
#elif defined(CONFIG_DORITO)
/* Dorito has both UART0+UART1, so we need to use the second. */
#define CONFIG_CONSOLE_LINUX_DEV "ttyPS1"
#endif

/* Use both GEM interfaces */
#define CONFIG_NET_MULTI
#define CONFIG_ZYNQ_GEM
#define CONFIG_ZYNQ_GEM_COUNT		1

/* GEM0 on EMIO */
#define CONFIG_ZYNQ_GEM0_BASE_ADDR	XPSS_GEM0_BASEADDR
#define CONFIG_ZYNQ_GEM0_PHY_ADDR	0

#if defined(CONFIG_BLUEFIN)
#define CONFIG_MV88E61XX_SWITCH
#define CONFIG_RESET_PHY_R	/* call board driver's reset_phy */
#endif

#define CONFIG_CMD_MII
#define CONFIG_MII
#define CONFIG_PHY_GIGE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_LINK_LOCAL
#define CONFIG_MENU
#define CONFIG_CMD_PXE
#define CONFIG_BOOTP_MAY_FAIL
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_NETCONSOLE

#ifdef CONFIG_DEVICE_CODE
#define CONFIG_PXE_PLATFORM_DEFAULT	"default-" CONFIG_TARGET_CLASS, \
					"default-" CONFIG_DEVICE_CODE
#else
#define CONFIG_PXE_PLATFORM_DEFAULT	"default-" CONFIG_TARGET_CLASS
#endif
					
#ifdef CONFIG_MFG
#define CONFIG_ETHADDR 00:80:2f:00:00:00
#define CONFIG_ENV_OVERWRITE
#endif

#define CONFIG_ZYNQ_GPIO

#if defined(CONFIG_BLUEFIN)
#define CONFIG_CMD_I2C
/* not really used */
#define CONFIG_SYS_I2C_SPEED 400
#define CONFIG_HARD_I2C
#define CONFIG_ZYNQ_I2C
#define CONFIG_ZYNQ_I2C_CTLR_0
#define CONFIG_ZYNQ_I2C_MIO_RESET
#define CONFIG_ZYNQ_I2C_SCL_MIO 42
#define CONFIG_ZYNQ_I2C_SDA_MIO 43
#endif
#undef CONFIG_RTC_XPSSRTC

/* USB Host support */
#define CONFIG_CMD_USB
#define CONFIG_USB_HOST
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_ETHER_SMSC95XX
#define CONFIG_USB_ULPI
#define CONFIG_USB_ULPI_VIEWPORT
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_ZYNQ
#define CONFIG_USB_EHCI_TXFIFO_THRESH 8
#define CONFIG_EHCI_IS_TDI
#define CONFIG_USB_BASE_ADDR XPSS_USB0_BASEADDR /* int 21 */

/*
 * Physical Memory map
 */
#define PHYS_SDRAM_1_SIZE (256 * 1024 * 1024)
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
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_RESERVE_END 524288
#define CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT "nand0=xilinx_nand"
#define MTDPARTS_DEFAULT \
	"mtdparts=xilinx_nand:" \
		"128k(fsbl)ro," \
		"512k(u-boot)ro," \
		"36M(boot-config)," \
		"-(root)"

#if defined(CONFIG_MFG)
#define CONFIG_NAND_BBT_NO_MARK
#endif

/* UBI */
#define CONFIG_RBTREE
#define CONFIG_CMD_UBI

/* UBIFS */
#define CONFIG_LZO
#define CONFIG_CMD_UBIFS

#define CONFIG_MTD_UBOOT_OFFSET		0x20000
#define CONFIG_BOARD_SIZE_LIMIT		0x80000
#define CONFIG_BOOT_BIN_SIZE_LIMIT	0xA0000
#define CONFIG_BOOTFS_VOLUME_SIZE	0x17A0000
#define CONFIG_ROOTFS_VOLUME_SIZE	0x6AB0000

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

#define FDT_HIGH "0x17FFFFFF"
#define INITRD_HIGH "0x17FF7FFF"
#define VERIFY_ADDR "0x10000000"

/* This address is 32k before the load address of the initrd */
#define FDT_ADDR "0x7FF8000"

#define REAL_EXTRA_ENV_SETTINGS \
	"autoload=n\0" \
	"silent=1\0" \
	"bootdelay=-2\0" \
	"verify=n\0" \
	"console=" CONFIG_CONSOLE_LINUX_DEV ",115200\0" \
	"fdt_high=" FDT_HIGH "\0" \
	"initrd_high=" INITRD_HIGH "\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"verifyaddr=" VERIFY_ADDR "\0" \
	"DeviceCode=0x" CONFIG_DEVICE_CODE "\0" \
	"DeviceDesc=" CONFIG_DEVICE_DESC "\0" \
	"TargetClass=" CONFIG_TARGET_CLASS "\0" \
	"ncoutport=7865\0" \
	"ncinport=8473\0" \
	"ncip=192.168.1.1\0" \
	"nc=" \
		"setenv stdout nc;" \
		"setenv stdin nc;\0" \
	"sc=" \
		"setenv stdout " CONFIG_CONSOLE_UBOOT_DEV ";" \
		"setenv stderr " CONFIG_CONSOLE_UBOOT_DEV ";" \
		"setenv stdin " CONFIG_CONSOLE_UBOOT_DEV ";\0" \
	"recoverybootcmd=" \
		"if test -n \\\\\"$savebootcmd\\\\\"; " \
		"then " \
			"flags=${.flags} && " \
			"setenv .flags bootcmd && " \
			"setenv bootcmd \\\\\"$savebootcmd\\\\\"; " \
			"setenv .flags $flags; " \
		"fi; " \
		"if test -n \\\\\"$savebootdelay\\\\\"; " \
		"then " \
			"setenv bootdelay $savebootdelay; " \
		"fi;\0" \
	"recoverycmd=" \
		"echo Entering recovery mode!; " \
		"dcache off; " \
		"flags=${.flags} && " \
			"savebootcmd=$bootcmd && " \
			"setenv .flags bootcmd && " \
			"setenv bootcmd \\\\\"$recoverybootcmd\\\\\" && " \
			"setenv .flags $flags; " \
		"savebootdelay=$bootdelay && setenv bootdelay -2; " \
		"run nc; " \
		"while sleep 1; do " \
			"echo \\\"${DeviceCode:-<not-set>}, " \
				"${serial#:-<not-set>}, " \
				"${ethaddr:-<not-set>}, " \
				"${ipaddr:-<not-set>}, " \
				"${hostname:-<not-set>}, " \
				"${comment:-<not-set>}\\\"; " \
		"done;\0" \
	"switchfw=" \
		"oldimagefile=$imagefile; " \
		"oldrootdev=$rootdev; " \
		"imagefile=$altimagefile; " \
		"rootdev=$altrootdev; " \
		"altimagefile=$oldimagefile; " \
		"altrootdev=$oldrootdev;\0" \
	"updateenv=" \
		"env export -b $loadaddr; " \
		"env default -f -a; " \
		"env import -b $loadaddr;\0" \
	"writepartitions=" \
		"if ubi part boot-config && " \
			"ubi read $verifyaddr u-boot-env1 1 && " \
			"ubi read $verifyaddr u-boot-env2 1 && " \
		"then " \
			"ubi remove bootfs && " \
			"ubi remove config; " \
		"else " \
			"nand erase.part boot-config && " \
			"ubi part boot-config && " \
			"ubi create u-boot-env1 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
			"ubi create u-boot-env2 " __stringify(CONFIG_ENV_SIZE) " dynamic; " \
		"fi && " \
		"ubi create bootfs " __stringify(CONFIG_BOOTFS_VOLUME_SIZE) " dynamic && " \
		"ubi create config - dynamic && " \
		"if ubi part root && " \
			"ubi read $verifyaddr rootfs1 1; " \
		"then " \
			"ubi remove rootfs1; " \
			"ubi remove rootfs2; " \
		"else " \
			"nand erase.part root && " \
			"ubi part root; " \
		"fi && " \
		"ubi create rootfs1 " __stringify(CONFIG_ROOTFS_VOLUME_SIZE) " dynamic; " \
		"ubi create rootfs2 - dynamic;\0" \
	"writeboot=" \
		"nand erase 0 " __stringify(CONFIG_BOOT_BIN_SIZE_LIMIT) "; " \
		"nand write $loadaddr 0 " __stringify(CONFIG_BOOT_BIN_SIZE_LIMIT) "; " \
		"nand read $verifyaddr 0 " __stringify(CONFIG_BOOT_BIN_SIZE_LIMIT) "; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_BOOT_BIN_SIZE_LIMIT) ";\0" \
	"writefsbl=" \
		"nand erase.part fsbl; " \
		"nand write $loadaddr fsbl; " \
		"nand read $verifyaddr fsbl; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_MTD_UBOOT_OFFSET) ";\0" \
	"writeuboot=" \
		"nand erase.part u-boot; " \
		"nand write $loadaddr u-boot; " \
		"nand read $verifyaddr u-boot; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_BOARD_SIZE_LIMIT) ";\0"

#define REAL_PREBOOT \
	"dcache on; " \
	"ubi part boot-config; " \
	"ubifsmount ubi:bootfs; " \
	"run sc;" \
	"imagefile=bootimage0.itb; rootdev=mtdblock8; " \
	"altimagefile=bootimage1.itb; altrootdev=mtdblock9; " \
	"if ubifsload $loadaddr image_1_active; " \
	"then " \
		"run switchfw; " \
	"fi"

#define REAL_BOOTCOMMAND \
	"if ubifsmount ubi:bootfs && ubifsload $loadaddr $imagefile && imi $loadaddr; " \
	"then " \
		"source $loadaddr:bootscript; " \
	"elif run switchfw && ubifsload $loadaddr $imagefile && imi $loadaddr; " \
	"then " \
		"source $loadaddr:bootscript; " \
	"fi; " \
	"echo 'Boot failed!'; " \
	"run recoverycmd; "

#if defined(CONFIG_MFG)

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"ipaddr=192.168.1.2\0" \
	"netmask=255.255.255.0\0" \
	"gatewayip=192.168.1.1\0" \
	"serverip=192.168.1.1\0" \
	REAL_EXTRA_ENV_SETTINGS \
	"realbootcmd="REAL_BOOTCOMMAND"\0" \
	"realpreboot="REAL_PREBOOT"\0" \
	"banner=" \
		"echo \"********\" && " \
		"echo \"******** $text\" && " \
		"echo \"********\";\0" \
	"bringup=" \
		"dcache off; " \
		"serial_save=${serial#:-DEADBEEF} && " \
			"ethaddr_save=${ethaddr:-02:00:DE:AD:BE:EF} && " \
			"env default -f -a && " \
			"env set serial# $serial_save && " \
			"env set ethaddr $ethaddr_save; " \
		"setenv silent; " \
		"if setenv bootcmd \"$realbootcmd\" && " \
			"setenv realbootcmd && " \
			"setenv preboot \"$realpreboot\" && " \
			"setenv realpreboot && " \
			"nand unlock && " \
			"setenv ipaddr 192.168.1.2 && " \
			"setenv netmask 255.255.255.0 && " \
			"setenv serverip 192.168.1.1 && " \
			"setenv text 'Installing BOOT_NAND.BIN...' && run banner && " \
				"tftpboot BOOT.BIN && " \
				"run writeboot && " \
			"setenv text 'Partitioning NAND flash...' && run banner && " \
				"run writepartitions && " \
				"setenv text && " \
				"saveenv && " \
				"saveenv && " \
			"setenv text 'Installing bootfs...' && run banner && " \
				"ubi part boot-config && " \
				"tftpboot bootfs.ubifs && " \
				"ubi write $loadaddr bootfs $filesize && " \
			"setenv text 'Installing rootfs...' && run banner && " \
				"ubi part root && " \
				"tftpboot rootfs.img && " \
				"ubi write $loadaddr rootfs1 $filesize; " \
		"then " \
			"setenv text 'Auto-bringup succeeded!' && run banner; " \
		"else " \
			"setenv text 'Auto-bringup failed!' && run banner; " \
		"fi; " \
		"setenv text; " \

#undef CONFIG_BOOTCOMMAND

/* For bringup, use serial */
#define CONFIG_PREBOOT \
	"run sc;" \
	"setenv silent;"

#else /* !CONFIG_MFG */

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	REAL_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	REAL_BOOTCOMMAND

#define CONFIG_PREBOOT \
	REAL_PREBOOT

#endif

#endif /* __CONFIG_NIBLUEFIN_H */
