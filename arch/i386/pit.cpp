////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.cpp
//	Date:	01 Feb. 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <include/port.hpp>
#include <include/interrupts.hpp>
#include <include/vgaConsole.hpp>
#include <include/pit.hpp>


// Arch-dependent code zone
namespace arch {


	// Ticks count
	static dword_t	PIT_TICKS	= 0;
	// Current frequency
	static word_t	PIT_FREQUENCY	= 0;
	// Current divisor
	static word_t	PIT_DIVISOR	= 1;


        // Setup PIT frequency
	void pitSetupFrequency(word_t frequency) {

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		word_t divisor	= PIT_MAIN_FREQUENCY / frequency;

		// Save divisor value
		PIT_DIVISOR	= divisor;
		// Save current real frequency value
		PIT_FREQUENCY	= PIT_MAIN_FREQUENCY / divisor;

		vgaConsoleWrite("REAL frequency set to: ");
		vgaConsoleWriteDec(PIT_FREQUENCY);
		vgaConsoleWriteLine(" Hz");

		// Tell pit we want to change divisor for channel 0
		outPort8(PIT_CONTROL,	0x36);

		// Set divisor (LOW first, then HIGH)
		outPort8(PIT_CHANNEL_0,	divisor & 0xFF);
		outPort8(PIT_CHANNEL_0,	(divisor >> 8) & 0xFF);

        }


	// Get expired ticks
	quad_t pitGetTicks() {

		// Send latch command for channel 0;
		outPort8(PIT_CONTROL, 0x00);

		// Get number of elapsed ticks since last IRQ
		byte_t	lowByte		= inPort8(PIT_CHANNEL_0);
		byte_t	highByte	= inPort8(PIT_CHANNEL_0);

		// Total elapsed ticks value
		word_t elapsedSinceIRQ	= (highByte << 8) | lowByte;

		// Return full expired ticks count
		return PIT_TICKS * PIT_DIVISOR + elapsedSinceIRQ;

	}


	// PIT interrupt (#0) handler
	void pitInterruptHandler(const taskRegs_t*) {

		// Output every N-th tick were N = frequency
		if ((++PIT_TICKS % PIT_FREQUENCY) == 0) {

			dword_t elapsed		= pitGetTicks();
			dword_t nanoseconds	= elapsed % PIT_MAIN_FREQUENCY;
			dword_t seconds		= elapsed / PIT_MAIN_FREQUENCY;
			dword_t minutes		= seconds / 60;
			dword_t hours		= minutes / 60;

			vgaConsoleWriteLine("IRQ\t\t-> PIT");
			vgaConsoleWriteLine("\t100 TICKS (~1 SECOND) EXPIRED");
			vgaConsoleWrite("\t");
			vgaConsoleWriteDec(hours % 24);
			vgaConsoleWrite(":");
			vgaConsoleWriteDec(minutes % 60);
			vgaConsoleWrite(":");
			vgaConsoleWriteDec(seconds % 60);
			vgaConsoleWrite(".");
			vgaConsoleWriteDec(nanoseconds);
			vgaConsoleWriteLine("");
			vgaConsoleWriteLine("");

		}

	}


}	// namespace arch

