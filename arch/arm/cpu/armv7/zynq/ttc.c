/*
 * (C) Copyright 2016
 * Brandon Streiff, National Instruments, brandon.streiff@ni.com
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
#include <asm/arch/ttc.h>
#include <asm/io.h>

/*
 * Zynq has two Triple Timer Counter (TTC) blocks. Each has three
 * sets of timer/counter registers.
 */
static const u32 ttc_baseaddr[ZYNQ_TTC_COUNT] = {
	XPSS_TTC0_BASEADDR,
	XPSS_TTC0_BASEADDR + 0x0004,
	XPSS_TTC0_BASEADDR + 0x0008,
	XPSS_TTC1_BASEADDR,
	XPSS_TTC1_BASEADDR + 0x0004,
	XPSS_TTC1_BASEADDR + 0x0008,
};

#ifdef CONFIG_USE_IRQ
static const int ttc_int_id[ZYNQ_TTC_COUNT] = {
	XPSS_TTC0_0_INT_ID,
	XPSS_TTC0_1_INT_ID,
	XPSS_TTC0_2_INT_ID,
	XPSS_TTC1_0_INT_ID,
	XPSS_TTC1_1_INT_ID,
	XPSS_TTC1_2_INT_ID,
};
#endif

#define TTC_CLK_CNTRL		0x0000
#define TTC_CNT_CNTRL		0x000C
#define TTC_COUNT_VALUE		0x0018
#define TTC_INTERVAL_VAL	0x0024
#define TTC_MATCH_0		0x0030
#define TTC_MATCH_1		0x003C
#define TTC_MATCH_2		0x0048
#define TTC_ISR			0x0054
#define TTC_IER			0x0060
#define TTC_EVT_CNTRL		0x006C
#define TTC_EVR			0x0078

#define CLK_CNTRL_PS_VAL_SHIFT	1
#define CLK_CNTRL_PS_EN		0x1

#define CNT_CNTRL_DIS		(1 << 0) /* Disable counter */
#define CNT_CNTRL_INT		(1 << 1) /* Interval mode */
#define CNT_CNTRL_DEC		(1 << 2) /* Decrement */
#define CNT_CNTRL_MATCH		(1 << 3) /* Match mode */
#define CNT_CNTRL_RST		(1 << 4) /* Reset */
#define CNT_CNTRL_EN_WAVE	(1 << 5) /* Waveform output enable */
#define CNT_CNTRL_POL_WAVE	(1 << 6) /* Waveform polarity */

#define IXR_INTERVAL		(1 << 0) /* Interval interrupt */
#define IXR_MATCH_0		(1 << 1) /* Match 0 interrupt */
#define IXR_MATCH_1		(1 << 2) /* Match 1 interrupt */
#define IXR_MATCH_2		(1 << 3) /* Match 2 interrupt */
#define IXR_CNT_OVR		(1 << 4) /* Counter overflow */
#define IXR_EV			(1 << 5) /* Event timer overflow */

static inline u32 ttc_readl(u32 ttcid, u32 offset)
{
	return readl(ttc_baseaddr[ttcid]+offset);
}

static inline u32 ttc_writel(u32 ttcid, u32 value, u32 offset)
{
	return writel(value, ttc_baseaddr[ttcid]+offset);
}


int zynq_ttc_set_prescaler(u32 ttcid, u32 prescale)
{
	u32 tmp;

	if (ttcid > ZYNQ_TTC_COUNT)
		return -1;
	if (prescale > 16)
		return -1;

	/* Assume we never use EXT_EDGE or SRC. */
	if (prescale == 0)
		tmp = 0x00000000;
	else {
		tmp = ((prescale-1) << CLK_CNTRL_PS_VAL_SHIFT) |
		      CLK_CNTRL_PS_EN;
	}

	ttc_writel(ttcid, tmp, TTC_CLK_CNTRL);

	return 0;
}

int zynq_ttc_set_interval(u32 ttcid, u16 interval)
{
	if (ttcid > ZYNQ_TTC_COUNT)
		return -1;

	ttc_writel(ttcid, (u32)interval, TTC_INTERVAL_VAL);
	return 0;
}

int zynq_ttc_start(u32 ttcid, u32 mode)
{
	u32 tmp;

	if (ttcid > ZYNQ_TTC_COUNT)
		return -1;

	tmp = ttc_readl(ttcid, TTC_CNT_CNTRL);
	/* Configure interval/decrement bits based on "mode". */
	tmp = (tmp & ~(CNT_CNTRL_INT|CNT_CNTRL_DEC)) | mode;
	/* Unset "disable" */
	tmp &= ~(CNT_CNTRL_DIS);
	/* Reset the counter */
	tmp |= CNT_CNTRL_RST;
	/* Let's go! */
	ttc_writel(ttcid, tmp, TTC_CNT_CNTRL);

	return 0;
}

int zynq_ttc_stop(u32 ttcid)
{
	u32 tmp;

	if (ttcid > ZYNQ_TTC_COUNT)
		return -1;

	tmp = ttc_readl(ttcid, TTC_CNT_CNTRL);
	tmp |= CNT_CNTRL_DIS;
	ttc_writel(ttcid, tmp, TTC_CNT_CNTRL);
}

#ifdef CONFIG_USE_IRQ
struct _irq_handler {
	void		*data;
	void		(*func)(void *data);
};

struct _ttc_irq_handlers {
	struct _irq_handler	interval;
	/* TODO: If you need the other interrupts, add them here. */
};

static struct _ttc_irq_handlers handlers[ZYNQ_TTC_COUNT];

static void zynq_ttc_handler(void *ttcid_as_ptr)
{
	u32 ttcid = (u32)ttcid_as_ptr;
	struct _ttc_irq_handlers *h = &(handlers[ttcid]);
	u32 intmask;

	/* Read the interrupt mask, which also acks the interrupts */
	intmask = ttc_readl(ttcid, TTC_ISR);
	if (intmask & IXR_INTERVAL) {
		if (h->interval.func)
			h->interval.func(h->interval.data);
	}
}

int zynq_ttc_set_interval_handler(u32 ttcid, interrupt_handler_t handle_irq,
				  void *data)
{
	u32 mask;

	if (ttcid > ZYNQ_TTC_COUNT)
		return -1;

	handlers[ttcid].interval.data = data;
	handlers[ttcid].interval.func = handle_irq;

	mask = ttc_readl(ttcid, TTC_IER);
	if (handle_irq)
		mask |= IXR_INTERVAL;
	else
		mask &= ~(IXR_INTERVAL);
	ttc_writel(ttcid, mask, TTC_IER);

	/* We need the handler installed if any interrupt bits are cfg'd. */
	if (mask)
		irq_install_handler(ttc_int_id[ttcid], zynq_ttc_handler,
				    (void *)ttcid);
	else
		irq_free_handler(ttc_int_id[ttcid]);
}

#endif /* CONFIG_USE_IRQ */
