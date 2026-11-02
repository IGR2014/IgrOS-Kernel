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
#include <cstddef>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/tss.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Set up task state segment interrupt stacks
	[[nodiscard]]
	auto tss::init() noexcept -> const segment_t& {

		// Interrupt stacks (one per IST entry in use)
		alignas(16) constinit static std::array<std::array<igros_byte_t, STACK_SIZE>, 3> stacks {};
		// Quad fields of the packed layout are at offsets 4 + 8n: segment starts 4 bytes
		// past an 8 byte boundary, so they are naturally aligned (misaligned access is UB)
		struct alignas(igros_quad_t) storage_t {
			igros_dword_t	padding;
			segment_t	segment;
		};
		static_assert(sizeof(igros_dword_t) == offsetof(storage_t, segment), "TSS quad fields must be aligned");
		// Task state segment (no I/O permission bitmap: offset past segment limit)
		constinit static storage_t storage {
			.padding	= 0_u32,
			.segment	= {
				.reserved0	= 0_u32,
				.rsp		= {},
				.reserved1	= 0_u64,
				.ist		= {},
				.reserved2	= 0_u64,
				.reserved3	= 0_u16,
				.iomapBase	= static_cast<igros_word_t>(sizeof(segment_t)),
			},
		};
		auto &segment {storage.segment};

		// Stacks grow down: IST entry holds stack top
		for (auto index {0_usize}; index < stacks.size(); ++index) {
			segment.ist[index] = static_cast<igros_quad_t>(std::bit_cast<igros_usize_t>(stacks[index].data() + stacks[index].size()));
		}
		return segment;

	}


}	// namespace igros::x86_64
