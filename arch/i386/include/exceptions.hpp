////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.hpp
//	Date:	20 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_EXCEPTIONS_HPP
#define IGROS_ARCH_EXCEPTIONS_HPP


#include <type_traits>

#include <include/types.hpp>
#include <include/taskRegs.hpp>


// Arch-dependent code zone
namespace arch {


	// Interrupt handler type
	using exHandler_t = std::add_pointer<void(const taskRegs* regs)>::type;


	// Interrupts number enumeration
	enum EX_NUMBER : t_u8 {

		EX_DIVIDE_BY_ZERO		= 0,
		EX_DEBUG			= 1,
		EX_NON_MASKABLE_IRQ		= 2,
		EX_BREAKPOINT			= 3,
		EX_INTO_DETECTED_OVERFLOW	= 4,
		EX_BOUND_RANGE_EXCEEDED		= 5,
		EX_INVALID_OPCODE		= 6,
		EX_NO_COPROCESSOR		= 7,
		EX_DOUBLE_FAULT			= 8,
		EX_COPROCESSOR_SEGMENT_OVERRUN	= 9,
		EX_INVALID_TSS			= 10,
		EX_SEGMENT_NOT_PRESENT		= 11,
		EX_STACK_FAULT			= 12,
		EX_GENERAL_PROTECTION_FAULT	= 13,
		EX_PAGE_FAULT			= 14,
		EX_UNKNOWN_IRQ			= 15,
		EX_COPROCESSOR_FAULT		= 16,
		EX_ALIGNMENT_CHECK		= 17,
		EX_MACHINE_CHECK		= 18

	};


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


	// Install handler
	void	exHandlerInstall(EX_NUMBER, exHandler_t);
	// Uninstall handler
	void	exHandlerUninstall(EX_NUMBER);


}	// namespace arch


#endif	// IGROS_ARCH_EXCEPTIONS_HPP

