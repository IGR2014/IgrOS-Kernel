////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.cpp
//	Date:	03 Aug. 2018
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
	void pitSetupFrequency(word_t freqency) {

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		word_t divisor = PIT_FREQUENCY / freqency;

		// Tell pit we want to change divisor for channel 0
		outPort8(PIT_CONTROL,	0x36);
		// Set divisor (LOW first, then HIGH)
		outPort8(PIT_CHANNEL_0,	divisor & 0xFF);
		outPort8(PIT_CHANNEL_0,	divisor >> 8);

        }


	// Convert expired ticks count to seconds
	dword_t pitToSeconds() {

		return PIT_TICKS / 100;

	}


	// PIT interrupt (#0) handler
	void pitInterruptHandler(const taskRegs_t*) {

		++PIT_TICKS;

		if ((PIT_TICKS % 100) == 0) {

			dword_t elapsed = pitToSeconds();
			dword_t seconds	= elapsed % 60;
			dword_t minutes	= (seconds / 60) % 60;
			dword_t hours	= (seconds / 3600) % 24;

			vgaConsoleWriteLine("IRQ\t\t-> PIT");
			vgaConsoleWriteLine("\t100 TICKS (~1 SECOND) EXPIRED");
			vgaConsoleWriteDec(hours);
			vgaConsoleWrite(":");
			vgaConsoleWriteDec(minutes);
			vgaConsoleWrite(":");
			vgaConsoleWriteDec(seconds);
			vgaConsoleWrite(".");
			vgaConsoleWriteDec(0);
			vgaConsoleWriteLine("");
			vgaConsoleWriteLine("");

		}

	}


}	// namespace arch

