////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.cpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch i386
#include <arch/i386/io.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/isr.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Enable interrupts
	void	irqEnable() noexcept;
	// Disable interrupts
	void	irqDisable() noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 namespace
namespace igros::i386 {


	// Init IRQ
	void irq::init() noexcept {
		// Restart PIC`s
		::inPort8(PIC_MASTER_CONTROL,	0x11_u8);
		::inPort8(PIC_SLAVE_CONTROL,	0x11_u8);
		// Remap IRQ`s because of exceptions
		::inPort8(PIC_MASTER_DATA,	0x20_u8);
		::inPort8(PIC_SLAVE_DATA,	0x28_u8);
		// Setup PIC`s cascading
		::inPort8(PIC_MASTER_DATA,	0x04_u8);
		::inPort8(PIC_SLAVE_DATA,	0x02_u8);
		// Setup done
		::inPort8(PIC_MASTER_DATA,	0x01_u8);
		::inPort8(PIC_SLAVE_DATA,	0x01_u8);
		// Unmask all interrupts
		irq::setMask();
	}


	// Enable interrupts
	void irq::enable() noexcept {
		::irqEnable();

	}

	// Disable interrupts
	void irq::disable() noexcept {
		::irqDisable();
	}


	// Mask interrupt
	void irq::mask(const irq_t irqNumber) noexcept {
		// Chech if it's hardware interrupt
		if (static_cast<igros_dword_t>(irqNumber) < 16_u32) [[likely]] {
			// Set interrupts mask
			irq::setMask(static_cast<igros_word_t>(irq::getMask() & ~(1_u16 << static_cast<igros_dword_t>(irqNumber))));
		}
	}

	// Unmask interrupt
	void irq::unmask(const irq_t irqNumber) noexcept {
		// Chech if it's hardware interrupt
		if (static_cast<igros_dword_t>(irqNumber) < 16_u32) [[likely]] {
			// Set interrupts mask
			irq::setMask(static_cast<igros_word_t>(irq::getMask() | (1_u16 << static_cast<igros_dword_t>(irqNumber))));
		}
	}


	// Set interrupts mask
	void irq::setMask(const igros_word_t mask) noexcept {
		// Set Master controller mask
		::inPort8(PIC_MASTER_DATA,	static_cast<igros_byte_t>(mask & 0x00FF_u16));
		// Set Slave controller mask
		::inPort8(PIC_SLAVE_DATA,	static_cast<igros_byte_t>((mask >> 8) & 0x00FF_u16));
	}

	// Get interrupts mask
	[[nodiscard]]
	igros_word_t irq::getMask() noexcept {
		// Read slave PIC current mask
		auto mask	{static_cast<igros_word_t>(::outPort8(PIC_SLAVE_DATA)) << 8};
		// Read master PIC current mask
		mask		|= ::outPort8(PIC_MASTER_DATA);
		// Return IRQ mask
		return static_cast<igros_word_t>(mask);
	}


	// Send EOI (IRQ done)
	void irq::eoi(const irq_t number) noexcept {
		// If it`s an interrupt
		if (static_cast<igros_dword_t>(number) >= IRQ_OFFSET) [[likely]] {
			// Notify slave PIC if needed
			if (static_cast<igros_dword_t>(number) > 39_u32) {
				// Notify slave PIC
				::inPort8(PIC_SLAVE_CONTROL, 0x20_u8);
			} else {
				// Notify master PIC
				::inPort8(PIC_MASTER_CONTROL, 0x20_u8);
			}
		}
	}


}	// namespace igros::i386

