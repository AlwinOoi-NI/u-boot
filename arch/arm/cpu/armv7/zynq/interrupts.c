/*
 * (C) Copyright 2016
 * Brandon Streiff, National Instruments <brandon.streiff@ni.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/xparameters_ps.h>
#include <asm/arch/gic.h>
#include <asm/io.h>

/*
 * This is incredibly basic interrupt support for the GIC as used
 * in the Zynq.
 */

#define N_IRQS			96
#define N_IRQ_REGS		(N_IRQS >> 5)

/* IRQs #0->#15 are software generated */
#define N_SGI_IRQBASE		0
#define N_SGI_IRQS		16
/* IRQs #16->#31 are private to each processor */
#define N_PPI_BASE		16
#define N_PPI_IRQS		16
/* IRQs #32->#95 are shared peripheral interrupts */
#define N_SPI_BASE		32
#define N_SPI_IRQS		64

#define IRQS_PER_BANK		32

#define GICC_BASEADDR		(XPSS_SCU_BASEADDR+GIC_CPU_OFFSET_A9)
#define GICD_BASEADDR		(XPSS_SCU_BASEADDR+GIC_DIST_OFFSET)

#define GICC_CTLR_ENABLE_VALUE	0x00000001
#define GICD_CTLR_ENABLE_VALUE	0x00000001
#define GICD_INT_DEF_TARGET	0x01
#define GICD_INT_DEF_TARGET_X4	((GICD_INT_DEF_TARGET << 24) | \
				 (GICD_INT_DEF_TARGET << 16) | \
				 (GICD_INT_DEF_TARGET << 8) | \
				 (GICD_INT_DEF_TARGET))
#define GICD_INT_DEF_PRI	0xa0
#define GICD_INT_DEF_PRI_X4	((GICD_INT_DEF_PRI << 24) | \
				 (GICD_INT_DEF_PRI << 16) | \
				 (GICD_INT_DEF_PRI << 8) | \
				 (GICD_INT_DEF_PRI))
#define GICC_PRI_THRESHOLD	0xf0

struct _irq_handler {
	void            *data;
	void            (*func)(void *data);
	unsigned int    count;
};

static struct _irq_handler handlers[N_IRQS];

static inline u32 gicc_readl(u32 offset)
{
	return readl(GICC_BASEADDR+offset);
}

static inline u32 gicd_readl(u32 offset)
{
	return readl(GICD_BASEADDR+offset);
}

static inline u32 gicc_writel(u32 value, u32 offset)
{
	return writel(value, GICC_BASEADDR+offset);
}

static inline u32 gicd_writel(u32 value, u32 offset)
{
	return writel(value, GICD_BASEADDR+offset);
}

static inline void enable_irq(int irq)
{
	int irqbank = (irq / IRQS_PER_BANK);
	int irqline = (irq % IRQS_PER_BANK);

	gicd_writel((1 << irqline), GICD_ISENABLERn + (irqbank*sizeof(u32)));
}

static inline void disable_irq(int irq)
{
	int irqbank = (irq / IRQS_PER_BANK);
	int irqline = (irq % IRQS_PER_BANK);

	gicd_writel((1 << irqline), GICD_ICENABLERn + (irqbank*sizeof(u32)));
}

static inline void clear_pending_irq(int irq)
{
	int irqbank = (irq / IRQS_PER_BANK);
	int irqline = (irq % IRQS_PER_BANK);

	gicd_writel((1 << irqline), GICD_ICPENDRn + (irqbank*sizeof(u32)));
}

void do_irq(struct pt_regs *pt_regs)
{
	u32 iar;
	u32 irq;

	/* Read the Interrupt Acknowledge Register */
	iar = gicc_readl(GICC_IAR);
	irq = (iar & 0x3FF);

	if (handlers[irq].func) {
		handlers[irq].count++;
		handlers[irq].func(handlers[irq].data);
	} else {
		disable_irq(irq);
		printf("unhandled irq %d\n", irq);
	}

	clear_pending_irq(irq);

	/* Write the End Of Interrupt Register */
	gicc_writel(iar, GICC_EOIR);
}

void irq_install_handler(int irq, interrupt_handler_t handle_irq, void *data)
{
	if (irq < 0 || irq >= N_IRQS)
		return;

	if (handle_irq) {
		if (handlers[irq].func && (handlers[irq].func != handle_irq))
			printf("warning: replacing handler for IRQ %d\n", irq);

		handlers[irq].data = data;
		handlers[irq].func = handle_irq;

		enable_irq(irq);
	} else {
		disable_irq(irq);
		handlers[irq].func = NULL;
	}
}

void irq_free_handler(int irq)
{
	irq_install_handler(irq, NULL, NULL);
}

int arch_interrupt_init(void)
{
	int i;

	memset(handlers, 0, sizeof(handlers));

	/* Enable the distributor. */
	gicd_writel(GICD_CTLR_ENABLE_VALUE, GICD_CTLR);

	/*
	 * Enable and set priority of SPIs.
	 * Each register in GICD_ITARGETSRn and GICD_IPRIORITYRn
	 * are four eight-bit values.
	 */
	for (i = (N_SPI_BASE/4); i < ((N_SPI_BASE+N_SPI_IRQS)/4); i++) {
		/* All SPIs will be handled on CPU#0. */
		gicd_writel(GICD_INT_DEF_TARGET_X4,
			    GICD_ITARGETSRn + (i*sizeof(u32)));
		/* Set priority for each. */
		gicd_writel(GICD_INT_DEF_PRI_X4,
			    GICD_IPRIORITYRn + (i*sizeof(u32)));
	}

	/* Enable the CPU Interface */
	gicc_writel(GICC_CTLR_ENABLE_VALUE, GICC_CTLR);

	/* Configure the priority mask for this processor. */
	gicc_writel(GICC_PRI_THRESHOLD, GICC_PMR);

	return 0;
}

#if defined(CONFIG_CMD_IRQ)
int do_irqinfo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i;

	printf("Interrupt information:\n");
	printf("Nr  Func      Arg       Count\n");
	printf("-------------------------------\n");

	for (i = 0; i < N_IRQS; i++) {
		if (handlers[i].func) {
			printf("%02d  %08lx  %08lx  %d\n", i,
				(ulong)handlers[i].func,
				(ulong)handlers[i].data,
				handlers[i].count);
		}
	}
	printf("\n");

	return 0;
}
#endif
