////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.hpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <array>

#include <arch/i386/types.hpp>
#include <arch/i386/isr.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Exception 0 handler
	inline void	exHandler00() noexcept;
	// Exception 1 handler
	inline void	exHandler01() noexcept;
	// Exception 2 handler
	inline void	exHandler02() noexcept;
	// Exception 3 handler
	inline void	exHandler03() noexcept;
	// Exception 4 handler
	inline void	exHandler04() noexcept;
	// Exception 5 handler
	inline void	exHandler05() noexcept;
	// Exception 6 handler
	inline void	exHandler06() noexcept;
	// Exception 7 handler
	inline void	exHandler07() noexcept;
	// Exception 8 handler
	inline void	exHandler08() noexcept;
	// Exception 9 handler
	inline void	exHandler09() noexcept;
	// Exception 10 handler
	inline void	exHandler0A() noexcept;
	// Exception 11 handler
	inline void	exHandler0B() noexcept;
	// Exception 12 handler
	inline void	exHandler0C() noexcept;
	// Exception 13 handler
	inline void	exHandler0D() noexcept;
	// Exception 14 handler
	inline void	exHandler0E() noexcept;
	// Exception 15 handler
	inline void	exHandler0F() noexcept;
	// Exception 16 handler
	inline void	exHandler10() noexcept;
	// Exception 17 handler
	inline void	exHandler11() noexcept;
	// Exception 18 handler
	inline void	exHandler12() noexcept;
	// Exception 19 handler
	inline void	exHandler13() noexcept;
	// Exception 20 handler
	inline void	exHandler14() noexcept;
	// Exception 21 handler
	inline void	exHandler15() noexcept;
	// Exception 22 handler
	inline void	exHandler16() noexcept;
	// Exception 23 handler
	inline void	exHandler17() noexcept;
	// Exception 24 handler
	inline void	exHandler18() noexcept;
	// Exception 25 handler
	inline void	exHandler19() noexcept;
	// Exception 26 handler
	inline void	exHandler1A() noexcept;
	// Exception 27 handler
	inline void	exHandler1B() noexcept;
	// Exception 28 handler
	inline void	exHandler1C() noexcept;
	// Exception 29 handler
	inline void	exHandler1D() noexcept;
	// Exception 30 handler
	inline void	exHandler1E() noexcept;
	// Exception 31 handler
	inline void	exHandler1F() noexcept;

#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 namespace
namespace igros::i386 {


	// Task registers structure forward declaration
	struct registeri386_t;


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
		constexpr static std::array<const sbyte_t* const, 32> NAME {
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
			DIVIDE_BY_ZERO			= 0U,
			DEBUG				= 1U,
			NON_MASKABLE_IRQ		= 2U,
			BREAKPOINT			= 3U,
			INTO_DETECTED_OVERFLOW		= 4U,
			BOUND_RANGE_EXCEEDED		= 5U,
			INVALID_OPCODE			= 6U,
			NO_COPROCESSOR			= 7U,
			DOUBLE_FAULT			= 8U,
			COPROCESSOR_SEGMENT_OVERRUN	= 9U,
			INVALID_TSS			= 10U,
			SEGMENT_NOT_PRESENT		= 11U,
			STACK_FAULT			= 12U,
			GENERAL_PROTECTION_FAULT	= 13U,
			PAGE_FAULT			= 14U,
			UNKNOWN_IRQ			= 15U,
			COPROCESSOR_FAULT		= 16U,
			ALIGNMENT_CHECK			= 17U,
			MACHINE_CHECK			= 18U
		};


		// Default c-tor
		except() noexcept = default;

		// Init exceptions
		static void init() noexcept;

		// Install exceptions handler
		static void install(const NUMBER irqNumber, const isri386_t irqHandler) noexcept;
		// Uninstall exceptions handler
		static void uninstall(const NUMBER irqNumber) noexcept;

		// Default exception handler
		static void exDefaultHandler(const registeri386_t* regs) noexcept;


	};


	// Install handler
	inline void except::install(const NUMBER exNumber, const isri386_t handler) noexcept {
		// Install ISR
		isrHandlerInstall(static_cast<dword_t>(exNumber), handler);
	}

	// Uninstall handler
	inline void except::uninstall(const NUMBER exNumber) noexcept {
		// Uninstall ISR
		isrHandlerUninstall(static_cast<dword_t>(exNumber));
	}


}	// namespace igros::i386

