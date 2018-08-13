////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.cpp
//	Date:	13 Aug. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/port.hpp>
#include <include/interrupts.hpp>
#include <include/vgaConsole.hpp>
#include <include/pit.hpp>


// Arch-dependent code zone
namespace arch {


        // Setup PIT frequency
	void pitSetupFrequency(word_t frequency) {

		// Save current frequency value
		PIT_FREQUENCY	= frequency;

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		word_t divisor	= PIT_MAIN_FREQUENCY / frequency;

		// Tell pit we want to change divisor for channel 0
		outPort8(PIT_CONTROL,	0x36);

		// Set divisor (LOW first, then HIGH)
		outPort8(PIT_CHANNEL_0,	divisor & 0xFF);
		outPort8(PIT_CHANNEL_0,	divisor >> 8);

        }


	// Get expired ticks
	dword_t pitGetTicks() {

		// Send latch command for channel 0;
		outPort8(PIT_CONTROL, 0x00);

		// Get number of elapsed ticks since last IRQ
		byte_t	lowByte		= inPort8(PIT_CHANNEL_0);
		byte_t	highByte	= inPort8(PIT_CHANNEL_0);

		// Total elapsed value
		dword_t elapsedSinceIRQ = lowByte | (highByte << 8);

		// Return full expired ticks count
		return PIT_TICKS;// * PIT_FREQUENCY + elapsedSinceIRQ;

	}


	// PIT interrupt (#0) handler
	void pitInterruptHandler(const taskRegs_t*) {

		if ((++PIT_TICKS % PIT_FREQUENCY) == 0) {

			dword_t elapsed = pitGetTicks();
			dword_t seconds	= elapsed / PIT_MAIN_FREQUENCY;
			dword_t minutes	= 0;//seconds / 60;
			dword_t hours	= 0;//minutes / 60;

			vgaConsoleWriteLine("IRQ\t\t-> PIT");
			vgaConsoleWriteLine("\t100 TICKS (~1 SECOND) EXPIRED");
			vgaConsoleWrite("\t");
			vgaConsoleWriteDec(hours % 24);
			vgaConsoleWrite(":");
			vgaConsoleWriteDec(minutes % 60);
			vgaConsoleWrite(":");
			vgaConsoleWriteDec(seconds % 60);
			vgaConsoleWrite(".");
			vgaConsoleWriteDec(elapsed);
			vgaConsoleWriteLine("");
			vgaConsoleWriteLine("");

		}

	}


}	// namespace arch

