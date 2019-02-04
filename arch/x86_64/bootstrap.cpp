////////////////////////////////////////////////////////////////
//
//	Bootstrap from i386 to x86_64 code
//
//	File:	bootstrap.cpp
//	Date:	28 Jan. 2019
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//
//


#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/longMode.hpp>


extern "C" void kernelBootstrap() {

	// check if multiboot is ok
	/*
	if (arch::checkMultiboot() == 0x00) {

		return;
	
	}
	*/

	// Check if CPUID is available
	if (arch::checkCPUID() == 0x00) {

		return;

	}

	//Check if long mode is available
	if (arch::checkLongMode() == 0x00) {

		return;
	
	}

	// Perform jump to long mode
	arch::jumpToLongMode();

}

