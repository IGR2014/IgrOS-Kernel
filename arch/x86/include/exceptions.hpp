////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.hpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_EXCEPTIONS_HPP
#define IGROS_ARCH_EXCEPTIONS_HPP


#include <include/types.hpp>
#include <include/taskRegs.hpp>


// Arch-dependent code zone
namespace arch {


	extern "C" {

		// Exception 0 handler
		extern void exHandler00();
		// Exception 1 handler
		extern void exHandler01();
		// Exception 2 handler
		extern void exHandler02();
		// Exception 3 handler
		extern void exHandler03();
		// Exception 4 handler
		extern void exHandler04();
		// Exception 5 handler
		extern void exHandler05();
		// Exception 6 handler
		extern void exHandler06();
		// Exception 7 handler
		extern void exHandler07();
		// Exception 8 handler
		extern void exHandler08();
		// Exception 9 handler
		extern void exHandler09();
		// Exception 10 handler
		extern void exHandler0A();
		// Exception 11 handler
		extern void exHandler0B();
		// Exception 12 handler
		extern void exHandler0C();
		// Exception 13 handler
		extern void exHandler0D();
		// Exception 14 handler
		extern void exHandler0E();
		// Exception 15 handler
		extern void exHandler0F();
		// Exception 16 handler
		extern void exHandler10();
		// Exception 17 handler
		extern void exHandler11();
		// Exception 18 handler
		extern void exHandler12();
		// Exception 19 handler
		extern void exHandler13();
		// Exception 20 handler
		extern void exHandler14();
		// Exception 21 handler
		extern void exHandler15();
		// Exception 22 handler
		extern void exHandler16();
		// Exception 23 handler
		extern void exHandler17();
		// Exception 24 handler
		extern void exHandler18();
		// Exception 25 handler
		extern void exHandler19();
		// Exception 26 handler
		extern void exHandler1A();
		// Exception 27 handler
		extern void exHandler1B();
		// Exception 28 handler
		extern void exHandler1C();
		// Exception 29 handler
		extern void exHandler1D();
		// Exception 30 handler
		extern void exHandler1E();
		// Exception 31 handler
		extern void exHandler1F();

		// Exception handler function
		void exHandler(const taskRegs*);

	}	// extern "C"


}	// namespace arch


#endif	// IGROS_ARCH_EXCEPTIONS_HPP

