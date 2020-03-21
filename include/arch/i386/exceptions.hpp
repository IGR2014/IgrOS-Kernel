////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_EXCEPTIONS_HPP
#define IGROS_ARCH_EXCEPTIONS_HPP


#include <types.hpp>
#include <isr.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


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


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Task registers structure forward declaration
	struct taskRegs_t;


	// Exceptions structure
	class except final {

	public:

		// Exceptions names
		constexpr static const sbyte_t* const NAME[32] {
			u8"DIVIDE BY ZERO",			// 0
			u8"DEBUG",				// 1
			u8"NON-MASKABLE INTERRUPT",		// 2
			u8"BREAKPOINT",				// 3
			u8"INTO DETECTED OVERFLOW",		// 4
			u8"BOUND RANGE EXCEEDED",		// 5
			u8"INVALID OPCODE",			// 6
			u8"NO COPROCESSOR",			// 7
			u8"DOUBLE FAULT",			// 8
			u8"COPROCESSOR SEGMENT OVERRUN",	// 9
			u8"INVALID TSS",			// 10
			u8"SEGMENT NOT PPRESENT",		// 11
			u8"STACK FAULT",			// 12
			u8"GENERAL PROTECTION FAULT",		// 13
			u8"PAGE FAULT",				// 14
			u8"UNKNOWN INTERRUPT",			// 15
			u8"COPROCESSOR FAULT",			// 16
			u8"ALIGNMENT CHECK",			// 17
			u8"MACHINE CHECK",			// 18
			u8"RESERVED",				// 19
			u8"RESERVED",				// 20
			u8"RESERVED",				// 21
			u8"RESERVED",				// 22
			u8"RESERVED",				// 23
			u8"RESERVED",				// 24
			u8"RESERVED",				// 25
			u8"RESERVED",				// 26
			u8"RESERVED",				// 27
			u8"RESERVED",				// 28
			u8"RESERVED",				// 29
			u8"RESERVED",				// 30
			u8"RESERVED"				// 31
		};

		// Exceptions number enumeration
		enum class NUMBER : dword_t {
			DIVIDE_BY_ZERO			= 0,
			DEBUG				= 1,
			NON_MASKABLE_IRQ		= 2,
			BREAKPOINT			= 3,
			INTO_DETECTED_OVERFLOW		= 4,
			BOUND_RANGE_EXCEEDED		= 5,
			INVALID_OPCODE			= 6,
			NO_COPROCESSOR			= 7,
			DOUBLE_FAULT			= 8,
			COPROCESSOR_SEGMENT_OVERRUN	= 9,
			INVALID_TSS			= 10,
			SEGMENT_NOT_PRESENT		= 11,
			STACK_FAULT			= 12,
			GENERAL_PROTECTION_FAULT	= 13,
			PAGE_FAULT			= 14,
			UNKNOWN_IRQ			= 15,
			COPROCESSOR_FAULT		= 16,
			ALIGNMENT_CHECK			= 17,
			MACHINE_CHECK			= 18
		};


		// Default c-tor
		except() noexcept = default;

		// Copy c-tor
		except(const except &other) = delete;
		// Copy assignment
		except& operator=(const except &other) = delete;

		// Move c-tor
		except(except &&other) = delete;
		// Move assignment
		except& operator=(except &&other) = delete;

		// Init exceptions
		static void init() noexcept;

		// Install exceptions handler
		constexpr static void install(const NUMBER irqNumber, const isrHandler_t irqHandler) noexcept;
		// Uninstall exceptions handler
		constexpr static void uninstall(const NUMBER irqNumber) noexcept;

		// Default exception handler
		static void exDefaultHandler(const taskRegs_t* regs) noexcept;


	};


	// Install handler
	constexpr void except::install(const NUMBER exNumber, const isrHandler_t handler) noexcept {
		// Install ISR
		isrHandlerInstall(static_cast<dword_t>(exNumber), handler);
	}

	// Uninstall handler
	constexpr void except::uninstall(const NUMBER exNumber) noexcept {
		// Uninstall ISR
		isrHandlerUninstall(static_cast<dword_t>(exNumber));
	}


}	// namespace arch


#endif	// IGROS_ARCH_EXCEPTIONS_HPP

