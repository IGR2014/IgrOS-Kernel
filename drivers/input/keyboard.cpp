////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Keyboard generic handling
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


// IgrOS-Kernel arch
#include <arch/io.hpp>
#include <arch/irq.hpp>
#include <arch/register.hpp>
#include <arch/types.hpp>
// IgrOS-Kernel drivers
#include <drivers/input/keyboard.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>
#include <klib/kPrint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Keyboard ports
	constexpr auto KEYBOARD_CONTROL {static_cast<port_t>(0x0064_u16)}; ///< Controller status/command port
	constexpr auto KEYBOARD_DATA {static_cast<port_t>(0x0060_u16)};	   ///< Data port

	/// @brief Anonymous namespace
	namespace {

		/// @brief Controller status register bits
		enum class KEYBOARD_STATUS : igros_byte_t {
			OUTPUT_FULL	= 0x01_u8,	///< Output buffer full (data port has byte)
		};

	}	// namespace


	/// @brief Keyboard interrupt (#1) handler
	/// @param regs Registers saved on interrupt (unused)
	static void keyboardInterruptHandler([[maybe_unused]] const register_t* regs) noexcept {
		// Check keyboard data port
		if (klib::kFlags<KEYBOARD_STATUS>::from(io::readPort8(KEYBOARD_CONTROL)).isSet(KEYBOARD_STATUS::OUTPUT_FULL)) [[likely]] {
			// Read keyboard data
			const auto keyCode = io::readPort8(KEYBOARD_DATA);
			klib::kprintf(
				"IRQ #%u\t[Keyboard]\n"
				"Key:\t%s\n"
				"Code:\t0x%x\n",
				static_cast<igros_dword_t>(irq_t::KEYBOARD),
				(0x00_u8 != (keyCode & 0x80_u8)) ? "RELEASED" : "PRESSED",
				keyCode
			);
		}
	}


	// Setup keyboard function
	[[nodiscard]]
	auto keyboardSetup() noexcept -> bool {

		// Install keyboard interrupt handler
		irq::install<irq_t::KEYBOARD, keyboardInterruptHandler>();
		// Unmask Keyboard interrupts
		irq::unmask(irq_t::KEYBOARD);

		// Success
		return true;
	}


} // namespace igros::arch

