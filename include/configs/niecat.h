#ifndef __CONFIG_NIECAT_H
#define __CONFIG_NIECAT_H

/*
 * High Level Configuration Options
 */
#if defined (CONFIG_NI9145)
#define CONFIG_DEVICE_CODE "789B"
#define CONFIG_FPGA_DEVICE_CODE "7743"
#define CONFIG_DEVICE_DESC "NI 9145"
#define CONFIG_TARGET_CLASS "cRIO"
#else
#error "Unrecognized CONFIG_DEVICE_CODE"
#endif
#ifndef CONFIG_PREFIXED_DEVICE_DESC
#define CONFIG_PREFIXED_DEVICE_DESC "NI " CONFIG_DEVICE_DESC
#endif

#define CONFIG_NI_USB_PID "0x770D"
#define CONFIG_NI_USB_VID "0x3923"

#define CONFIG_CPU_FREQ_HZ 666666666

#include <configs/zynq_common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_SYS_NO_FLASH

#define CONFIG_API

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

#define CONFIG_TIMESTAMP	/* print image timestamp on bootm, etc */

#if defined(CONFIG_NI9145)
#define CONFIG_NI_BOARD_NAME "NI 9145"
#else
#error "CONFIG_NI_BOARD_NAME not defined for this target"
#endif

#ifdef CONFIG_MFG
#define CONFIG_NI_BOARD_NAME_SUFFIX " Manufacturing"
#else
#define CONFIG_NI_BOARD_NAME_SUFFIX
#endif

#define CONFIG_NI_BOARD_NAME_PREFIX

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
#define CONFIG_ZYNQ_SERIAL
#define CONFIG_ZYNQ_UART0
#define CONFIG_CONSOLE_LINUX_DEV "ttyPS0"
#define CONFIG_CONSOLE_UBOOT_DEV "zynquart0"
#define CONFIG_SYS_DEVICE_NULLDEV

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

#define CONFIG_PXE_PLATFORM_DEFAULT	"default-" CONFIG_TARGET_CLASS, \
					"default-" CONFIG_DEVICE_CODE

#define CONFIG_ZYNQ_GPIO

#define CONFIG_CMD_I2C
/* not really used */
#define CONFIG_SYS_I2C_SPEED 400
#define CONFIG_HARD_I2C
#define CONFIG_ZYNQ_I2C
#define CONFIG_ZYNQ_I2C_CTLR_0
#define CONFIG_ZYNQ_I2C_MIO_RESET
#define CONFIG_ZYNQ_I2C_SCL_MIO 18
#define CONFIG_ZYNQ_I2C_SDA_MIO 19
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
#define CONFIG_USB_BASE_ADDR XPSS_USB1_BASEADDR

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
		"11M(u-boot)ro," \
		"19M(backup)ro," \
		"82M(boot-config)," \
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

#define CONFIG_MTD_UBOOT_OFFSET			0x20000
#define CONFIG_BOARD_SIZE_LIMIT			0x80000 /* 512MB */
#define CONFIG_BOOT_BIN_SIZE_LIMIT		0x120000
#define CONFIG_BOOTFS_VOLUME_SIZE		0x3600000

#define CONFIG_BACKUP_PAGE			0xB1F800
#define CONFIG_BACKUP_USBGADGETETHADDR_OFFSET	0x7ea
#define CONFIG_BACKUP_SERIAL_OFFSET		0x7f0
#define CONFIG_BACKUP_ETHADDR_OFFSET		0x7f4 /* Don't really use but
							 keep it to ensure
							 consistency across
							 different products */
#define CONFIG_BACKUP_ETH1ADDR_OFFSET		0x7fa

