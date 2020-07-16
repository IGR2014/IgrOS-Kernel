////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.hpp
//	Date:	16 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/x86_64/types.hpp>
#include <arch/x86_64/isr.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Exception 0 handler
		constexpr void	exHandler00();
		// Exception 1 handler
		constexpr void	exHandler01();
		// Exception 2 handler
		constexpr void	exHandler02();
		// Exception 3 handler
		constexpr void	exHandler03();
		// Exception 4 handler
		constexpr void	exHandler04();
		// Exception 5 handler
		constexpr void	exHandler05();
		// Exception 6 handler
		constexpr void	exHandler06();
		// Exception 7 handler
		constexpr void	exHandler07();
		// Exception 8 handler
		constexpr void	exHandler08();
		// Exception 9 handler
		constexpr void	exHandler09();
		// Exception 10 handler
		constexpr void	exHandler0A();
		// Exception 11 handler
		constexpr void	exHandler0B();
		// Exception 12 handler
		constexpr void	exHandler0C();
		// Exception 13 handler
		constexpr void	exHandler0D();
		// Exception 14 handler
		constexpr void	exHandler0E();
		// Exception 15 handler
		constexpr void	exHandler0F();
		// Exception 16 handler
		constexpr void	exHandler10();
		// Exception 17 handler
		constexpr void	exHandler11();
		// Exception 18 handler
		constexpr void	exHandler12();
		// Exception 19 handler
		constexpr void	exHandler13();
		// Exception 20 handler
		constexpr void	exHandler14();
		// Exception 21 handler
		constexpr void	exHandler15();
		// Exception 22 handler
		constexpr void	exHandler16();
		// Exception 23 handler
		constexpr void	exHandler17();
		// Exception 24 handler
		constexpr void	exHandler18();
		// Exception 25 handler
		constexpr void	exHandler19();
		// Exception 26 handler
		constexpr void	exHandler1A();
		// Exception 27 handler
		constexpr void	exHandler1B();
		// Exception 28 handler
		constexpr void	exHandler1C();
		// Exception 29 handler
		constexpr void	exHandler1D();
		// Exception 30 handler
		constexpr void	exHandler1E();
		// Exception 31 handler
		constexpr void	exHandler1F();


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Task registers structure forward declaration
	struct register_t;


	// Exceptions structure
	class except final {

		// Copy c-tor
		except(const except &other) = delete;
		// Copy assignment
		except& operator=(const except &other) = delete;

		// Move c-tor
		except(except &&other) = delete;
		// Move assignment
		except& operator=(except &&other) = delete;


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

		// Init exceptions
		static void init() noexcept;

		// Install exceptions handler
		inline static void install(const NUMBER irqNumber, const x86_64::isr_t irqHandler) noexcept;
		// Uninstall exceptions handler
		inline static void uninstall(const NUMBER irqNumber) noexcept;

		// Default exception handler
		static void exDefaultHandler(const register_t* regs) noexcept;


	};


	// Install handler
	inline void except::install(const NUMBER exNumber, const x86_64::isr_t handler) noexcept {
		// Install ISR
		x86_64::isrHandlerInstall(static_cast<dword_t>(exNumber), handler);
	}

	// Uninstall handler
	inline void except::uninstall(const NUMBER exNumber) noexcept {
		// Uninstall ISR
		x86_64::isrHandlerUninstall(static_cast<dword_t>(exNumber));
	}


}	// namespace igros::x86_64

