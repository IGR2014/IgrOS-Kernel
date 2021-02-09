////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/i386/types.hpp>


// i386 architecture namespace
namespace igros::i386 {


	// Define port data type which is 16bit word
	using porti386_t = word_t;


}	// namespace igros::i386


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Read byte from port
	[[nodiscard]]
	inline igros::byte_t volatile	outPort8(const igros::i386::porti386_t addr) noexcept;
	// Read word from port
	[[nodiscard]]
	inline igros::word_t volatile	outPort16(const igros::i386::porti386_t addr) noexcept;
	// Read long from port
	[[nodiscard]]
	inline igros::dword_t volatile	outPort32(const igros::i386::porti386_t addr) noexcept;

	// Write byte to port
	inline void volatile inPort8(const igros::i386::porti386_t addr, const igros::byte_t value) noexcept;
	// Write word to port
	inline void volatile inPort16(const igros::i386::porti386_t addr, const igros::word_t value) noexcept;
	// Write long to port
	inline void volatile inPort32(const igros::i386::porti386_t addr, const igros::dword_t value) noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 architecture namespace
namespace igros::i386 {


	// I/O structure
	class ioi386 final {

		// Copy c-tor
		ioi386(const ioi386 &other) = delete;
		// Copy assignment
		ioi386& operator=(const ioi386 &other) = delete;

		// Move c-tor
		ioi386(ioi386 &&other) = delete;
		// Move assignment
		ioi386& operator=(ioi386 &&other) = delete;


	public:

		// Default c-tor
		ioi386() noexcept = default;

		// Read byte from port
		[[nodiscard]]
		static byte_t volatile	readPort8(const porti386_t addr) noexcept;
		// Read word from port
		[[nodiscard]]
		static word_t volatile	readPort16(const porti386_t addr) noexcept;
		// Read long from port
		[[nodiscard]]
		static dword_t volatile	readPort32(const porti386_t addr) noexcept;

		// Write byte to port
		static void volatile	writePort8(const porti386_t addr, const byte_t value) noexcept;
		// Write word to port
		static void volatile	writePort16(const porti386_t addr, const word_t value) noexcept;
		// Write long to port
		static void volatile	writePort32(const porti386_t addr, const dword_t value) noexcept;

		// Read byte from memory
		[[nodiscard]]
		static byte_t volatile	readMemory8(const byte_t* const addr) noexcept;
		// Read word from memory
		[[nodiscard]]
		static word_t volatile	readMemory16(const word_t* const addr) noexcept;
		// Read long from memory
		[[nodiscard]]
		static dword_t volatile	readMemory32(const dword_t* const addr) noexcept;

		// Write byte to memory
		static void volatile	writeMemory8(byte_t* const addr, const byte_t value) noexcept;
		// Write word to memory
		static void volatile	writeMemory16(word_t* const addr, const word_t value) noexcept;
		// Write long to memory
		static void volatile	writeMemory32(dword_t* const addr, const dword_t value) noexcept;


	};


	// Read byte from port
	[[nodiscard]]
	inline byte_t volatile ioi386::readPort8(const porti386_t addr) noexcept {
		return outPort8(addr);
	}

	// Read word from port
	[[nodiscard]]
	inline word_t volatile ioi386::readPort16(const porti386_t addr) noexcept {
		return outPort16(addr);
	}

	// Read long from port
	[[nodiscard]]
	inline dword_t volatile ioi386::readPort32(const porti386_t addr) noexcept {
		return outPort32(addr);
	}


	// Write byte to port
	inline void volatile ioi386::writePort8(const porti386_t addr, const byte_t value) noexcept {
		inPort8(addr, value);
	}

	// Write word to port
	inline void volatile ioi386::writePort16(const porti386_t addr, const word_t value) noexcept {
		inPort16(addr, value);
	}

	// Write long to port
	inline void volatile ioi386::writePort32(const porti386_t addr, const dword_t value) noexcept {
		inPort32(addr, value);
	}


	// Read byte from memory
	[[nodiscard]]
	inline byte_t volatile ioi386::readMemory8(const byte_t* const addr) noexcept {
		return *addr;
	}

	// Read word from memory
	[[nodiscard]]
	inline word_t volatile ioi386::readMemory16(const word_t* const addr) noexcept {
		return *addr;
	}

	// Read long from memory
	[[nodiscard]]
	inline dword_t volatile ioi386::readMemory32(const dword_t* const addr) noexcept {
		return *addr;
	}


	// Write byte to memory
	inline void volatile ioi386::writeMemory8(byte_t* const addr, const byte_t value) noexcept {
		*addr = value;
	}

	// Write word to memory
	inline void volatile ioi386::writeMemory16(word_t* const addr, const word_t value) noexcept {
		*addr = value;
	}

	// Write long to memory
	inline void volatile ioi386::writeMemory32(dword_t* const addr, const dword_t value) noexcept {
		*addr = value;
	}


}	// namespace igros::i386

