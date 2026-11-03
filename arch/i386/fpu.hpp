////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		FPU operations
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kPrint.hpp>


#ifdef __cplusplus

extern "C" {

#endif // __cplusplus


	/// @brief Check FPU
	/// @return true if FPU is present
	[[nodiscard]]
	auto fpuCheck() noexcept -> bool;


#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus


// i386 platform namespace
namespace igros::i386 {


	/// @brief FPU representation
	class fpu final {

		/// @brief Copy c-tor
		fpu(const fpu &other) = delete;
		/// @brief Copy assignment
		fpu &operator=(const fpu &other) = delete;

		/// @brief Move c-tor
		fpu(fpu &&other) = delete;
		/// @brief Move assignment
		fpu &operator=(fpu &&other) = delete;


	public:

		/// @brief Default c-tor
		fpu() noexcept = default;

		/// @brief Check FPU
		/// @return true if FPU is present
		[[nodiscard]]
		static auto check() noexcept -> bool;
	};


	/// @brief Check FPU
	[[nodiscard]]
	inline auto fpu::check() noexcept -> bool {
		return ::fpuCheck();
	}


} // namespace igros::i386

