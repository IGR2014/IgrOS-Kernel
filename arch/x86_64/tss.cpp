////////////////////////////////////////////////////////////////
//
//	Task state segment (x86_64)
//
//	File:	tss.cpp
//	Date:	22 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <bit>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/tss.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Set up task state segment interrupt stacks
	[[nodiscard]]
	auto tss::init() noexcept -> const segment_t& {

		// Interrupt stacks (one per IST entry in use)
		alignas(16) constinit static std::array<std::array<igros_byte_t, STACK_SIZE>, 3> stacks {};
		// Task state segment (no I/O permission bitmap: offset past segment limit)
		constinit static segment_t segment {
			.reserved0	= 0_u32,
			.rsp		= {},
			.reserved1	= 0_u64,
			.ist		= {},
			.reserved2	= 0_u64,
			.reserved3	= 0_u16,
			.iomapBase	= static_cast<igros_word_t>(sizeof(segment_t))
		};

		// Stacks grow down: IST entry holds stack top
		for (auto index {0_usize}; index < stacks.size(); ++index) {
			segment.ist[index] = static_cast<igros_quad_t>(std::bit_cast<igros_usize_t>(stacks[index].data() + stacks[index].size()));
		}
		return segment;

	}


}	// namespace igros::x86_64