#define CONFIG_USER_FPGA_SIZE			0x500000
#define CONFIG_USER_FW_SIZE			0x200000
#define CONFIG_CARTRIDGE_SIZE			0x100000
#define CONFIG_BACKPLANE_SIZE			0x10000
#define CONFIG_EVENT_LOG_SIZE			0x10000
#define CONFIG_MOTION_SIZE			0x400100
#define CONFIG_MASTER_FPGA_SIZE			CONFIG_USER_FPGA_SIZE
#define CONFIG_MASTER_HEAD_SIZE			0x100000
#define CONFIG_MASTER_FW_SIZE			CONFIG_USER_FW_SIZE

#define CONFIG_ECAT_FW_LOADADDR		0x8000

#define READONLY_DEFAULT_ENV_FLAGS_APPEND \
	"ecatfwloadaddr:xo,boot_ecatfw:so,"

#define WRITE_PARTITIONS_COMMANDS_OVERRIDE \
	"writepartitions=" \
		"if ubi part boot-config && " \
			"ubi read $verifyaddr u-boot-env1 1 && " \
			"ubi read $verifyaddr u-boot-env2 1; " \
		"then " \
			"ubi remove user-fpga && " \
			"ubi remove user-fw && " \
			"ubi remove cartridge && " \
			"ubi remove event-log && " \
			"ubi remove motion && " \
			"ubi remove bootfs && " \
			"ubi remove config; " \
		"else " \
			"nand erase.part boot-config && " \
			"ubi part boot-config && " \
			"ubi create u-boot-env1 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
			"ubi create u-boot-env2 " __stringify(CONFIG_ENV_SIZE) " dynamic; " \
		"fi && " \
		"ubi create user-fpga "	__stringify(CONFIG_USER_FPGA_SIZE) " dynamic && " \
		"ubi create user-fw "	__stringify(CONFIG_USER_FW_SIZE) " dynamic && " \
		"ubi create cartridge "	__stringify(CONFIG_CARTRIDGE_SIZE) " dynamic && " \
		"ubi create backplane "	__stringify(CONFIG_BACKPLANE_SIZE) " dynamic && " \
		"ubi create event-log "	__stringify(CONFIG_EVENT_LOG_SIZE) " dynamic && " \
		"ubi create motion "	__stringify(CONFIG_MOTION_SIZE) " dynamic && " \
		"ubi create bootfs "	__stringify(CONFIG_BOOTFS_VOLUME_SIZE) " dynamic && " \
		"ubi create config - dynamic && " \
		"if ubi part backup && " \
			"ubi read $verifyaddr master-fpga 1 && " \
			"ubi read $verifyaddr master-head 1 && " \
			"ubi read $verifyaddr master-fw 1; "\
		"then " \
			"ubi remove master-fpga && " \
			"ubi remove master-head && " \
			"ubi remove master-fw; " \
		"else " \
			"nand erase.part backup && " \
			"ubi part backup; " \
		"fi && " \
		"ubi create master-fpga " __stringify(CONFIG_MASTER_FPGA_SIZE) " dynamic && " \
		"ubi create master-head " __stringify(CONFIG_MASTER_HEAD_SIZE) " dynamic && " \
		"ubi create master-fw " __stringify(CONFIG_MASTER_FW_SIZE) " dynamic && " \
		"if ubi part root && " \
			"ubi read $verifyaddr rootfs 1; " \
		"then " \
			"ubi remove rootfs; " \
		"else " \
			"nand erase.part root && " \
			"ubi part root; " \
		"fi && " \
		"ubi create rootfs - dynamic;\0" \

#define REAL_BOOTCOMMAND_OVERRIDE \
	"run boot_ecatfw"

#define REAL_PREBOOT_OVERRIDE \
	"setenv silent 1; " \
	"nand lock tight; " \
	"run ecatconsoleoutcmd; "

