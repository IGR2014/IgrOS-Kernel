////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.hpp
//	Date:	19 Dec 2023
//
//	Copyright (c) 2017 - 2023, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <array>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch x86
#include <arch/x86/isr.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Exception handlers addresses, indexed by exception number (exceptions.s)
	extern const std::add_pointer_t<void () noexcept>	exHandlerTable[32];


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86 namespace
namespace igros::x86 {


	// Exceptions structure
	class except final {

		// Copy c-tor
		except(const except &other) = delete;
		// Copy assignment
		auto	operator=(const except &other) -> except& = delete;

		// Move c-tor
		except(except &&other) = delete;
		// Move assignment
		auto	operator=(except &&other) -> except& = delete;


	public:

		// Exceptions names
		constexpr static std::array<const char* const, 32_usize> NAME {
			"DIVIDE BY ZERO",			// 0
			"DEBUG",				// 1
			"NON-MASKABLE INTERRUPT",		// 2
			"BREAKPOINT",				// 3
			"INTO DETECTED OVERFLOW",		// 4
			"BOUND RANGE EXCEEDED",			// 5
			"INVALID OPCODE",			// 6
			"NO COPROCESSOR",			// 7
			"DOUBLE FAULT",				// 8
			"COPROCESSOR SEGMENT OVERRUN",		// 9
			"INVALID TSS",				// 10
			"SEGMENT NOT PPRESENT",			// 11
			"STACK FAULT",				// 12
			"GENERAL PROTECTION FAULT",		// 13
			"PAGE FAULT",				// 14
			"UNKNOWN INTERRUPT",			// 15
			"COPROCESSOR FAULT",			// 16
			"ALIGNMENT CHECK",			// 17
			"MACHINE CHECK",			// 18
			"RESERVED",				// 19
			"RESERVED",				// 20
			"RESERVED",				// 21
			"RESERVED",				// 22
			"RESERVED",				// 23
			"RESERVED",				// 24
			"RESERVED",				// 25
			"RESERVED",				// 26
			"RESERVED",				// 27
			"RESERVED",				// 28
			"RESERVED",				// 29
			"RESERVED",				// 30
			"RESERVED"				// 31
		};

		// Exceptions number enumeration
		enum class NUMBER : igros_dword_t {
			DIVIDE_BY_ZERO			= 0_u32,
			DEBUG				= 1_u32,
			NON_MASKABLE_IRQ		= 2_u32,
			BREAKPOINT			= 3_u32,
			INTO_DETECTED_OVERFLOW		= 4_u32,
			BOUND_RANGE_EXCEEDED		= 5_u32,
			INVALID_OPCODE			= 6_u32,
			NO_COPROCESSOR			= 7_u32,
			DOUBLE_FAULT			= 8_u32,
			COPROCESSOR_SEGMENT_OVERRUN	= 9_u32,
			INVALID_TSS			= 10_u32,
			SEGMENT_NOT_PRESENT		= 11_u32,
			STACK_FAULT			= 12_u32,
			GENERAL_PROTECTION_FAULT	= 13_u32,
			PAGE_FAULT			= 14_u32,
			UNKNOWN_IRQ			= 15_u32,
			COPROCESSOR_FAULT		= 16_u32,
			ALIGNMENT_CHECK			= 17_u32,
			MACHINE_CHECK			= 18_u32
		};


		// Default c-tor
		except() noexcept = default;

		// Init exceptions
		static void	init() noexcept;

		// Install exceptions handler
		template<NUMBER N, isr_t HANDLE>
		constexpr static void	install() noexcept;
		// Uninstall exceptions handler
		template<NUMBER N>
		constexpr static void	uninstall() noexcept;

		// Default exception handler
		[[noreturn]]
		static void	defaultHandler(const register_t* regs) noexcept;


	};


	// Install handler
	template<except::NUMBER N, isr_t HANDLE>
	constexpr void except::install() noexcept {
		// Install ISR
		isrHandlerInstall(static_cast<igros_dword_t>(N), HANDLE);
	}

	// Uninstall handler
	template<except::NUMBER N>
	constexpr void except::uninstall() noexcept {
		// Uninstall ISR
		isrHandlerUninstall(static_cast<igros_dword_t>(N));
	}


}	// namespace igros::x86

