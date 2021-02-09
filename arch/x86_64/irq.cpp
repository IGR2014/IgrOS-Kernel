////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.cpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/x86_64/isr.hpp>
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/io.hpp>

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


// x86_64 platform
namespace igros::x86_64 {


	// Init IRQ
	void irqx86_64::init() noexcept {
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
		irqx86_64::setMask();
	}


	// Enable interrupts
	void irqx86_64::enable() noexcept {
		irqEnable();

	}

	// Disable interrupts
	void irqx86_64::disable() noexcept {
		irqDisable();
	}


	// Mask interrupt
	void irqx86_64::mask(const irqx86_64_t number) noexcept {
		// Chech if it's hardware interrupt
		if (static_cast<dword_t>(number) < 16U) {
			// Set interrupts mask
			irqx86_64::setMask(irqx86_64::getMask() & ~(1U << static_cast<dword_t>(number)));
		}
	}

	// Unmask interrupt
	void irqx86_64::unmask(const irqx86_64_t number) noexcept {
		// Chech if it's hardware interrupt
		if (static_cast<dword_t>(number) < 16U) {
			// Set interrupts mask
			irqx86_64::setMask(irqx86_64::getMask() | (1u << static_cast<dword_t>(number)));
		}
	}


	// Set interrupts mask
	void irqx86_64::setMask(const word_t mask) noexcept {
		// Set Master controller mask
		inPort8(PIC_MASTER_DATA, static_cast<byte_t>(mask & 0xFF));
		// Set Slave controller mask
		inPort8(PIC_SLAVE_DATA, static_cast<byte_t>((mask >> 8) & 0xFF));
	}

	// Get interrupts mask
	[[nodiscard]]
	word_t irqx86_64::getMask() noexcept {
		// Read slave PIC current mask
		auto mask	= static_cast<word_t>(outPort8(PIC_SLAVE_DATA)) << 8;
		// Read master PIC current mask
		mask		|= outPort8(PIC_MASTER_DATA);
		// Return IRQ mask
		return mask;
	}


	// Install handler
	void irqx86_64::install(const irqx86_64_t number, const isrx86_64_t handler) noexcept {
		// Install ISR
		isrHandlerInstall(static_cast<dword_t>(number) + IRQ_OFFSET, handler);
	}

	// Uninstall handler
	void irqx86_64::uninstall(const irqx86_64_t number) noexcept {
		// Uninstall ISR
		isrHandlerUninstall(static_cast<dword_t>(number) + IRQ_OFFSET);
	}


	// Send EOI (IRQ done)
	void irqx86_64::eoi(const irqx86_64_t number) noexcept {
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


}	// namespace igros::x86_64

