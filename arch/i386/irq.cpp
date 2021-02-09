////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.cpp
//	Date:	03 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/i386/isr.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/io.hpp>

#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Enable interrupts
	inline void	irqEnable() noexcept;
	// Disable interrupts
	inline void	irqDisable() noexcept;

#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 namespace
namespace igros::i386 {


	// Init IRQ
	void irqi386::init() noexcept {
		// Restart PIC`s
		inPort8(PIC_MASTER_CONTROL,	0x11);
		inPort8(PIC_SLAVE_CONTROL,	0x11);
		// Remap IRQ`s because of exceptions
		inPort8(PIC_MASTER_DATA,	0x20);
		inPort8(PIC_SLAVE_DATA,		0x28);
		// Setup PIC`s cascading
		inPort8(PIC_MASTER_DATA,	0x04);
		inPort8(PIC_SLAVE_DATA,		0x02);
		// Setup done
		inPort8(PIC_MASTER_DATA,	0x01);
		inPort8(PIC_SLAVE_DATA,		0x01);
		// Unmask all interrupts
		irqi386::setMask();
	}


	// Enable interrupts
	void irqi386::enable() noexcept {
		irqEnable();

	}

	// Disable interrupts
	void irqi386::disable() noexcept {
		irqDisable();
	}


	// Mask interrupt
	void irqi386::mask(const irqi386_t irqNumber) noexcept {
		// Chech if it's hardware interrupt
		if (static_cast<dword_t>(irqNumber) < 16U) {
			// Set interrupts mask
			irqi386::setMask(irqi386::getMask() & ~(1U << static_cast<dword_t>(irqNumber)));
		}
	}

	// Unmask interrupt
	void irqi386::unmask(const irqi386_t irqNumber) noexcept {
		// Chech if it's hardware interrupt
		if (static_cast<dword_t>(irqNumber) < 16U) {
			// Set interrupts mask
			irqi386::setMask(irqi386::getMask() | (1U << static_cast<dword_t>(irqNumber)));
		}
	}


	// Set interrupts mask
	void irqi386::setMask(const word_t mask) noexcept {
		// Set Master controller mask
		inPort8(PIC_MASTER_DATA, static_cast<byte_t>(mask & 0xFF));
		// Set Slave controller mask
		inPort8(PIC_SLAVE_DATA, static_cast<byte_t>((mask >> 8) & 0xFF));
	}

	// Get interrupts mask
	[[nodiscard]]
	word_t irqi386::getMask() noexcept {
		// Read slave PIC current mask
		auto mask	= static_cast<word_t>(outPort8(PIC_SLAVE_DATA)) << 8;
		// Read master PIC current mask
		mask		|= outPort8(PIC_MASTER_DATA);
		// Return IRQ mask
		return mask;
	}


	// Send EOI (IRQ done)
	void irqi386::eoi(const irqi386_t number) noexcept {
		// If it`s an interrupt
		if (static_cast<dword_t>(number) >= IRQ_OFFSET) {
			// Notify slave PIC if needed
			if (static_cast<dword_t>(number) > 39U) {
				// Notify slave PIC
				inPort8(PIC_SLAVE_CONTROL, 0x20);
			} else {
				// Notify master PIC
				inPort8(PIC_MASTER_CONTROL, 0x20);
			}
		}
	}


}	// namespace igros::i386

