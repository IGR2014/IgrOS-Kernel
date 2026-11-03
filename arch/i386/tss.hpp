////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Task state segment (i386)
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// i386 namespace
namespace igros::i386 {


	/// @brief Task state segments: kernel task (CPU saves interrupted state there on
	///
	/// task switch, ring 0 stack for future user mode) and double fault task
	/// (reached through IDT task gate: known good stack and page directory
	/// even if kernel stack overflowed)
	class tss final {

	public:

		/// @brief D-tor
		~tss() noexcept = default;

		/// @brief Copy c-tor
		tss(const tss &other) = delete;
		/// @brief Copy assignment
		auto operator=(const tss &other) -> tss & = delete;

		/// @brief Double fault task stack size
		constexpr static auto STACK_SIZE {0x4000_usize};

#pragma pack(push, 1)

		/// @brief Task state segment layout (segment registers in low 16 bits)
		struct segment_t {
			igros_dword_t link;	 ///< Previous task selector
			igros_dword_t esp0;	 ///< Stack for CPL change to ring 0
			igros_dword_t ss0;	 ///< Stack segment for CPL change to ring 0
			igros_dword_t esp1;	 ///< Stack for CPL change to ring 1
			igros_dword_t ss1;	 ///< Stack segment for CPL change to ring 1
			igros_dword_t esp2;	 ///< Stack for CPL change to ring 2
			igros_dword_t ss2;	 ///< Stack segment for CPL change to ring 2
			igros_dword_t cr3;	 ///< Page directory
			igros_dword_t eip;	 ///< Instruction pointer
			igros_dword_t eflags;	 ///< Flags register
			igros_dword_t eax;	 ///< Accumulator register
			igros_dword_t ecx;	 ///< Counter register
			igros_dword_t edx;	 ///< Data register
			igros_dword_t ebx;	 ///< Base register
			igros_dword_t esp;	 ///< Stack pointer
			igros_dword_t ebp;	 ///< Stack base pointer
			igros_dword_t esi;	 ///< Source index register
			igros_dword_t edi;	 ///< Destination index register
			igros_dword_t es;	 ///< Extra segment
			igros_dword_t cs;	 ///< Code segment
			igros_dword_t ss;	 ///< Stack segment
			igros_dword_t ds;	 ///< Data segment
			igros_dword_t fs;	 ///< Extra segment 2
			igros_dword_t gs;	 ///< Extra segment 3
			igros_dword_t ldt;	 ///< LDT selector
			igros_word_t  trap;	 ///< Debug trap on task switch
			igros_word_t  iomapBase; ///< I/O permission bitmap offset
		};

#pragma pack(pop)

		static_assert(104_usize == sizeof(segment_t), "TSS layout must match CPU");

		/// @brief Default c-tor
		tss() noexcept = delete;

		/// @brief Set up task state segments (double fault task uses current page directory)
		/// @param codeSelector Kernel code segment selector
		/// @param dataSelector Kernel data segment selector
		static void init(const igros_word_t codeSelector, const igros_word_t dataSelector) noexcept;

		/// @brief Kernel task state segment
		/// @return Kernel task state segment
		[[nodiscard]]
		static auto kernel() noexcept -> segment_t &;
		/// @brief Double fault task state segment
		/// @return Double fault task state segment
		[[nodiscard]]
		static auto doubleFault() noexcept -> segment_t &;
	};


} // namespace igros::i386

