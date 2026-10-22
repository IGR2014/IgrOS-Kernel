////////////////////////////////////////////////////////////////
//
//	Task state segment (x86_64)
//
//	File:	tss.hpp
//	Date:	22 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <array>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Task state segment: in long mode only stack pointers for privilege
	// changes (RSP0..2) and interrupt stack table (IST1..7)
	class tss final {

		// Copy c-tor
		tss(const tss &other) = delete;
		// Copy assignment
		auto	operator=(const tss &other) -> tss& = delete;


	public:

		// Interrupt stack table index (0 - no stack switch)
		enum class IST : igros_byte_t {
			NONE		= 0_u8,
			DOUBLE_FAULT	= 1_u8,		// #DF: kernel stack may be broken (overflow)
			NMI		= 2_u8,		// NMI: may arrive at any instruction
			MACHINE_CHECK	= 3_u8		// #MC: may arrive at any instruction
		};

		// Interrupt stack size
		constexpr static auto	STACK_SIZE	{0x4000_usize};

#pragma pack(push, 1)

		// Task state segment layout
		struct segment_t {
			igros_dword_t			reserved0;
			std::array<igros_quad_t, 3>	rsp;		// Stack for CPL change to ring 0..2
			igros_quad_t			reserved1;
			std::array<igros_quad_t, 7>	ist;		// Interrupt stack table (IST1..IST7)
			igros_quad_t			reserved2;
			igros_word_t			reserved3;
			igros_word_t			iomapBase;	// I/O permission bitmap offset
		};

#pragma pack(pop)

		static_assert(104_usize == sizeof(segment_t), "TSS layout must match CPU");

		// Default c-tor
		tss() noexcept = delete;

		// Set up task state segment interrupt stacks (returns static segment for GDT)
		[[nodiscard]]
		static auto	init() noexcept -> const segment_t&;


	};


}	// namespace igros::x86_64

