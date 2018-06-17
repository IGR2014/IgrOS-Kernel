////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.hpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
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
		void	exHandler00();
		// Exception 1 handler
		void	exHandler01();
		// Exception 2 handler
		void	exHandler02();
		// Exception 3 handler
		void	exHandler03();
		// Exception 4 handler
		void	exHandler04();
		// Exception 5 handler
		void	exHandler05();
		// Exception 6 handler
		void	exHandler06();
		// Exception 7 handler
		void	exHandler07();
		// Exception 8 handler
		void	exHandler08();
		// Exception 9 handler
		void	exHandler09();
		// Exception 10 handler
		void	exHandler0A();
		// Exception 11 handler
		void	exHandler0B();
		// Exception 12 handler
		void	exHandler0C();
		// Exception 13 handler
		void	exHandler0D();
		// Exception 14 handler
		void	exHandler0E();
		// Exception 15 handler
		void	exHandler0F();
		// Exception 16 handler
		void	exHandler10();
		// Exception 17 handler
		void	exHandler11();
		// Exception 18 handler
		void	exHandler12();
		// Exception 19 handler
		void	exHandler13();
		// Exception 20 handler
		void	exHandler14();
		// Exception 21 handler
		void	exHandler15();
		// Exception 22 handler
		void	exHandler16();
		// Exception 23 handler
		void	exHandler17();
		// Exception 24 handler
		void	exHandler18();
		// Exception 25 handler
		void	exHandler19();
		// Exception 26 handler
		void	exHandler1A();
		// Exception 27 handler
		void	exHandler1B();
		// Exception 28 handler
		void	exHandler1C();
		// Exception 29 handler
		void	exHandler1D();
		// Exception 30 handler
		void	exHandler1E();
		// Exception 31 handler
		void	exHandler1F();

		// Exception handler function
		void	exHandler(const taskRegs*);


	}	// extern "C"


}	// namespace arch


#endif	// IGROS_ARCH_EXCEPTIONS_HPP

