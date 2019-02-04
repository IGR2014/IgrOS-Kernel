////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//
//


#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/longMode.hpp>
#include <include/vgaConsole.hpp>


extern "C" void kernelFunc() {

	// Init VGA console
	arch::vgaConsoleInit();
	arch::vgaConsoleWriteLine("IgrOS kernel");

}

