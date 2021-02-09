////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.cpp
//	Date:	08 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/types.hpp>
#include <arch/io.hpp>
#include <arch/irq.hpp>
#include <arch/register.hpp>

#include <drivers/clock/pit.hpp>

#include <klib/kmath.hpp>
#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// PIT ports
	constexpr auto PIT_CONTROL	= static_cast<io::port_t>(0x0043);
	constexpr auto PIT_CHANNEL_0	= static_cast<io::port_t>(0x0040);
	constexpr auto PIT_CHANNEL_1	= static_cast<io::port_t>(PIT_CHANNEL_0 + 1U);
	constexpr auto PIT_CHANNEL_2	= static_cast<io::port_t>(PIT_CHANNEL_1 + 1U);


	// Ticks count
	static dword_t	PIT_TICKS	= 0U;
	// Current frequency
	static word_t	PIT_FREQUENCY	= 0U;
	// Current divisor
	static word_t	PIT_DIVISOR	= 1U;


        // Setup PIT frequency
	void pitSetupFrequency(const word_t frequency) noexcept {

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		const auto divisor = static_cast<word_t>(PIT_MAIN_FREQUENCY / frequency);
		// Save divisor value
		PIT_DIVISOR	= divisor;
		// Save current real frequency value
		PIT_FREQUENCY	= PIT_MAIN_FREQUENCY / divisor;

		klib::kprintf(
			u8"REAL frequency set to: %d Hz.",
			PIT_FREQUENCY
		);

		// Tell pit we want to change divisor for channel 0
		io::get().writePort8(PIT_CONTROL,	0x36);
		// Set divisor (LOW first, then HIGH)
		io::get().writePort8(PIT_CHANNEL_0,	divisor & 0xFF);
		io::get().writePort8(PIT_CHANNEL_0,	(divisor & 0xFF00) >> 8);

        }


	// Get expired ticks
	quad_t pitGetTicks() noexcept {
		// Send latch command for channel 0;
		io::get().writePort8(PIT_CONTROL, 0x00);
		// Get number of elapsed ticks since last IRQ
		const auto loByte = io::get().readPort8(PIT_CHANNEL_0);
		const auto hiByte = io::get().readPort8(PIT_CHANNEL_0);
		// Total elapsed ticks value
		const auto elapsedSinceIRQ = (hiByte << 8) | loByte;
		// Return full expired ticks count
		return PIT_TICKS * PIT_DIVISOR + elapsedSinceIRQ;
	}


	// PIT interrupt (#0) handler
	void pitInterruptHandler(const register_t* regs) noexcept {
		// Output every N-th tick were N = frequency
		if (0u == (++PIT_TICKS % PIT_FREQUENCY)) {
			// Current time to HH:MM:SS.zzz
			const auto elapsed	= pitGetTicks();
			const auto res		= klib::kudivmod(elapsed, PIT_MAIN_FREQUENCY);
			const auto nanoseconds	= static_cast<dword_t>(res.reminder);
			const auto seconds	= static_cast<dword_t>(res.quotient);
			const auto minutes	= seconds / 60U;
			const auto hours	= minutes / 60U;
			// Debug date/time
			klib::kprintf(
				u8"IRQ #%d\t[PIT]\r\n"
				u8"Time:\t%02d:%02d:%02d.%03d (~1 sec.)\r\n",
				irq::irq_t::PIT,
				hours % 24U,
				minutes % 60U,
				seconds % 60U,
				nanoseconds
			);
		}
		// IRQ EOI
		irq::get().eoi(static_cast<const irq::irq_t>(regs->number));
	}


	// Setup programmable interrupt timer
	void pitSetup() noexcept {

		// Setup PIT frequency to 100 HZ
		pitSetupFrequency(PIT_DEFAULT_FREQUENCY);

		// Install PIT interrupt handler
		irq::get().install(irq::irq_t::PIT, pitInterruptHandler);
		// Mask PIT interrupts
		irq::get().mask(irq::irq_t::PIT);

		// Print buffer
		klib::kprintf(
			"IRQ #%d [PIT] installed\r\n",
			irq::irq_t::PIT
		);

	}


}	// namespace igros::arch

