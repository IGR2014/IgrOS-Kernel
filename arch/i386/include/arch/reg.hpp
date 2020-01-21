////////////////////////////////////////////////////////////////////////
//
//	General purpose, segment, stack and flags registers access
//
//	File:	reg.hpp
//	Date:	21 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_GENERAL_REGISTERS_HPP
#define IGROS_ARCH_GENERAL_REGISTERS_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read CS register
		[[nodiscard]] word_t volatile outCS() noexcept;
		// Read DS register
		[[nodiscard]] word_t volatile outDS() noexcept;
		// Read ES register
		[[nodiscard]] word_t volatile outES() noexcept;
		// Read FS register
		[[nodiscard]] word_t volatile outFS() noexcept;
		// Read GS register
		[[nodiscard]] word_t volatile outGS() noexcept;
		// Read SS register
		[[nodiscard]] word_t volatile outSS() noexcept;

		// Read EBP register
		[[nodiscard]] dword_t volatile outEBP() noexcept;
		// Read ESP register
		[[nodiscard]] dword_t volatile outESP() noexcept;

		// Read EFLAGS register
		[[nodiscard]] dword_t volatile outEFLAGS() noexcept;

		// Read EAX register
		[[nodiscard]] dword_t volatile outEAX() noexcept;
		// Read EBX register
		[[nodiscard]] dword_t volatile outEBX() noexcept;
		// Read ECX register
		[[nodiscard]] dword_t volatile outECX() noexcept;
		// Read EDX register
		[[nodiscard]] dword_t volatile outEDX() noexcept;
		// Read EDI register
		[[nodiscard]] dword_t volatile outEDI() noexcept;
		// Read ESI register
		[[nodiscard]] dword_t volatile outESI() noexcept;

		// Read EIP register
		[[nodiscard]] dword_t volatile outEAX() noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_GENERAL_REGISTERS_HPP

