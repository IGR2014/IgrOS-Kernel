////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.hpp
//	Date:	14 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <array>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch i386
#include <arch/i386/isr.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Exception 0 handler
	void	exHandler00() noexcept;
	// Exception 1 handler
	void	exHandler01() noexcept;
	// Exception 2 handler
	void	exHandler02() noexcept;
	// Exception 3 handler
	void	exHandler03() noexcept;
	// Exception 4 handler
	void	exHandler04() noexcept;
	// Exception 5 handler
	void	exHandler05() noexcept;
	// Exception 6 handler
	void	exHandler06() noexcept;
	// Exception 7 handler
	void	exHandler07() noexcept;
	// Exception 8 handler
	void	exHandler08() noexcept;
	// Exception 9 handler
	void	exHandler09() noexcept;
	// Exception 10 handler
	void	exHandler0A() noexcept;
	// Exception 11 handler
	void	exHandler0B() noexcept;
	// Exception 12 handler
	void	exHandler0C() noexcept;
	// Exception 13 handler
	void	exHandler0D() noexcept;
	// Exception 14 handler
	void	exHandler0E() noexcept;
	// Exception 15 handler
	void	exHandler0F() noexcept;
	// Exception 16 handler
	void	exHandler10() noexcept;
	// Exception 17 handler
	void	exHandler11() noexcept;
	// Exception 18 handler
	void	exHandler12() noexcept;
	// Exception 19 handler
	void	exHandler13() noexcept;
	// Exception 20 handler
	void	exHandler14() noexcept;
	// Exception 21 handler
	void	exHandler15() noexcept;
	// Exception 22 handler
	void	exHandler16() noexcept;
	// Exception 23 handler
	void	exHandler17() noexcept;
	// Exception 24 handler
	void	exHandler18() noexcept;
	// Exception 25 handler
	void	exHandler19() noexcept;
	// Exception 26 handler
	void	exHandler1A() noexcept;
	// Exception 27 handler
	void	exHandler1B() noexcept;
	// Exception 28 handler
	void	exHandler1C() noexcept;
	// Exception 29 handler
	void	exHandler1D() noexcept;
	// Exception 30 handler
	void	exHandler1E() noexcept;
	// Exception 31 handler
	void	exHandler1F() noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 namespace
namespace igros::i386 {


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
		static void	defaultHandler(const register_t* regs) noexcept;


	};


	// Install handler
	template<except::NUMBER N, isr_t HANDLE>
	constexpr void except::install() noexcept {
		// Install ISR
		isrHandlerInstall<static_cast<igros_dword_t>(N), HANDLE>();
	}

	// Uninstall handler
	template<except::NUMBER N>
	constexpr void except::uninstall() noexcept {
		// Uninstall ISR
		isrHandlerUninstall<static_cast<igros_dword_t>(N)>();
	}


}	// namespace igros::i386

