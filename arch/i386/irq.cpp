////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.cpp
//	Date:	21 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/i386/isr.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/port.hpp>

#include <klib/kprint.hpp>


// i386 namespace
namespace igros::i386 {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Enable interrupts
		constexpr void	irqEnable() noexcept;
		// Disable interrupts
		constexpr void	irqDisable() noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Init IRQ
	void irq::init() noexcept {
		// Restart PIC`s
		inPort8(PIC_MASTER_CONTROL, 0x11);
		inPort8(PIC_SLAVE_CONTROL, 0x11);
		// Remap IRQ`s because of exceptions
		inPort8(PIC_MASTER_DATA, 0x20);
		inPort8(PIC_SLAVE_DATA, 0x28);
		// Setup PIC`s cascading
		inPort8(PIC_MASTER_DATA, 0x04);
		inPort8(PIC_SLAVE_DATA, 0x02);
		// Setup done
		inPort8(PIC_MASTER_DATA, 0x01);
		inPort8(PIC_SLAVE_DATA, 0x01);
		// Unmask all interrupts
		irq::setMask();
	}


	// Enable interrupts
	void irq::enable() noexcept {
		irqEnable();

	}

	// Disable interrupts
	void irq::disable() noexcept {
		irqDisable();
	}


	// Mask interrupt
	void irq::mask(const irq_t irqNumber) noexcept {
		// Chech if it's hardware interrupt
		if (static_cast<dword_t>(irqNumber) < 16u) {
			// Set interrupts mask
			irq::setMask(irq::getMask() & ~(1u << static_cast<dword_t>(irqNumber)));
		}
	}

	// Unmask interrupt
	void irq::unmask(const irq_t irqNumber) noexcept {
		// Chech if it's hardware interrupt
		if (static_cast<dword_t>(irqNumber) < 16u) {
			// Set interrupts mask
			irq::setMask(irq::getMask() | (1u << static_cast<dword_t>(irqNumber)));
		}
	}


	// Set interrupts mask
	void irq::setMask(const word_t mask) noexcept {
		// Set Master controller mask
		inPort8(PIC_MASTER_DATA, static_cast<byte_t>(mask & 0xFF));
		// Set Slave controller mask
		inPort8(PIC_SLAVE_DATA, static_cast<byte_t>((mask >> 8) & 0xFF));
	}

	// Get interrupts mask
	[[nodiscard]] word_t irq::getMask() noexcept {
		// Read slave PIC current mask
		auto mask	= static_cast<word_t>(outPort8(PIC_SLAVE_DATA)) << 8;
		// Read master PIC current mask
		mask		|= outPort8(PIC_MASTER_DATA);
		// Return IRQ mask
		return mask;
	}


	// Install handler
	void irq::install(const irq_t number, const isr_t handler) noexcept {
		// Install ISR
		isrHandlerInstall(static_cast<dword_t>(number) + IRQ_OFFSET, handler);
	}

	// Uninstall handler
	void irq::uninstall(const irq_t number) noexcept {
		// Uninstall ISR
		isrHandlerUninstall(static_cast<dword_t>(number) + IRQ_OFFSET);
	}


	// Send EOI (IRQ done)
	void irq::eoi(const irq_t number) noexcept {
		// If it`s an interrupt
		if (static_cast<dword_t>(number) >= IRQ_OFFSET) {
			// Notify slave PIC if needed
			if (static_cast<dword_t>(number) > 39) {
				inPort8(PIC_SLAVE_CONTROL, 0x20);
			} else {
				// Notify master PIC
				inPort8(PIC_MASTER_CONTROL, 0x20);
			}
		}
	}


}	// namespace igros::i386

