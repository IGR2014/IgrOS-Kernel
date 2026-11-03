////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		CR0 - CR4 registers operations
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// x86 namespace
namespace igros::x86 {


	/// @brief CR0 register bits
	enum class CR0 : igros_usize_t {
		PE	= 0x00000001,	///< Protected mode enable
		MP	= 0x00000002,	///< Monitor coprocessor
		EM	= 0x00000004,	///< x87 emulation
		TS	= 0x00000008,	///< Task switched
		ET	= 0x00000010,	///< Extension type
		NE	= 0x00000020,	///< Native x87 errors
		WP	= 0x00010000,	///< Write protect (kernel can't write to read-only pages)
		AM	= 0x00040000,	///< Alignment mask
		NW	= 0x20000000,	///< Not write-through
		CD	= 0x40000000,	///< Cache disable
		PG	= 0x80000000,	///< Paging enable
	};


}	// namespace igros::x86


#ifdef __cplusplus

extern "C" {

#endif // __cplusplus


	/// @brief Read CR0 register
	/// @return CR0 value
	[[nodiscard]]
	auto readCR0() noexcept -> igros::igros_usize_t;
	/// @brief Read CR2 register
	/// @return CR2 value
	[[nodiscard]]
	auto readCR2() noexcept -> igros::igros_usize_t;
	/// @brief Read CR3 register
	/// @return CR3 value
	[[nodiscard]]
	auto readCR3() noexcept -> igros::igros_usize_t;
	/// @brief Read CR4 register
	/// @return CR4 value
	[[nodiscard]]
	auto readCR4() noexcept -> igros::igros_usize_t;

	/// @brief Write CR0 register
	/// @param value New CR0 value
	void writeCR0(const igros::igros_usize_t value) noexcept;
	/// @brief Write CR3 register
	/// @param value New CR3 value
	void writeCR3(const igros::igros_usize_t value) noexcept;
	/// @brief Write CR4 register
	/// @param value New CR4 value
	void writeCR4(const igros::igros_usize_t value) noexcept;

	/// @brief Invalidate TLB entry of page
	/// @param page Virtual address inside the page
	void pageInvalidate(const void* const page) noexcept;


#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus

