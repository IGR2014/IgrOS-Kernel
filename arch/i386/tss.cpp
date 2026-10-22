////////////////////////////////////////////////////////////////
//
//	Task state segment (i386)
//
//	File:	tss.cpp
//	Date:	22 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <bit>
// IgrOS-Kernel arch i386
#include <arch/i386/tss.hpp>
// IgrOS-Kernel arch x86
#include <arch/x86/cr.hpp>
#include <arch/x86/native.hpp>
// IgrOS-Kernel library
#include <klib/kPrint.hpp>


// i386 namespace
namespace igros::i386 {


	// Task state segments
	namespace {


		// Empty task state segment (no I/O permission bitmap: offset past segment limit)
		constexpr tss::segment_t EMPTY_SEGMENT {
			.link = 0_u32, .esp0 = 0_u32, .ss0 = 0_u32, .esp1 = 0_u32, .ss1 = 0_u32, .esp2 = 0_u32, .ss2 = 0_u32,
			.cr3 = 0_u32, .eip = 0_u32, .eflags = 0_u32,
			.eax = 0_u32, .ecx = 0_u32, .edx = 0_u32, .ebx = 0_u32, .esp = 0_u32, .ebp = 0_u32, .esi = 0_u32, .edi = 0_u32,
			.es = 0_u32, .cs = 0_u32, .ss = 0_u32, .ds = 0_u32, .fs = 0_u32, .gs = 0_u32, .ldt = 0_u32,
			.trap = 0_u16, .iomapBase = static_cast<igros_word_t>(sizeof(tss::segment_t))
		};
		// Kernel task
		constinit tss::segment_t kernelSegment		{EMPTY_SEGMENT};
		// Double fault task
		constinit tss::segment_t doubleFaultSegment	{EMPTY_SEGMENT};
		// Double fault task stack
		alignas(16) constinit std::array<igros_byte_t, tss::STACK_SIZE> doubleFaultStack {};


		// Double fault task: interrupted state was saved in kernel task state segment
		[[noreturn]]
		void doubleFaultTask() noexcept {
			const auto &state {tss::kernel()};
			klib::kprintf(
R"exception(
EXCEPTION [#8]
Name:		DOUBLE FAULT (double fault task)
EIP:		0x%08x
ESP:		0x%08x
EBP:		0x%08x
CR2:		0x%08x
)exception",
				state.eip,
				state.esp,
				state.ebp,
				static_cast<igros_dword_t>(::readCR2())
			);
			x86::cpu::halt();
		}


	}	// namespace


	// Set up task state segments
	void tss::init(const igros_word_t codeSelector, const igros_word_t dataSelector) noexcept {
		// Kernel task: ring 0 stack is set when user mode appears
		kernelSegment.ss0		= dataSelector;
		// Double fault task: own stack, current (boot) page directory - never changed,
		// maps kernel image even if kernel page tables are broken; interrupts disabled
		doubleFaultSegment.cr3		= static_cast<igros_dword_t>(::readCR3());
		doubleFaultSegment.eip		= static_cast<igros_dword_t>(std::bit_cast<igros_usize_t>(&doubleFaultTask));
		doubleFaultSegment.eflags	= 0x00000002_u32;
		doubleFaultSegment.esp		= static_cast<igros_dword_t>(std::bit_cast<igros_usize_t>(doubleFaultStack.data() + doubleFaultStack.size()));
		doubleFaultSegment.cs		= codeSelector;
		doubleFaultSegment.ss		= dataSelector;
		doubleFaultSegment.ds		= dataSelector;
		doubleFaultSegment.es		= dataSelector;
		doubleFaultSegment.fs		= dataSelector;
		doubleFaultSegment.gs		= dataSelector;
	}


	// Kernel task state segment
	[[nodiscard]]
	auto tss::kernel() noexcept -> segment_t& {
		return kernelSegment;
	}

	// Double fault task state segment
	[[nodiscard]]
	auto tss::doubleFault() noexcept -> segment_t& {
		return doubleFaultSegment;
	}


}	// namespace igros::i386

