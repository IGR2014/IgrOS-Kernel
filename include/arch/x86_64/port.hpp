////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma	once


#include <arch/x86_64/types.hpp>


// OS namespace
namespace igros {


	// Define port data type which is 16bit word
	using port_t = word_t;


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read byte from port
		[[nodiscard]] inline byte_t volatile outPort8(const port_t addr) noexcept;
		// Read word from port
		[[nodiscard]] inline word_t volatile outPort16(const port_t addr) noexcept;
		// Read long from port
		[[nodiscard]] inline dword_t volatile outPort32(const port_t addr) noexcept;

		// Write byte to port
		inline void volatile inPort8(const port_t addr, const byte_t value) noexcept;
		// Write word to port
		inline void volatile inPort16(const port_t addr, const word_t value) noexcept;
		// Write long to port
		inline void volatile inPort32(const port_t addr, const dword_t value) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace igros

