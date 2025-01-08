/*
 * Derived from code:
 * 
 * https://github.com/swarren/rpi-3-aarch64-demo
 * Copyright (C) 2012 Vikram Narayananan <vikram186@gmail.com>
 * (C) Copyright 2012-2016 Stephen Warren
 * Copyright (C) 1996-2000 Russell King
 *
 * SPDX-License-Identifier:	GPL-2.0
 *
 * https://github.com/dwelch67/raspberrypi/tree/master/armjtag
 * Copyright (c) 2012 David Welch dwelch@dwelch.com
 */

#ifndef _RPI4HW_H
#define _RPI4HW_H

#include <stdint.h>

#define PERI_BASE				(0xFE000000)

#define __arch_getl(a)		(*(volatile uint32_t *)(a))
#define __arch_putl(v,a)	(*(volatile uint32_t *)(a) = (v))

#define dmb()				__asm__ __volatile__ ("dmb st" : : : "memory")
#define nop()				__asm__ __volatile__ ("nop")
#define __iormb()			dmb()
#define __iowmb()			dmb()

// PL011
// http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183f/DDI0183.pdf
#define UART_BASE				(PERI_BASE + 0x00201000)
#define UART_UARTDR	(UART_BASE + 0x000)
#define UART_UARTRSR	(UART_BASE + 0x004)
#define UART_UARTFR	(UART_BASE + 0x018)
#define UART_UARTIBRD	(UART_BASE + 0x024)
#define UART_UARTFBRD	(UART_BASE + 0x028)
#define UART_UARTLCR_H	(UART_BASE + 0x02c)
#define UART_UARTCR	(UART_BASE + 0x030)

enum {
    GPFSEL1         = PERI_BASE + 0x200004,
    GPPUD           = PERI_BASE + 0x200094,
    GPPUDCLK0          = PERI_BASE + 0x200098
   // GPPUPPDN0       = PERI_BASE + 0x2000E4
};

enum {
    UART0_BASE      = PERI_BASE + 0x201000,
    UART0_DR        = UART0_BASE + 0,
    UART0_FR        = UART0_BASE + 0x18,
    UART0_ILPR      = UART0_BASE + 0x20,
    UART0_IBRD		= UART0_BASE + 0x24,
	UART0_FBRD		= UART0_BASE + 0x28,
	UART0_LCRH		= UART0_BASE + 0x2c,
	UART0_CR		= UART0_BASE + 0x30,
	UART0_IFLS		= UART0_BASE + 0x34,
	UART0_IMSC		= UART0_BASE + 0x38,
	UART0_RIS		= UART0_BASE + 0x3c,
	UART0_MIS		= UART0_BASE + 0x40,
	UART0_ICR		= UART0_BASE + 0x44,
	UART0_DMACR		= UART0_BASE + 0x48,
	UART0_ITCR		= UART0_BASE + 0x80,
	UART0_ITIP		= UART0_BASE + 0x84,
	UART0_ITOP		= UART0_BASE + 0x88,
	UART0_TDR		= UART0_BASE + 0x8c
};


static inline uint32_t readl(uint64_t addr)
{
	uint32_t value = __arch_getl(addr);
	__iormb();
	return value;
}


static inline void writel(uint64_t addr, uint32_t value)
{
	__arch_putl(value, addr);
	__iowmb();
}

#endif // _RPI4HW_H

