////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		IO low-level port operations
///	@date		16 Dec 2022
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// x86 architecture namespace
namespace igros::x86 {


	/// @brief Define port data type which is 16bit word
	using port_t	= igros_word_t;


}	// namespace igros::x86


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	/// @brief Read byte from port
	/// @param addr Port
	/// @return Read byte
	[[nodiscard]]
	auto	portRead8(const igros::x86::port_t addr) noexcept -> igros::igros_byte_t;
	/// @brief Read word from port
	/// @param addr Port
	/// @return Read word
	[[nodiscard]]
	auto	portRead16(const igros::x86::port_t addr) noexcept -> igros::igros_word_t;
	/// @brief Read long from port
	/// @param addr Port
	/// @return Read long
	[[nodiscard]]
	auto	portRead32(const igros::x86::port_t addr) noexcept -> igros::igros_dword_t;

	/// @brief Write byte to port
	/// @param addr Port
	/// @param value Written byte
	void	portWrite8(const igros::x86::port_t addr, const igros::igros_byte_t value) noexcept;
	/// @brief Write word to port
	/// @param addr Port
	/// @param value Written word
	void	portWrite16(const igros::x86::port_t addr, const igros::igros_word_t value) noexcept;
	/// @brief Write long to port
	/// @param addr Port
	/// @param value Written long
	void	portWrite32(const igros::x86::port_t addr, const igros::igros_dword_t value) noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86 architecture namespace
namespace igros::x86 {


	/// @brief I/O structure
	class io final {

	public:

		/// @brief Default c-tor
		io() noexcept = default;
		/// @brief D-tor
		~io() noexcept = default;

		/// @brief Copy c-tor
		io(const io &other) = delete;
		/// @brief Copy assignment
		auto	operator=(const io &other) -> io& = delete;
		/// @brief Move c-tor
		io(io &&other) = delete;
		/// @brief Move assignment
		auto	operator=(io &&other) -> io& = delete;

		/// @brief Read byte from port
		/// @param addr Port
		/// @return Read byte
		[[nodiscard]]
		static auto	readPort8(const port_t addr) noexcept -> igros_byte_t;
		/// @brief Read word from port
		/// @param addr Port
		/// @return Read word
		[[nodiscard]]
		static auto	readPort16(const port_t addr) noexcept -> igros_word_t;
		/// @brief Read long from port
		/// @param addr Port
		/// @return Read long
		[[nodiscard]]
		static auto	readPort32(const port_t addr) noexcept -> igros_dword_t;

		/// @brief Write byte to port
		/// @param addr Port
		/// @param value Written byte
		static void	writePort8(const port_t addr, const igros_byte_t value) noexcept;
		/// @brief Write word to port
		/// @param addr Port
		/// @param value Written word
		static void	writePort16(const port_t addr, const igros_word_t value) noexcept;
		/// @brief Write long to port
		/// @param addr Port
		/// @param value Written long
		static void	writePort32(const port_t addr, const igros_dword_t value) noexcept;

		/// @brief Read byte from memory
		/// @param addr Memory address (volatile access)
		/// @return Read byte
		[[nodiscard]]
		static auto	readMemory8(const igros_byte_t* const addr) noexcept -> igros_byte_t;
		/// @brief Read word from memory
		/// @param addr Memory address (volatile access)
		/// @return Read word
		[[nodiscard]]
		static auto	readMemory16(const igros_word_t* const addr) noexcept -> igros_word_t;
		/// @brief Read long from memory
		/// @param addr Memory address (volatile access)
		/// @return Read long
		[[nodiscard]]
		static auto	readMemory32(const igros_dword_t* const addr) noexcept -> igros_dword_t;

		/// @brief Write byte to memory
		/// @param addr Memory address (volatile access)
		/// @param value Written byte
		static void	writeMemory8(igros_byte_t* const addr, const igros_byte_t value) noexcept;
		/// @brief Write word to memory
		/// @param addr Memory address (volatile access)
		/// @param value Written word
		static void	writeMemory16(igros_word_t* const addr, const igros_word_t value) noexcept;
		/// @brief Write long to memory
		/// @param addr Memory address (volatile access)
		/// @param value Written long
		static void	writeMemory32(igros_dword_t* const addr, const igros_dword_t value) noexcept;


	};


	/// @brief Read byte from port
	[[nodiscard]]
	inline auto io::readPort8(const port_t addr) noexcept -> igros_byte_t {
		return ::portRead8(addr);
	}

	/// @brief Read word from port
	[[nodiscard]]
	inline auto io::readPort16(const port_t addr) noexcept -> igros_word_t {
		return ::portRead16(addr);
	}

	/// @brief Read long from port
	[[nodiscard]]
	inline auto io::readPort32(const port_t addr) noexcept -> igros_dword_t {
		return ::portRead32(addr);
	}


	/// @brief Write byte to port
	inline void io::writePort8(const port_t addr, const igros_byte_t value) noexcept {
		::portWrite8(addr, value);
	}

	/// @brief Write word to port
	inline void io::writePort16(const port_t addr, const igros_word_t value) noexcept {
		::portWrite16(addr, value);
	}

	/// @brief Write long to port
	inline void io::writePort32(const port_t addr, const igros_dword_t value) noexcept {
		::portWrite32(addr, value);
	}


	/// @brief Read byte from memory
	[[nodiscard]]
	inline auto io::readMemory8(const igros_byte_t* const addr) noexcept -> igros_byte_t {
		return *addr;
	}

	/// @brief Read word from memory
	[[nodiscard]]
	inline auto io::readMemory16(const igros_word_t* const addr) noexcept -> igros_word_t {
		return *addr;
	}

	/// @brief Read long from memory
	[[nodiscard]]
	inline auto io::readMemory32(const igros_dword_t* const addr) noexcept -> igros_dword_t {
		return *addr;
	}


	/// @brief Write byte to memory
	inline void io::writeMemory8(igros_byte_t* const addr, const igros_byte_t value) noexcept {
		*addr = value;
	}

	/// @brief Write word to memory
	inline void io::writeMemory16(igros_word_t* const addr, const igros_word_t value) noexcept {
		*addr = value;
	}

	/// @brief Write long to memory
	inline void io::writeMemory32(igros_dword_t* const addr, const igros_dword_t value) noexcept {
		*addr = value;
	}


}	// namespace igros::x86

