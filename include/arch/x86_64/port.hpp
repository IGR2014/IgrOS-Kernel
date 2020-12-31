////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	18 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma	once


#include <arch/x86_64/types.hpp>


// x86_64 architecture namespace
namespace igros::x86_64 {


	// Define port data type which is 16bit word
	using port_t = word_t;


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read byte from port
		[[nodiscard]]
		inline byte_t volatile	outPort8(const port_t addr) noexcept;
		// Read word from port
		[[nodiscard]]
		inline word_t volatile	outPort16(const port_t addr) noexcept;
		// Read long from port
		[[nodiscard]]
		inline dword_t volatile	outPort32(const port_t addr) noexcept;

		// Write byte to port
		inline void volatile inPort8(const port_t addr, const byte_t value) noexcept;
		// Write word to port
		inline void volatile inPort16(const port_t addr, const word_t value) noexcept;
		// Write long to port
		inline void volatile inPort32(const port_t addr, const dword_t value) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// I/O structure
	class io final {

		// Copy c-tor
		io(const io &other) = delete;
		// Copy assignment
		io& operator=(const io &other) = delete;

		// Move c-tor
		io(io &&other) = delete;
		// Move assignment
		io& operator=(io &&other) = delete;


	public:

		// Default c-tor
		io() noexcept = default;

		// Read byte from port
		[[nodiscard]]
	inline	static byte_t volatile	readPort8(const port_t addr) noexcept;
		// Read word from port
		[[nodiscard]]
	inline	static word_t volatile	readPort16(const port_t addr) noexcept;
		// Read long from port
		[[nodiscard]]
	inline	static dword_t volatile	readPort32(const port_t addr) noexcept;

		// Write byte to port
	inline	static void volatile	writePort8(const port_t addr, const byte_t value) noexcept;
		// Write word to port
	inline	static void volatile	writePort16(const port_t addr, const word_t value) noexcept;
		// Write long to port
	inline	static void volatile	writePort32(const port_t addr, const dword_t value) noexcept;

		// Read byte from memory
		[[nodiscard]]
	inline	static byte_t volatile	readMemory8(const byte_t* const addr) noexcept;
		// Read word from memory
		[[nodiscard]]
	inline	static word_t volatile	readMemory16(const word_t* const addr) noexcept;
		// Read long from memory
		[[nodiscard]]
	inline	static dword_t volatile	readMemory32(const dword_t* const addr) noexcept;

		// Write byte to memory
	inline	static void volatile	writeMemory8(byte_t* const addr, const byte_t value) noexcept;
		// Write word to memory
	inline	static void volatile	writeMemory16(word_t* const addr, const word_t value) noexcept;
		// Write long to memory
	inline	static void volatile	writeMemory32(dword_t* const addr, const dword_t value) noexcept;


	};


	// Read byte from port
	[[nodiscard]]
	inline byte_t volatile io::readPort8(const port_t addr) noexcept {
		return outPort8(addr);
	}

	// Read word from port
	[[nodiscard]]
	inline word_t volatile io::readPort16(const port_t addr) noexcept {
		return outPort16(addr);
	}

	// Read long from port
	[[nodiscard]]
	inline dword_t volatile io::readPort32(const port_t addr) noexcept {
		return outPort32(addr);
	}


	// Write byte to port
	inline void volatile io::writePort8(const port_t addr, const byte_t value) noexcept {
		inPort8(addr, value);
	}

	// Write word to port
	inline void volatile io::writePort16(const port_t addr, const word_t value) noexcept {
		inPort16(addr, value);
	}

	// Write long to port
	inline void volatile io::writePort32(const port_t addr, const dword_t value) noexcept {
		inPort32(addr, value);
	}


	// Read byte from memory
	[[nodiscard]]
	inline byte_t volatile io::readMemory8(const byte_t* const addr) noexcept {
		return *addr;
	}

	// Read word from memory
	[[nodiscard]]
	inline word_t volatile io::readMemory16(const word_t* const addr) noexcept {
		return *addr;
	}

	// Read long from memory
	[[nodiscard]]
	inline dword_t volatile io::readMemory32(const dword_t* const addr) noexcept {
		return *addr;
	}


	// Write byte to memory
	inline void volatile io::writeMemory8(byte_t* const addr, const byte_t value) noexcept {
		*addr = value;
	}

	// Write word to memory
	inline void volatile io::writeMemory16(word_t* const addr, const word_t value) noexcept {
		*addr = value;
	}

	// Write long to memory
	inline void volatile io::writeMemory32(dword_t* const addr, const dword_t value) noexcept {
		*addr = value;
	}


}	// namespace igros::x86_64

