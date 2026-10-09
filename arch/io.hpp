////////////////////////////////////////////////////////////////
//
//	I/O operations
//
//	File:	io.hpp
//	Date:	21 Mar 2023
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <concepts>
// IgrOS-Kernel arch
#include <arch/types.hpp>

#if	defined (IGROS_ARCH_i386) || defined (IGROS_ARCH_x86_64)

// IgrOS-Kernel arch x86
#include <arch/x86/io.hpp>

#else

#error "Unknown architecture!"

#endif


// Arch namespace
namespace igros::arch {


	// Port I/O every architecture must provide
	template<class T, class P>
	concept PortIO = requires(const P port, const igros_byte_t byte, const igros_word_t word, const igros_dword_t dword) {
		{ T::readPort8(port) }		-> std::same_as<igros_byte_t>;
		{ T::readPort16(port) }		-> std::same_as<igros_word_t>;
		{ T::readPort32(port) }		-> std::same_as<igros_dword_t>;
		{ T::writePort8(port, byte) }	-> std::same_as<void>;
		{ T::writePort16(port, word) }	-> std::same_as<void>;
		{ T::writePort32(port, dword) }	-> std::same_as<void>;
	};


#if	defined (IGROS_ARCH_i386) || defined (IGROS_ARCH_x86_64)

	// I/O port address type
	using port_t	= x86::port_t;
	// I/O type
	using io	= x86::io;

#endif


	static_assert(PortIO<io, port_t>, "Architecture I/O doesn't provide port I/O interface!");


}	// namespace igros::arch

