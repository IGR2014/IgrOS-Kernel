////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	io.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma	once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// x86_64 architecture namespace
namespace igros::x86_64 {


	// Define port data type which is 16bit word
	using port_t	= igros_word_t;


}	// namespace igros::x86_64


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Read byte from port
	[[nodiscard]]
	auto	outPort8(const igros::x86_64::port_t addr) noexcept -> igros::igros_byte_t;
	// Read word from port
	[[nodiscard]]
	auto	outPort16(const igros::x86_64::port_t addr) noexcept -> igros::igros_word_t;
	// Read long from port
	[[nodiscard]]
	auto	outPort32(const igros::x86_64::port_t addr) noexcept -> igros::igros_dword_t;

	// Write byte to port
	void	inPort8(const igros::x86_64::port_t addr, const igros::igros_byte_t value) noexcept;
	// Write word to port
	void	inPort16(const igros::x86_64::port_t addr, const igros::igros_word_t value) noexcept;
	// Write long to port
	void	inPort32(const igros::x86_64::port_t addr, const igros::igros_dword_t value) noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86_64 architecture namespace
namespace igros::x86_64 {


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
		static auto	readPort8(const port_t addr) noexcept -> igros_byte_t;
		// Read word from port
		[[nodiscard]]
		static auto	readPort16(const port_t addr) noexcept -> igros_word_t;
		// Read long from port
		[[nodiscard]]
		static auto	readPort32(const port_t addr) noexcept -> igros_dword_t;

		// Write byte to port
		static void	writePort8(const port_t addr, const igros_byte_t value) noexcept;
		// Write word to port
		static void	writePort16(const port_t addr, const igros_word_t value) noexcept;
		// Write long to port
		static void	writePort32(const port_t addr, const igros_dword_t value) noexcept;

		// Read byte from memory
		[[nodiscard]]
		static auto	readMemory8(const igros_byte_t* const addr) noexcept -> igros_byte_t;
		// Read word from memory
		[[nodiscard]]
		static auto	readMemory16(const igros_word_t* const addr) noexcept -> igros_word_t;
		// Read long from memory
		[[nodiscard]]
		static auto	readMemory32(const igros_dword_t* const addr) noexcept -> igros_dword_t;

		// Write byte to memory
		static void	writeMemory8(igros_byte_t* const addr, const igros_byte_t value) noexcept;
		// Write word to memory
		static void	writeMemory16(igros_word_t* const addr, const igros_word_t value) noexcept;
		// Write long to memory
		static void	writeMemory32(igros_dword_t* const addr, const igros_dword_t value) noexcept;


	};


	// Read byte from port
	[[nodiscard]]
	inline auto io::readPort8(const port_t addr) noexcept -> igros_byte_t {
		return ::outPort8(addr);
	}

	// Read word from port
	[[nodiscard]]
	inline auto io::readPort16(const port_t addr) noexcept -> igros_word_t {
		return ::outPort16(addr);
	}

	// Read long from port
	[[nodiscard]]
	inline auto io::readPort32(const port_t addr) noexcept -> igros_dword_t {
		return ::outPort32(addr);
	}


	// Write byte to port
	inline void io::writePort8(const port_t addr, const igros_byte_t value) noexcept {
		::inPort8(addr, value);
	}

	// Write word to port
	inline void io::writePort16(const port_t addr, const igros_word_t value) noexcept {
		::inPort16(addr, value);
	}

	// Write long to port
	inline void io::writePort32(const port_t addr, const igros_dword_t value) noexcept {
		::inPort32(addr, value);
	}


	// Read byte from memory
	[[nodiscard]]
	inline auto io::readMemory8(const igros_byte_t* const addr) noexcept -> igros_byte_t {
		return *addr;
	}

	// Read word from memory
	[[nodiscard]]
	inline auto io::readMemory16(const igros_word_t* const addr) noexcept -> igros_word_t {
		return *addr;
	}

	// Read long from memory
	[[nodiscard]]
	inline auto io::readMemory32(const igros_dword_t* const addr) noexcept -> igros_dword_t {
		return *addr;
	}


	// Write byte to memory
	inline void io::writeMemory8(igros_byte_t* const addr, const igros_byte_t value) noexcept {
		*addr = value;
	}

	// Write word to memory
	inline void io::writeMemory16(igros_word_t* const addr, const igros_word_t value) noexcept {
		*addr = value;
	}

	// Write long to memory
	inline void io::writeMemory32(igros_dword_t* const addr, const igros_dword_t value) noexcept {
		*addr = value;
	}


}	// namespace igros::x86_64

