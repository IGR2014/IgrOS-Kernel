////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.cpp
//	Date:	10 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/port.hpp>
#include <arch/interrupts.hpp>

#include <drivers/vmem.hpp>
#include <drivers/pit.hpp>

#include <klib/kmath.hpp>
#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Default pit frequency (100 Hz)
	static const dword_t	PIT_DEFAULT_FREQUENCY	= 100;

	// Ticks count
	static dword_t		PIT_TICKS		= 0;
	// Current frequency
	static word_t		PIT_FREQUENCY		= 0;
	// Current divisor
	static word_t		PIT_DIVISOR		= 1;


        // Setup PIT frequency
	void pitSetupFrequency(word_t frequency) {

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		word_t divisor	= PIT_MAIN_FREQUENCY / frequency;
		// Save divisor value
		PIT_DIVISOR	= divisor;
		// Save current real frequency value
		PIT_FREQUENCY	= PIT_MAIN_FREQUENCY / divisor;

		// Print buffer
		sbyte_t text[1024];
		klib::ksprintf(	text,
				"REAL frequency set to: %d Hz.\r\n",
				PIT_FREQUENCY);
		arch::vmemWrite(text);

		// Tell pit we want to change divisor for channel 0
		outPort8(PIT_CONTROL, 0x36);
		// Set divisor (LOW first, then HIGH)
		outPort8(PIT_CHANNEL_0,	divisor & 0xFF);
		outPort8(PIT_CHANNEL_0,	(divisor & 0xFF00) >> 8);

        }


	// Get expired ticks
	quad_t pitGetTicks() {

		// Send latch command for channel 0;
		outPort8(PIT_CONTROL, 0x00);
		// Get number of elapsed ticks since last IRQ
		byte_t lowByte	= inPort8(PIT_CHANNEL_0);
		byte_t highByte	= inPort8(PIT_CHANNEL_0);
		// Total elapsed ticks value
		word_t elapsedSinceIRQ = (highByte << 8) | lowByte;
		// Return full expired ticks count
		return PIT_TICKS * PIT_DIVISOR + elapsedSinceIRQ;

	}


	// PIT interrupt (#0) handler
	void pitInterruptHandler(const taskRegs_t*) {

		// Output every N-th tick were N = frequency
		if ((++PIT_TICKS % PIT_FREQUENCY) == 0) {

			auto	elapsed		= pitGetTicks();
			auto	res		= klib::kudivmod(elapsed, PIT_MAIN_FREQUENCY);
			dword_t nanoseconds	= dword_t(res.reminder);
			dword_t seconds		= dword_t(res.quotient);
			dword_t minutes		= seconds / 60;
			dword_t hours		= minutes / 60;

			//
			sbyte_t text[1024];
			klib::ksprintf(	text,
					"IRQ #%d\t[PIT]\r\n"
					"Time:\t%d:%d:%d.%d (~1 sec.)\r\n"
					"\r\n",
					arch::irqNumber_t::PIT,
					hours % 24,
					minutes % 60,
					seconds % 60,
					nanoseconds);
			arch::vmemWrite(text);
			//

		}

	}


	// Setup programmable interrupt timer
	void pitSetup() {

		// Setup PIT frequency to 100 HZ
		pitSetupFrequency(PIT_DEFAULT_FREQUENCY);
		// Install PIT interrupt handler
		irqHandlerInstall(arch::irqNumber_t::PIT, arch::pitInterruptHandler);
		// Mask PIT interrupts
		irqMask(arch::irqNumber_t::PIT);

		// Print buffer
		sbyte_t text[1024];
		klib::ksprintf(	text,
				"IRQ #%d [PIT] installed\r\n",
				arch::irqNumber_t::PIT);
		arch::vmemWrite(text);

	}


}	// namespace arch

