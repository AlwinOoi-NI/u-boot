#ifndef __GIC_H__
#define __GIC_H__

/* Register offsets for the ARM generic interrupt controller (GIC) */

#define GIC_DIST_OFFSET		0x1000
#define GIC_CPU_OFFSET_A9	0x0100

/* Distributor Registers */
#define GICD_CTLR		0x0000	/* Distributor Control */
#define GICD_TYPER		0x0004	/* Interrupt Controller Type */
#define GICD_IIDR		0x0008	/* Implementer Id */
#define GICD_IGROUPRn		0x0080	/* Interrupt Security */
#define GICD_ISENABLERn		0x0100	/* Interrupt Set-Enable */
#define GICD_ICENABLERn		0x0180	/* Interrupt Clear-Enable */
#define GICD_ISPENDRn		0x0200	/* Interrupt Set-Pending */
#define GICD_ICPENDRn		0x0280	/* Interrupt Clear-Pending */
#define GICD_ISACTIVERn		0x0300	/* Interrupt Set-Active */
#define GICD_ICACTIVERn		0x0380	/* Interrupt Clear-Active */
#define GICD_IPRIORITYRn	0x0400	/* Interrupt Priority */
#define GICD_ITARGETSRn		0x0800	/* Interrupt Processor Targets */
#define GICD_ICFGR		0x0c00	/* Interrupt Configuration */
#define GICD_SGIR		0x0f00	/* Software Generated Interrupt */

/* Cpu Interface Memory Mapped Registers */
#define GICC_CTLR		0x0000	/* CPU Interface Control */
#define GICC_PMR		0x0004	/* Interrupt Priority Mask */
#define GICC_BPR		0x0008	/* Binary Point */
#define GICC_IAR		0x000C	/* Interrupt Acknowledge */
#define GICC_EOIR		0x0010	/* End Of Interrupt */
#define GICC_RPR		0x0014	/* Running Priority */
#define GICC_HPPIR		0x0018	/* Highest Pending Interrupt */
#define GICC_ABPR		0x001c	/* Aliased Non-secure Binary Point */
#define GICC_IIDR		0x00fc	/* CPU Interface Inplementer Id */

#endif /* __GIC_H__ */
