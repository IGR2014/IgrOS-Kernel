////////////////////////////////////////////////////////////////
//
//	I/O operations
//
//	File:	io.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <cstdint>
// IgrOS-Kernel arch i386
#include <arch/i386/io.hpp>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/io.hpp>
// IgrOS-Kernel library
#include <klib/ksingleton.hpp>


// Arch namespace
namespace igros::arch {


	// I/O description type
	template<typename T, typename T2>
	class io_t final : public singleton<io_t<T, T2>> {

		// No copy construction
		io_t(const io_t &other) noexcept = delete;
		// No copy assignment
		io_t& operator=(const io_t &other) noexcept = delete;

		// No move construction
		io_t(io_t &&other) noexcept = delete;
		// No move assignment
		io_t& operator=(io_t &&other) noexcept = delete;


	public:

		// Port address type
		using port_t = T2;

		// Default c-tor
		io_t() noexcept = default;

		// Read byte from port
		[[nodiscard]]
		auto	readPort8(const port_t addr) const noexcept -> igros_byte_t;
		// Read word from port
		[[nodiscard]]
		auto	readPort16(const port_t addr) const noexcept -> igros_word_t;
		// Read double word from port
		[[nodiscard]]
		auto	readPort32(const port_t addr) const noexcept -> igros_dword_t;

		// Read count bytes from port
		[[maybe_unused]]
		auto	readPort8Rep(const port_t addr, igros_byte_t* const base, const igros_usize_t count) const noexcept -> igros_byte_t*;
		// Read count words from port
		[[maybe_unused]]
		auto	readPort16Rep(const port_t addr, igros_word_t* const base, const igros_usize_t count) const noexcept -> igros_word_t*;
		// Read count double words from port
		[[maybe_unused]]
		auto	readPort32Rep(const port_t addr, igros_dword_t* const base, const igros_usize_t count) const noexcept -> igros_dword_t*;

		// Write byte to port
		void	writePort8(const port_t addr, const igros_byte_t value) const noexcept;
		// Write word to port
		void	writePort16(const port_t addr, const igros_word_t value) const noexcept;
		// Write double word to port
		void	writePort32(const port_t addr, const igros_dword_t value) const noexcept;

		// Write count bytes to port
		void	writePort8Rep(const port_t addr, const igros_byte_t* const value, const igros_usize_t count) const noexcept;
		// Write count words to port
		void	writePort16Rep(const port_t addr, const igros_word_t* const value, const igros_usize_t count) const noexcept;
		// Write count double words to port
		void	writePort32Rep(const port_t addr, const igros_dword_t* const value, const igros_usize_t count) const noexcept;

		// Read byte from I/O memory
		[[nodiscard]]
		auto	readMemory8(const igros_byte_t* const addr) const noexcept -> igros_byte_t;
		// Read word from I/O memory
		[[nodiscard]]
		auto	readMemory16(const igros_word_t* const addr) const noexcept -> igros_word_t;
		// Read double word from I/O memory
		[[nodiscard]]
		auto	readMemory32(const igros_dword_t* const addr) const noexcept -> igros_dword_t;

		// Read count bytes from I/O memory
		[[maybe_unused]]
		auto	readMemory8Rep(const igros_byte_t* const addr, igros_byte_t* const base, const igros_usize_t count) const noexcept -> igros_byte_t*;
		// Read count words from I/O memory
		[[maybe_unused]]
		auto	readMemory16Rep(const igros_word_t* const addr, igros_word_t* const base, const igros_usize_t count) const noexcept -> igros_word_t*;
		// Read count double words from I/O memory
		[[maybe_unused]]
		auto	readMemory32Rep(const igros_dword_t* const addr, igros_dword_t* const base, const igros_usize_t count) const noexcept -> igros_dword_t*;

