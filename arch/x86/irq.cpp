////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Interrupts low-level operations
///	@date		16 Dec 2022
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


// IgrOS-Kernel arch x86
#include <arch/x86/io.hpp>
#include <arch/x86/irq.hpp>
#include <arch/x86/isr.hpp>
// IgrOS-Kernel library
#include <klib/kPrint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	/// @brief Enable interrupts
	void	irqEnable() noexcept;
	/// @brief Disable interrupts
	void	irqDisable() noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86 namespace
namespace igros::x86 {


	// Init IRQ
	void irq::init() noexcept {
		// Restart PIC`s
		::portWrite8(PIC_MASTER_CONTROL,	0x11_u8);
		::portWrite8(PIC_SLAVE_CONTROL,		0x11_u8);
		// Remap IRQ`s because of exceptions
		::portWrite8(PIC_MASTER_DATA,		0x20_u8);
		::portWrite8(PIC_SLAVE_DATA,		0x28_u8);
		// Setup PIC`s cascading
		::portWrite8(PIC_MASTER_DATA,		0x04_u8);
		::portWrite8(PIC_SLAVE_DATA,		0x02_u8);
		// Setup done
		::portWrite8(PIC_MASTER_DATA,		0x01_u8);
		::portWrite8(PIC_SLAVE_DATA,		0x01_u8);
		// Mask all interrupts (drivers unmask their own lines)
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


	// Mask interrupt (disable line)
	void irq::mask(const irq_t number) noexcept {
		// Check if it's hardware interrupt
		if (static_cast<igros_dword_t>(number) < 16_u32) [[likely]] {
			// Set line bit in PIC mask
			irq::setMask(static_cast<igros_word_t>(static_cast<igros_dword_t>(irq::getMask()) | (1_u32 << static_cast<igros_dword_t>(number))));
		}
	}

	// Unmask interrupt (enable line)
	void irq::unmask(const irq_t number) noexcept {
		// Check if it's hardware interrupt
		if (static_cast<igros_dword_t>(number) < 16_u32) [[likely]] {
			// Clear line bit in PIC mask
			irq::setMask(static_cast<igros_word_t>(static_cast<igros_dword_t>(irq::getMask()) & ~(1_u32 << static_cast<igros_dword_t>(number))));
		}
	}


	// Set interrupts mask
	void irq::setMask(const igros_word_t mask) noexcept {
		// Mask bits in 32-bit arithmetic (16-bit value would be promoted to int)
		const auto bits {static_cast<igros_dword_t>(mask)};
		// Set Master controller mask
		::portWrite8(PIC_MASTER_DATA,	static_cast<igros_byte_t>(bits & 0x00FF_u32));
		// Set Slave controller mask
		::portWrite8(PIC_SLAVE_DATA,	static_cast<igros_byte_t>((bits >> 8) & 0x00FF_u32));
	}

	// Get interrupts mask
	[[nodiscard]]
	auto irq::getMask() noexcept -> igros_word_t {
		// Read slave PIC current mask
		const auto high	{static_cast<igros_dword_t>(::portRead8(PIC_SLAVE_DATA))};
		// Read master PIC current mask
		const auto low	{static_cast<igros_dword_t>(::portRead8(PIC_MASTER_DATA))};
		// Return IRQ mask
		return static_cast<igros_word_t>((high << 8) | low);
	}


	// Send EOI (IRQ done)
	void irq::eoi(const irq_t number) noexcept {
		// Slave PIC lines (8..15) need EOI on both controllers
		if (static_cast<igros_dword_t>(number) >= 8_u32) {
			// Notify slave PIC
			::portWrite8(PIC_SLAVE_CONTROL, 0x20_u8);
		}
		// Notify master PIC
		::portWrite8(PIC_MASTER_CONTROL, 0x20_u8);
	}


	// Check if IRQ is spurious (only lines 7 and 15 can be)
	[[nodiscard]]
	auto irq::isSpurious(const irq_t number) noexcept -> bool {
		// Spurious IRQ is reported on the lowest priority line of PIC
		if ((irq_t::LPT1 != number) && (irq_t::ATA_SECONDARY != number)) [[likely]] {
			return false;
		}
		// Command port of PIC which raised IRQ
		const auto control {(irq_t::LPT1 == number) ? PIC_MASTER_CONTROL : PIC_SLAVE_CONTROL};
		// Select In-Service Register (OCW3)
		::portWrite8(control, 0x0B_u8);
		// Line 7 of the PIC is not in service - IRQ is spurious
		return 0x00_u8 == (::portRead8(control) & 0x80_u8);
	}


}	// namespace igros::x86