#define REAL_EXTRA_ENV_SETTINGS_APPEND \
	"boot_ecatfw=" \
		"if ubi part backup && " \
		"ubi read $ecatfwloadaddr master-head; then " \
			"go $ecatfwloadaddr; " \
		"else " \
			"echo Error booting firmware (head); " \
		"fi;\0" \
	"ecatconsoleoutcmd=" \
		"if test \\\\\"$bootdelay\\\\\" -gt 0 -o " \
		"\\\\\"$ecatconsoleout\\\\\" -gt 0; then " \
			"setenv silent; " \
		"fi; " \
		"run sc; \0" \
	"ecatfwloadaddr=" __stringify(CONFIG_ECAT_FW_LOADADDR) "\0" \

#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_DOS_PARTITION
#define CONFIG_DEFAULT_NVS

#undef CONFIG_SYS_LOAD_ADDR
#define CONFIG_SYS_LOAD_ADDR 0x4000000
#define CONFIG_LOADADDR CONFIG_SYS_LOAD_ADDR

#define FDT_HIGH "0x7FFFFFF"
#define INITRD_HIGH "0x7FF7FFF"
#define VERIFY_ADDR "0x8000000"

/* This address is 32k before the load address of the initrd */
#define FDT_ADDR "0x7FF8000"

#include "niresetenv.h"

/* usbgadgetethaddr for the USB Gadget Ethernet MAC. */
#define USBGADGETETHADDR_SAVE ENV_SAVE(usbgadgetethaddr)
#define USBGADGETETHADDR_RESTORE ENV_RESTORE(usbgadgetethaddr)

/* EtherCAT doesn't has WiFi. */
#define WIFIETHADDR_SAVE
#define WIFIETHADDR_RESTORE

/* EtherCAT doesn't has Ethernet port hence no MAC to save */
#define ETHADDR_SAVE
#define ETHADDR_RESTORE

/* EtherCAT does not have a wl12xx */
#define WL12XXNVS_SAVE
#define WL12XXNVS_RESTORE

#include "nicommonenv.h"

#if defined(READONLY_DEFAULT_ENV_FLAGS_APPEND)
#define CONFIG_ENV_FLAGS_LIST_STATIC \
	READONLY_DEFAULT_ENV_FLAGS_APPEND \
	READONLY_DEFAULT_ENV_FLAGS \
	READONLY_MFG_ENV_VARS \
	NET_TYPE_ENV_VARS
#else
#define CONFIG_ENV_FLAGS_LIST_STATIC \
	READONLY_DEFAULT_ENV_FLAGS \
	READONLY_MFG_ENV_VARS \
	NET_TYPE_ENV_VARS
#endif

#if defined(CONFIG_MFG)

#undef CONFIG_EXTRA_ENV_SETTINGS
#if defined(REAL_EXTRA_ENV_SETTINGS_APPEND)
#define CONFIG_EXTRA_ENV_SETTINGS \
	REAL_EXTRA_ENV_SETTINGS_APPEND \
	REAL_EXTRA_ENV_SETTINGS
#else
#define CONFIG_EXTRA_ENV_SETTINGS \
	REAL_EXTRA_ENV_SETTINGS
#endif

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND

#define CONFIG_PREBOOT \
	"setenv silent 1; " \
	"setenv ecatconsoleout 1;" \
	"run ecatconsoleoutcmd; "

#else /* !CONFIG_MFG */

#undef CONFIG_EXTRA_ENV_SETTINGS
#if defined(REAL_EXTRA_ENV_SETTINGS_APPEND)
#define CONFIG_EXTRA_ENV_SETTINGS \
	REAL_EXTRA_ENV_SETTINGS_APPEND \
	REAL_EXTRA_ENV_SETTINGS
#else
#define CONFIG_EXTRA_ENV_SETTINGS \
	REAL_EXTRA_ENV_SETTINGS
#endif

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	REAL_BOOTCOMMAND

#ifdef CONFIG_ENETEXP
#define CONFIG_PREBOOT \
	REAL_PREBOOT_FEEBLE_RECOVERY
#else
#define CONFIG_PREBOOT \
	REAL_PREBOOT
#endif

#endif

#endif /* __CONFIG_NICRIOZYNQ_H */
