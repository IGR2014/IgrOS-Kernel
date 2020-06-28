////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.cpp
//	Date:	28 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <port.hpp>
#include <irq.hpp>

#include <drivers/vmem.hpp>
#include <drivers/pit.hpp>

#include <klib/kmath.hpp>
#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Ticks count
	static dword_t	PIT_TICKS	= 0u;
	// Current frequency
	static word_t	PIT_FREQUENCY	= 0u;
	// Current divisor
	static word_t	PIT_DIVISOR	= 1u;


        // Setup PIT frequency
	void pitSetupFrequency(const word_t frequency) noexcept {

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		auto divisor	= PIT_MAIN_FREQUENCY / frequency;
		// Save divisor value
		PIT_DIVISOR	= divisor;
		// Save current real frequency value
		PIT_FREQUENCY	= PIT_MAIN_FREQUENCY / divisor;

		klib::kprintf(	u8"REAL frequency set to: %d Hz.",
				PIT_FREQUENCY);

		// Tell pit we want to change divisor for channel 0
		inPort8(PIT_CONTROL,	0x36);
		// Set divisor (LOW first, then HIGH)
		inPort8(PIT_CHANNEL_0,	divisor & 0xFF);
		inPort8(PIT_CHANNEL_0,	(divisor & 0xFF00) >> 8);

        }


	// Get expired ticks
	quad_t pitGetTicks() noexcept {

		// Send latch command for channel 0;
		inPort8(PIT_CONTROL, 0x00);
		// Get number of elapsed ticks since last IRQ
		auto lowByte		= outPort8(PIT_CHANNEL_0);
		auto highByte		= outPort8(PIT_CHANNEL_0);
		// Total elapsed ticks value
		auto elapsedSinceIRQ	= (highByte << 8) | lowByte;
		// Return full expired ticks count
		return PIT_TICKS * PIT_DIVISOR + elapsedSinceIRQ;

	}


	// PIT interrupt (#0) handler
	void pitInterruptHandler(const taskRegs_t*) noexcept {

		// Output every N-th tick were N = frequency
		if (0u == (++PIT_TICKS % PIT_FREQUENCY)) {

			auto elapsed		= pitGetTicks();
			auto res		= klib::kudivmod(elapsed, PIT_MAIN_FREQUENCY);
			auto nanoseconds	= dword_t(res.reminder);
			auto seconds		= dword_t(res.quotient);
			auto minutes		= seconds / 60;
			auto hours		= minutes / 60;

			klib::kprintf(	u8"IRQ #%d\t[PIT]\r\n"
					u8"Time:\t%d:%d:%d.%d (~1 sec.)\r\n",
					arch::irqNumber_t::PIT,
					hours % 24,
					minutes % 60,
					seconds % 60,
					nanoseconds);

		}

	}


	// Setup programmable interrupt timer
	void pitSetup() noexcept {

		// Setup PIT frequency to 100 HZ
		pitSetupFrequency(PIT_DEFAULT_FREQUENCY);
		// Install PIT interrupt handler
		irq::install(arch::irqNumber_t::PIT, arch::pitInterruptHandler);
		// Mask PIT interrupts
		irq::mask(arch::irqNumber_t::PIT);

		// Print buffer
		klib::kprintf(	"IRQ #%d [PIT] installed\r\n",
				arch::irqNumber_t::PIT);

	}


}	// namespace arch

