////////////////////////////////////////////////////////////////
//
//	I/O operations
//
//	File:	io.hpp
//	Date:	12 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++ headers
#include <cstdint>

// Common headers
#include <singleton.hpp>

// i386
#include <arch/i386/io.hpp>
// x86_64
#include <arch/x86_64/io.hpp>


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
		byte_t	readPort8(const port_t addr) const noexcept;
		// Read word from port
		[[nodiscard]]
		word_t	readPort16(const port_t addr) const noexcept;
		// Read double word from port
		[[nodiscard]]
		dword_t	readPort32(const port_t addr) const noexcept;

		// Read count bytes from port
		[[maybe_unused]]
		byte_t*		readPort8Rep(const port_t addr, const byte_t* const base, const std::size_t count) const noexcept;
		// Read count words from port
		[[maybe_unused]]
		word_t*		readPort16Rep(const port_t addr, const word_t* const base, const std::size_t count) const noexcept;
		// Read count double words from port
		[[maybe_unused]]
		dword_t*	readPort32Rep(const port_t addr, const dword_t* const base, const std::size_t count) const noexcept;

		// Write byte to port
		void	writePort8(const port_t addr, const byte_t value) const noexcept;
		// Write word to port
		void	writePort16(const port_t addr, const word_t value) const noexcept;
		// Write double word to port
		void	writePort32(const port_t addr, const dword_t value) const noexcept;

		// Write count bytes to port
		void	writePort8Rep(const port_t addr, byte_t* const value, const std::size_t count) const noexcept;
		// Write count words to port
		void	writePort16Rep(const port_t addr, word_t* const value, const std::size_t count) const noexcept;
		// Write count double words to port
		void	writePort32Rep(const port_t addr, dword_t* const value, const std::size_t count) const noexcept;

		// Read byte from I/O memory
		[[nodiscard]]
		byte_t	readMemory8(const byte_t* const addr) const noexcept;
		// Read word from I/O memory
		[[nodiscard]]
		word_t	readMemory16(const word_t* const addr) const noexcept;
		// Read double word from I/O memory
		[[nodiscard]]
		dword_t	readMemory32(const dword_t* const addr) const noexcept;

		// Read count bytes from I/O memory
		[[maybe_unused]]
		byte_t*		readMemory8Rep(const byte_t* const addr, const byte_t* const base, const std::size_t count) const noexcept;
		// Read count words from I/O memory
		[[maybe_unused]]
		word_t*		readMemory16Rep(const word_t* const addr, const word_t* const base, const std::size_t count) const noexcept;
		// Read count double words from I/O memory
		[[maybe_unused]]
		dword_t*	readMemory32Rep(const dword_t* const addr, const dword_t* const base, const std::size_t count) const noexcept;

		// Write byte to I/O memory
		void	writeMemory8(byte_t* const addr, const byte_t value) const noexcept;
		// Write word to I/O memory
		void	writeMemory16(word_t* const addr, const word_t value) const noexcept;
		// Write double word to I/O memory
		void	writeMemory32(dword_t* const addr, const dword_t value) const noexcept;

		// Write count bytes to I/O memory
		void	writeMemory8Rep(byte_t* const addr, const byte_t* const value, const std::size_t count) const noexcept;
		// Write count words to I/O memory
		void	writeMemory16Rep(word_t* const addr, const word_t* const value, const std::size_t count) const noexcept;
		// Write count double words to I/O memory
		void	writeMemory32Rep(dword_t* const addr, const dword_t* const value, const std::size_t count) const noexcept;


	};


	// Read byte from port
	template<typename T, typename T2>
	[[nodiscard]]
	inline byte_t io_t<T, T2>::readPort8(const port_t addr) const noexcept {
		return T::readPort8(addr);
	}

	// Read word from port
	template<typename T, typename T2>
	[[nodiscard]]
	inline word_t io_t<T, T2>::readPort16(const port_t addr) const noexcept {
		return T::readPort16(addr);
	}

	// Read double word from port
	template<typename T, typename T2>
	[[nodiscard]]
	inline dword_t io_t<T, T2>::readPort32(const port_t addr) const noexcept {
		return T::readPort32(addr);
	}


	// Read count bytes from port
	template<typename T, typename T2>
	[[maybe_unused]]
	inline byte_t* io_t<T, T2>::readPort8Rep(const port_t addr, const byte_t* base, const std::size_t count) const noexcept {
		// Read count of bytes to base buffer
		for (auto i = 0ULL; i < count; i++) {
			base[i] = readPort8(addr);
		}
		// Return pointer to base buffer
		return const_cast<byte_t* const>(base);
	}

	// Read count words from port
	template<typename T, typename T2>
	[[maybe_unused]]
	inline word_t* io_t<T, T2>::readPort16Rep(const port_t addr, const word_t* base, const std::size_t count) const noexcept {
		// Read count of words to base buffer
		for (auto i = 0ULL; i < count; i++) {
			base[i] = readPort16(addr);
		}
		// Return pointer to base buffer
		return const_cast<word_t* const>(base);
	}

	// Read count double words from port
	template<typename T, typename T2>
	[[maybe_unused]]
	inline dword_t* io_t<T, T2>::readPort32Rep(const port_t addr, const dword_t* base, const std::size_t count) const noexcept {
		// Read count of double words to base buffer
		for (auto i = 0ULL; i < count; i++) {
			base[i] = readPort32(addr);
		}
		// Return pointer to base buffer
		return const_cast<dword_t* const>(base);
	}


	// Write byte to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort8(const port_t addr, const byte_t value) const noexcept {
		T::writePort8(addr, value);
	}

	// Write word to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort16(const port_t addr, const word_t value) const noexcept {
		T::writePort16(addr, value);
	}

	// Write double word to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort32(const port_t addr, const dword_t value) const noexcept {
		T::writePort32(addr, value);
	}


	// Write count bytes to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort8Rep(const port_t addr, byte_t* const value, const std::size_t count) const noexcept {
		// Write count of bytes to port
		for (auto i = 0ULL; i < count; i++) {
			writePort8(addr, value[i]);
		}
	}

	// Write count words to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort16Rep(const port_t addr, word_t* const value, const std::size_t count) const noexcept {
		// Write count of words to port
		for (auto i = 0ULL; i < count; i++) {
			writePort16(addr, value[i]);
		}
	}

	// Write count double words to port
	template<typename T, typename T2>
	inline void io_t<T, T2>::writePort32Rep(const port_t addr, dword_t* const value, const std::size_t count) const noexcept {
		// Write count of double words to port
		for (auto i = 0ULL; i < count; i++) {
			writePort32(addr, value[i]);
		}
	}


	// Read byte from I/O memory
	template<typename T, typename T2>
	[[nodiscard]]
	inline byte_t io_t<T, T2>::readMemory8(const byte_t* const addr) const noexcept {
		return T::readMemory8(addr);
	}

	// Read word from I/O memory
	template<typename T, typename T2>
	[[nodiscard]]
	inline word_t io_t<T, T2>::readMemory16(const word_t* const addr) const noexcept {
		return T::readMemory16(addr);
	}

	// Read double word from I/O memory
	template<typename T, typename T2>
	[[nodiscard]]
	inline dword_t io_t<T, T2>::readMemory32(const dword_t* const addr) const noexcept {
		return T::readMemory32(addr);
	}


	// Read count bytes from I/O memory
	template<typename T, typename T2>
	[[maybe_unused]]
	inline byte_t* io_t<T, T2>::readMemory8Rep(const byte_t* const addr, const byte_t* const base, const std::size_t count) const noexcept {
		// Read count of bytes to base buffer
		for (auto i = 0ULL; i < count; i++) {
			base[i] = readMemory8(addr);
		}
		// Return pointer to base buffer
		return const_cast<byte_t* const>(base);
	}

	// Read count words from I/O memory
	template<typename T, typename T2>
	[[maybe_unused]]
	inline word_t* io_t<T, T2>::readMemory16Rep(const word_t* const addr, const word_t* const base, const std::size_t count) const noexcept {
		// Read count of words to base buffer
		for (auto i = 0ULL; i < count; i++) {
			base[i] = readMemory16(addr);
		}
		// Return pointer to base buffer
		return const_cast<word_t* const>(base);
	}

	// Read count double words from I/O memory
	template<typename T, typename T2>
	[[maybe_unused]]
	inline dword_t* io_t<T, T2>::readMemory32Rep(const dword_t* const addr, const dword_t* const base, const std::size_t count) const noexcept {
		// Read count of double words to base buffer
		for (auto i = 0ULL; i < count; i++) {
			base[i] = readMemory32(addr);
		}
		// Return pointer to base buffer
		return const_cast<dword_t* const>(base);
	}


	// Write byte to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory8(byte_t* const addr, const byte_t value) const noexcept {
		T::writeMemory8(addr, value);
	}

	// Write word to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory16(word_t* const addr, const word_t value) const noexcept {
		T::writeMemory16(addr, value);
	}

	// Write double word to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory32(dword_t* const addr, const dword_t value) const noexcept {
		T::writeMemory32(addr, value);
	}


	// Write byte to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory8Rep(byte_t* const addr, const byte_t* const value, const std::size_t count) const noexcept {
		// Write count of bytes to I/O memory
		for (auto i = 0ULL; i < count; i++) {
			writeMemory8(addr, value[i]);
		}
	}

	// Write word to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory16Rep(word_t* const addr, const word_t* const value, const std::size_t count) const noexcept {
		// Write count of words to I/O memory
		for (auto i = 0ULL; i < count; i++) {
			writeMemory16(addr, value[i]);
		}
	}

	// Write double word to I/O memory
	template<typename T, typename T2>
	inline void io_t<T, T2>::writeMemory32Rep(dword_t* const addr, const dword_t* const value, const std::size_t count) const noexcept {
		// Write count of double words to I/O memory
		for (auto i = 0ULL; i < count; i++) {
			writeMemory32(addr, value[i]);
		}
	}


#if	defined (IGROS_ARCH_i386)
	// I/O type
	using io = io_t<i386::ioi386, i386::porti386_t>;
#elif	defined (IGROS_ARCH_x86_64)
	// I/O type
	using io = io_t<x86_64::iox86_64, x86_64::portx86_64_t>;
#else
	// I/O type
	using io = io_t<void, void>;
	static_assert(false, u8"Unknown architecture!!!");
#endif


}	// namespace igros::arch

