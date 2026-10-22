////////////////////////////////////////////////////////////////
//
//	MSR registers operations
//
//	File:	msr.hpp
//	Date:	22 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Model specific registers
	enum class MSR : igros_dword_t {
		APIC_BASE	= 0x0000001B_u32,		// Local APIC base
		EFER		= 0xC0000080_u32,		// Extended feature enable
		STAR		= 0xC0000081_u32,		// SYSCALL segments
		LSTAR		= 0xC0000082_u32,		// SYSCALL 64-bit entry
		FMASK		= 0xC0000084_u32,		// SYSCALL RFLAGS mask
		FS_BASE		= 0xC0000100_u32,		// FS segment base
		GS_BASE		= 0xC0000101_u32,		// GS segment base
		KERNEL_GS_BASE	= 0xC0000102_u32		// SWAPGS target
	};

	// EFER register bits
	enum class EFER : igros_quad_t {
		SCE		= 0x0000000000000001_u64,	// SYSCALL enable
		LME		= 0x0000000000000100_u64,	// Long mode enable
		LMA		= 0x0000000000000400_u64,	// Long mode active
		NXE		= 0x0000000000000800_u64	// No-execute enable
	};


	// Read MSR register (RDMSR returns EDX:EAX)
	[[nodiscard]]
	inline auto readMSR(const MSR reg) noexcept -> igros_quad_t {
		auto low	{igros_dword_t {}};
		auto high	{igros_dword_t {}};
		asm volatile ("rdmsr" : "=a" (low), "=d" (high) : "c" (static_cast<igros_dword_t>(reg)));
		return (static_cast<igros_quad_t>(high) << 32) | low;
	}

	// Write MSR register (WRMSR takes EDX:EAX)
	inline void writeMSR(const MSR reg, const igros_quad_t value) noexcept {
		asm volatile (
			"wrmsr"
			:
			: "c" (static_cast<igros_dword_t>(reg)),
			  "a" (static_cast<igros_dword_t>(value)),
			  "d" (static_cast<igros_dword_t>(value >> 32))
			: "memory"
		);
	}


}	// namespace igros::x86_64

