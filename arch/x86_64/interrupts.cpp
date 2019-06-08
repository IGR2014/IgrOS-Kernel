////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	interupts.cpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/taskRegs.hpp>
#include <arch/interrupts.hpp>
#include <arch/port.hpp>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Interrupt handlers
	static irqHandler_t	isrList[224] = {0};


	// Interrupts handler function
	void irqHandler(const taskRegs_t* regs) {

		// Notify slave PIC if needed
		if (regs->number > 39) {

			outPort8(PIC_SLAVE_CONTROL, 0x20);

		} else {

			// Notify master PIC
			outPort8(PIC_MASTER_CONTROL, 0x20);

		}

		// Actually it`s an exception and normaly shouldn't be there
		if (regs->number < 32) {

			return;

		}

		// Acquire irq handler from list
		irqHandler_t isr = isrList[regs->number - 32];

		// If handler exists
		if (isr) {

			// Finally handle IRQ
			isr(regs);

		} else {

			// Print buffer
			sbyte_t text[64];

			// Print message about unhandled interrupt
			vmemWrite("\r\nIRQ\t\t-> #");
			klib::kitoa(text, 64, static_cast<dword_t>(regs->number - 32));
			vmemWrite(text);
			vmemWrite("\r\nSTATE:\t\tunhandled!\r\n");
		
		}

	}


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

		// Setup done
		outPort8(PIC_MASTER_DATA, 0x00);
		outPort8(PIC_SLAVE_DATA, 0x00);

		// Unmask all interrupts
		irqMaskSet(0xFFFF);

	}

	// Mask interrupts
	void irqMask(const irqNumber_t irq) {

		// Chech if it's hardware interrupt
		if (static_cast<dword_t>(irq) < 16) {

			// Set interrupts mask
			irqMaskSet(irqMaskGet() & ~(1 << static_cast<dword_t>(irq)));

		}

	}

	// Set interrupts mask
	void irqMaskSet(const word_t mask) {

		// Set Master controller mask
		outPort8(PIC_MASTER_DATA, static_cast<byte_t>(mask & 0xFF));
		// Set Slave controller mask
		outPort8(PIC_SLAVE_DATA, static_cast<byte_t>((mask >> 8) & 0xFF));

	}

	// Get interrupts mask
	word_t irqMaskGet() {

		// Read slave PIC current mask
		word_t mask	= static_cast<word_t>(inPort8(PIC_SLAVE_DATA)) << 8;
		// Read master PIC current mask
		mask		|= inPort8(PIC_MASTER_DATA);

		return mask;

	}

	// Install handler
	void irqHandlerInstall(irqNumber_t irqNumber, irqHandler_t handler) {

		// Set handler to ISR list
		isrList[static_cast<dword_t>(irqNumber)] = handler;

	}

	// Uninstall handler
	void irqHandlerUninstall(irqNumber_t irqNumber) {

		// Set nullptr as a handler to ISR list
		isrList[static_cast<dword_t>(irqNumber)] = nullptr;

	}


}	// namespace arch
