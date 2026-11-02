////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Interrupts low-level operations
///	@date		19 Dec 2023
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <array>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch x86
#include <arch/x86/native.hpp>


// x86 namespace
namespace igros::x86 {


	/// @brief IRQ offset in ISR list
	constexpr auto IRQ_OFFSET	{32_usize};
	/// @brief ISR list size
	constexpr auto ISR_SIZE		{256_usize};

	/// @brief Interrupt service routine handler type
	using isr_t			= std::add_pointer_t<void (const register_t* const)>;

	/// @brief Install interrupt service routine handler
	/// @param number Interrupt vector
	/// @param handle Handler
	void	isrHandlerInstall(const igros_usize_t number, const isr_t handle) noexcept;

	/// @brief Uninstall interrupt service routine handler
	/// @param number Interrupt vector
	void	isrHandlerUninstall(const igros_usize_t number) noexcept;


}	// namespace igros::x86