		// Write byte to I/O memory
		void	writeMemory8(igros_byte_t* const addr, const igros_byte_t value) const noexcept;
		// Write word to I/O memory
		void	writeMemory16(igros_word_t* const addr, const igros_word_t value) const noexcept;
		// Write double word to I/O memory
		void	writeMemory32(igros_dword_t* const addr, const igros_dword_t value) const noexcept;

		// Write count bytes to I/O memory
		void	writeMemory8Rep(igros_byte_t* const addr, const igros_byte_t* const value, const igros_usize_t count) const noexcept;
		// Write count words to I/O memory
		void	writeMemory16Rep(igros_word_t* const addr, const igros_word_t* const value, const igros_usize_t count) const noexcept;
		// Write count double words to I/O memory
		void	writeMemory32Rep(igros_dword_t* const addr, const igros_dword_t* const value, const igros_usize_t count) const noexcept;


	};


	// Read byte from port
	template<typename T, typename T2>
	[[nodiscard]]
	inline auto io_t<T, T2>::readPort8(const port_t addr) const noexcept -> igros_byte_t {
		return T::readPort8(addr);
	}

	// Read word from port
	template<typename T, typename T2>
	[[nodiscard]]
	inline auto io_t<T, T2>::readPort16(const port_t addr) const noexcept -> igros_word_t {
		return T::readPort16(addr);
	}

	// Read double word from port
	template<typename T, typename T2>
	[[nodiscard]]
	inline auto io_t<T, T2>::readPort32(const port_t addr) const noexcept -> igros_dword_t {
		return T::readPort32(addr);
	}


	// Read count bytes from port
	template<typename T, typename T2>
	[[maybe_unused]]
	inline auto io_t<T, T2>::readPort8Rep(const port_t addr, igros_byte_t* const base, const igros_usize_t count) const noexcept -> igros_byte_t* {
		// Read count of bytes to base buffer
		for (auto i (0_usize); i < count; i++) {
			base[i] = readPort8(addr);
		}
		// Return pointer to base buffer
		return base;
	}

	// Read count words from port
	template<typename T, typename T2>
	[[maybe_unused]]
	inline auto io_t<T, T2>::readPort16Rep(const port_t addr, igros_word_t* const base, const igros_usize_t count) const noexcept -> igros_word_t* {
		// Read count of words to base buffer
		for (auto i {0_usize}; i < count; i++) {
			base[i] = readPort16(addr);
		}
		// Return pointer to base buffer
		return base;
	}

	// Read count double words from port
	template<typename T, typename T2>
	[[maybe_unused]]
	inline auto io_t<T, T2>::readPort32Rep(const port_t addr, igros_dword_t* const base, const igros_usize_t count) const noexcept -> igros_dword_t* {
		// Read count of double words to base buffer
		for (auto i {0_usize}; i < count; i++) {
			base[i] = readPort32(addr);
		}
		// Return pointer to base buffer
		return base;
	}


