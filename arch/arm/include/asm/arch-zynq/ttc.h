/*
 * Zynq architecture Triple Timer Counter header file
 *
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#ifndef __ASM_ARCH_ZYNQ_TTC_H
#define __ASM_ARCH_ZYNQ_TTC_H

#define ZYNQ_TTC_COUNT 6

#define ZYNQ_TTC_MODE_OVERFLOW	(0)
#define ZYNQ_TTC_MODE_INTERVAL	(1 << 1)
#define ZYNQ_TTC_MODE_DECREMENT (1 << 2)

/* Configure the prescaler to 2^(N). '0' disables prescaling. */
int zynq_ttc_set_prescaler(u32 timer, u32 prescale);

int zynq_ttc_set_interval(u32 timer, u16 interval);

int zynq_ttc_start(u32 timer, u32 mode);

int zynq_ttc_stop(u32 timer);

#ifdef CONFIG_USE_IRQ
int zynq_ttc_set_interval_handler(u32 ttcid, interrupt_handler_t handle_irq, void *data);
#endif

#endif /* __ASM_ARCH_ZYNQ_TTC_H */
