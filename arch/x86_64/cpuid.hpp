////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		CPUID detection
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	/// @brief CPUID leaves
	enum class CPUID : igros_dword_t {
		VENDOR		  = 0x00000000_u32, ///< Max basic leaf and vendor string
		FEATURES	  = 0x00000001_u32, ///< Version and feature flags
		CACHE_TLB	  = 0x00000002_u32, ///< Cache and TLB descriptors
		EXTENDED_FEATURES = 0x00000007_u32, ///< Structured extended features (subleaf 0)
		EXTENDED_MAX	  = 0x80000000_u32, ///< Max extended leaf
		EXTENDED_INFO	  = 0x80000001_u32, ///< Extended feature flags (long mode, NX, 1 GB pages)
		ADDRESS_SIZES	  = 0x80000008_u32, ///< Physical and linear address sizes
	};

	/// @brief CPUID extended feature flags (leaf 80000001h, EDX)
	enum class CPUID_EXT_EDX : igros_dword_t {
		NX		= 0x00100000_u32,	///< No-execute page protection
		PAGE_1GB	= 0x04000000_u32,	///< 1 GB pages
		LONG_MODE	= 0x20000000_u32,	///< Long mode (64-bit)
	};

	/// @brief CPUID result registers
	struct cpuidRegs_t {
		igros_dword_t eax; ///< EAX register value
		igros_dword_t ebx; ///< EBX register value
		igros_dword_t ecx; ///< ECX register value
		igros_dword_t edx; ///< EDX register value
	};


	/// @brief Execute CPUID (CPUID is always available in long mode; caller checks leaf is supported)
	/// @param leaf CPUID leaf (EAX)
	/// @param subleaf CPUID subleaf (ECX)
	/// @return EAX, EBX, ECX and EDX values
	[[nodiscard]]
	inline auto cpuid(const CPUID leaf, const igros_dword_t subleaf = 0_u32) noexcept -> cpuidRegs_t {
		auto regs {cpuidRegs_t {}};
		asm volatile(
			"cpuid"
			: "=a"(regs.eax), "=b"(regs.ebx), "=c"(regs.ecx), "=d"(regs.edx)
			: "a"(static_cast<igros_dword_t>(leaf)), "c"(subleaf)
		);
		return regs;
	}

	/// @brief Check CPUID leaf is supported (basic and extended ranges are checked separately)
	/// @param leaf CPUID leaf
	/// @return true if leaf is within supported range
	[[nodiscard]]
	inline auto cpuidHasLeaf(const CPUID leaf) noexcept -> bool {
		const auto value {static_cast<igros_dword_t>(leaf)};
		const auto base {value & 0x80000000_u32};
		return cpuid(static_cast<CPUID>(base)).eax >= value;
	}


} // namespace igros::x86_64

