////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	io.hpp
//	Date:	07 Feb 2021
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
	using portx86_64_t = word_t;


}	// namespace igros::x86_64


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Read byte from port
	[[nodiscard]]
	inline igros::byte_t volatile	outPort8(const igros::x86_64::portx86_64_t addr) noexcept;
	// Read word from port
	[[nodiscard]]
	inline igros::word_t volatile	outPort16(const igros::x86_64::portx86_64_t addr) noexcept;
	// Read long from port
	[[nodiscard]]
	inline igros::dword_t volatile	outPort32(const igros::x86_64::portx86_64_t addr) noexcept;

	// Write byte to port
	inline void volatile inPort8(const igros::x86_64::portx86_64_t addr, const igros::byte_t value) noexcept;
	// Write word to port
	inline void volatile inPort16(const igros::x86_64::portx86_64_t addr, const igros::word_t value) noexcept;
	// Write long to port
	inline void volatile inPort32(const igros::x86_64::portx86_64_t addr, const igros::dword_t value) noexcept;

#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86_64 architecture namespace
namespace igros::x86_64 {


	// I/O structure
	class iox86_64 final {

		// Copy c-tor
		iox86_64(const iox86_64 &other) = delete;
		// Copy assignment
		iox86_64& operator=(const iox86_64 &other) = delete;

		// Move c-tor
		iox86_64(iox86_64 &&other) = delete;
		// Move assignment
		iox86_64& operator=(iox86_64 &&other) = delete;


	public:

		// Default c-tor
		iox86_64() noexcept = default;

		// Read byte from port
		[[nodiscard]]
		static byte_t volatile	readPort8(const portx86_64_t addr) noexcept;
		// Read word from port
		[[nodiscard]]
		static word_t volatile	readPort16(const portx86_64_t addr) noexcept;
		// Read long from port
		[[nodiscard]]
		static dword_t volatile	readPort32(const portx86_64_t addr) noexcept;

		// Write byte to port
		static void volatile	writePort8(const portx86_64_t addr, const byte_t value) noexcept;
		// Write word to port
		static void volatile	writePort16(const portx86_64_t addr, const word_t value) noexcept;
		// Write long to port
		static void volatile	writePort32(const portx86_64_t addr, const dword_t value) noexcept;

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
	inline byte_t volatile iox86_64::readPort8(const portx86_64_t addr) noexcept {
		return outPort8(addr);
	}

	// Read word from port
	[[nodiscard]]
	inline word_t volatile iox86_64::readPort16(const portx86_64_t addr) noexcept {
		return outPort16(addr);
	}

	// Read long from port
	[[nodiscard]]
	inline dword_t volatile iox86_64::readPort32(const portx86_64_t addr) noexcept {
		return outPort32(addr);
	}


	// Write byte to port
	inline void volatile iox86_64::writePort8(const portx86_64_t addr, const byte_t value) noexcept {
		inPort8(addr, value);
	}

	// Write word to port
	inline void volatile iox86_64::writePort16(const portx86_64_t addr, const word_t value) noexcept {
		inPort16(addr, value);
	}

	// Write long to port
	inline void volatile iox86_64::writePort32(const portx86_64_t addr, const dword_t value) noexcept {
		inPort32(addr, value);
	}


	// Read byte from memory
	[[nodiscard]]
	inline byte_t volatile iox86_64::readMemory8(const byte_t* const addr) noexcept {
		return *addr;
	}

	// Read word from memory
	[[nodiscard]]
	inline word_t volatile iox86_64::readMemory16(const word_t* const addr) noexcept {
		return *addr;
	}

	// Read long from memory
	[[nodiscard]]
	inline dword_t volatile iox86_64::readMemory32(const dword_t* const addr) noexcept {
		return *addr;
	}


	// Write byte to memory
	inline void volatile iox86_64::writeMemory8(byte_t* const addr, const byte_t value) noexcept {
		*addr = value;
	}

	// Write word to memory
	inline void volatile iox86_64::writeMemory16(word_t* const addr, const word_t value) noexcept {
		*addr = value;
	}

	// Write long to memory
	inline void volatile iox86_64::writeMemory32(dword_t* const addr, const dword_t value) noexcept {
		*addr = value;
	}


}	// namespace igros::x86_64

