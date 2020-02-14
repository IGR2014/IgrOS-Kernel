////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.cpp
//	Date:	13 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <isr.hpp>
#include <irq.hpp>
#include <port.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


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
	void irq::mask(const irqNumber_t irqNumber) noexcept {
		// Chech if it's hardware interrupt
		if (dword_t(irqNumber) < 16u) {
			// Set interrupts mask
			irq::setMask(irq::getMask() & ~(1u << dword_t(irqNumber)));
		}
	}

	// Unmask interrupt
	void irq::unmask(const irqNumber_t irqNumber) noexcept {
		// Chech if it's hardware interrupt
		if (dword_t(irqNumber) < 16u) {
			// Set interrupts mask
			irq::setMask(irq::getMask() | (1u << dword_t(irqNumber)));
		}
	}


	// Set interrupts mask
	void irq::setMask(const word_t mask) noexcept {
		// Set Master controller mask
		inPort8(PIC_MASTER_DATA, byte_t(mask & 0xFF));
		// Set Slave controller mask
		inPort8(PIC_SLAVE_DATA, byte_t((mask & 0xFF00) >> 8));
	}

	// Get interrupts mask
	[[nodiscard]] word_t irq::getMask() noexcept {
		// Read slave PIC current mask
		auto mask	= word_t(outPort8(PIC_SLAVE_DATA)) << 8;
		// Read master PIC current mask
		mask		|= outPort8(PIC_MASTER_DATA);
		// Return IRQ mask
		return mask;
	}


}	// namespace arch

