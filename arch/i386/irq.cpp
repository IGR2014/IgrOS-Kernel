////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.cpp
//	Date:	10 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/isr.hpp>
#include <arch/irq.hpp>
#include <arch/port.hpp>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Init interrupts
	void irqInit() {

		// Restart PIC`s
		outPort8(PIC_MASTER_CONTROL, 0x11);
		outPort8(PIC_SLAVE_CONTROL, 0x11);
		// Remap IRQ`s because of exceptions
		outPort8(PIC_MASTER_DATA, 0x20);
		outPort8(PIC_SLAVE_DATA, 0x28);
		// Setup PIC`s cascading
		outPort8(PIC_MASTER_DATA, 0x04);
		outPort8(PIC_SLAVE_DATA, 0x02);
		// Setup done
		outPort8(PIC_MASTER_DATA, 0x01);
		outPort8(PIC_SLAVE_DATA, 0x01);

		// Unmask all interrupts
		irqMaskSet(0xFFFF);

	}

	// Mask interrupts
	void irqMask(const irqNumber_t irq) {

		// Chech if it's hardware interrupt
		if (dword_t(irq) < 16) {
			// Set interrupts mask
			irqMaskSet(irqMaskGet() & ~(1 << dword_t(irq)));
		}

	}

	// Set interrupts mask
	void irqMaskSet(const word_t mask) {
		// Set Master controller mask
		outPort8(PIC_MASTER_DATA, byte_t(mask & 0xFF));
		// Set Slave controller mask
		outPort8(PIC_SLAVE_DATA, byte_t((mask & 0xFF00) >> 8));
	}

	// Get interrupts mask
	word_t irqMaskGet() {
		// Read slave PIC current mask
		word_t mask	= word_t(inPort8(PIC_SLAVE_DATA)) << 8;
		// Read master PIC current mask
		mask		|= inPort8(PIC_MASTER_DATA);
		// Return IRQ mask
		return mask;
	}

	// Install handler
	void irqHandlerInstall(irqNumber_t irqNumber, isrHandler_t handler) {
		// Install ISR
		isrHandlerInstall(dword_t(irqNumber) + IRQ_OFFSET, handler);
	}

	// Uninstall handler
	void irqHandlerUninstall(irqNumber_t irqNumber) {
		// Uninstall ISR
		isrHandlerUninstall(dword_t(irqNumber) + IRQ_OFFSET);
	}


}	// namespace arch