	// Write byte to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort8(const port_t addr, const igros_byte_t value) const noexcept {
		T::writePort8(addr, value);
	}

	// Write word to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort16(const port_t addr, const igros_word_t value) const noexcept {
		T::writePort16(addr, value);
	}

	// Write double word to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort32(const port_t addr, const igros_dword_t value) const noexcept {
		T::writePort32(addr, value);
	}


	// Write count bytes to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort8Rep(const port_t addr, const igros_byte_t* const value, const igros_usize_t count) const noexcept {
		// Write count of bytes to port
		for (auto i {0_usize}; i < count; i++) {
			writePort8(addr, value[i]);
		}
	}

	// Write count words to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort16Rep(const port_t addr, const igros_word_t* const value, const igros_usize_t count) const noexcept {
		// Write count of words to port
		for (auto i {0_usize}; i < count; i++) {
			writePort16(addr, value[i]);
		}
	}

	// Write count double words to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort32Rep(const port_t addr, const igros_dword_t* const value, const igros_usize_t count) const noexcept {
		// Write count of double words to port
		for (auto i {0_usize}; i < count; i++) {
			writePort32(addr, value[i]);
		}
	}


	// Read byte from I/O memory
	template<typename T, typename T2>
	[[nodiscard]]
	inline auto io_t<T, T2>::readMemory8(const igros_byte_t* const addr) const noexcept -> igros_byte_t {
		return T::readMemory8(addr);
	}

	// Read word from I/O memory
	template<typename T, typename T2>
	[[nodiscard]]
	inline auto io_t<T, T2>::readMemory16(const igros_word_t* const addr) const noexcept -> igros_word_t {
		return T::readMemory16(addr);
	}

	// Read double word from I/O memory
	template<typename T, typename T2>
	[[nodiscard]]
	inline auto io_t<T, T2>::readMemory32(const igros_dword_t* const addr) const noexcept -> igros_dword_t {
		return T::readMemory32(addr);
	}


	// Read count bytes from I/O memory
	template<typename T, typename T2>
	[[maybe_unused]]
	inline auto io_t<T, T2>::readMemory8Rep(const igros_byte_t* const addr, igros_byte_t* const base, const igros_usize_t count) const noexcept -> igros_byte_t* {
		// Read count of bytes to base buffer
		for (auto i {0_usize}; i < count; i++) {
			base[i] = readMemory8(addr);
		}
		// Return pointer to base buffer
		return base;
	}

	// Read count words from I/O memory
	template<typename T, typename T2>
	[[maybe_unused]]
	inline auto io_t<T, T2>::readMemory16Rep(const igros_word_t* const addr, igros_word_t* const base, const igros_usize_t count) const noexcept -> igros_word_t* {
		// Read count of words to base buffer
		for (auto i {0_usize}; i < count; i++) {
			base[i] = readMemory16(addr);
		}
		// Return pointer to base buffer
		return base;
	}

	// Read count double words from I/O memory
	template<typename T, typename T2>
	[[maybe_unused]]
	inline auto io_t<T, T2>::readMemory32Rep(const igros_dword_t* const addr, igros_dword_t* const base, const igros_usize_t count) const noexcept -> igros_dword_t* {
		// Read count of double words to base buffer
		for (auto i {0_usize}; i < count; i++) {
			base[i] = readMemory32(addr);
		}
		// Return pointer to base buffer
		return base;
	}


	// Write byte to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory8(igros_byte_t* const addr, const igros_byte_t value) const noexcept {
		T::writeMemory8(addr, value);
	}

	// Write word to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory16(igros_word_t* const addr, const igros_word_t value) const noexcept {
		T::writeMemory16(addr, value);
	}

	// Write double word to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory32(igros_dword_t* const addr, const igros_dword_t value) const noexcept {
		T::writeMemory32(addr, value);
	}


	// Write byte to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory8Rep(igros_byte_t* const addr, const igros_byte_t* const value, const igros_usize_t count) const noexcept {
		// Write count of bytes to I/O memory
		for (auto i {0_usize}; i < count; i++) {
			writeMemory8(addr, value[i]);
		}
	}

	// Write word to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory16Rep(igros_word_t* const addr, const igros_word_t* const value, const igros_usize_t count) const noexcept {
		// Write count of words to I/O memory
		for (auto i {0_usize}; i < count; i++) {
			writeMemory16(addr, value[i]);
		}
	}

	// Write double word to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory32Rep(igros_dword_t* const addr, const igros_dword_t* const value, const igros_usize_t count) const noexcept {
		// Write count of double words to I/O memory
		for (auto i {0_usize}; i < count; i++) {
			writeMemory32(addr, value[i]);
		}
	}


#if	defined (IGROS_ARCH_i386)

	// I/O type
	using io	= io_t<i386::io, i386::port_t>;

#elif	defined (IGROS_ARCH_x86_64)

	// I/O type
	using io	= io_t<x86_64::io, x86_64::port_t>;

#else

	static_assert(
		false,
		"Unknown architecture!"
	);

	// I/O type
	using io	= io_t<void, void>;

#endif


}	// namespace igros::arch

