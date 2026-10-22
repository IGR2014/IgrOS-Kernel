////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	19 Dec 2023
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <algorithm>
#include <iterator>
// IgrOS-Kernel arch x86
#include <arch/x86/exceptions.hpp>
#include <arch/x86/irq.hpp>
#include <arch/x86/native.hpp>

#if	defined (IGROS_ARCH_i386)

// IgrOS-Kernel arch i386
#include <arch/i386/gdt.hpp>
#include <arch/i386/idt.hpp>

#elif	defined (IGROS_ARCH_x86_64)

// IgrOS-Kernel arch x86_64
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/tss.hpp>

#endif


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Load IDT
	void	idtLoad(const igros::x86::native::idt::pointer_t* const idtPtr) noexcept;
	// Store IDT
	void	idtStore(igros::x86::native::idt::pointer_t* const idtPtr) noexcept;


#ifdef	__cplusplus

}

#endif	// __cplusplus


// OS namespace (members of native idt are defined in namespace enclosing it)
namespace igros {


	// Set IDT pointer
	void x86::native::idt::setPointer(const pointer_t &pointer) noexcept {
		// Load new IDT
		::idtLoad(&pointer);
	}


	// Init IDT table
	void x86::native::idt::init() noexcept {

		// Kernel code segment selector
		constexpr auto KERNEL_CODE	{x86::native::gdt::KERNEL_CODE};
		// Present, ring 0, interrupt gate
		constexpr auto INTERRUPT_GATE	{0x8E_u8};

		// Every exception and IRQ vector must fit into IDT
		static_assert(std::size(::irqHandlerTable) == x86::IRQ_LINES);
		static_assert(std::size(::exHandlerTable) <= x86::IRQ_OFFSET);

		// Exceptions and IRQ descriptors table (IDT)
		constinit static idt::table_t table {};

		// Kernel interrupt gate for handler
		const auto gate = [](const auto handler) noexcept {
			return idt::setEntry(handler, KERNEL_CODE, INTERRUPT_GATE);
		};
		// Exceptions setup (vectors from 0)
		std::ranges::transform(::exHandlerTable, table.begin(), gate);
		// IRQs setup (vectors from IRQ offset)
		std::ranges::transform(::irqHandlerTable, table.begin() + x86::irqVector(x86::irq_t::PIT), gate);

		// Exceptions that can't trust current stack
		using enum x86::except::NUMBER;
		const auto vector = [](const x86::except::NUMBER number) noexcept {
			return static_cast<igros_usize_t>(number);
		};

#if	defined (IGROS_ARCH_i386)

		// Double fault: task gate to double fault task (own stack and page directory)
		constexpr auto TASK_GATE	{0x85_u8};
		table[vector(DOUBLE_FAULT)]	= idt::setEntry(nullptr, x86::native::gdt::TSS_DOUBLE_FAULT, TASK_GATE);

#elif	defined (IGROS_ARCH_x86_64)

		// Double fault, NMI and machine check: own interrupt stacks (TSS IST)
		table[vector(DOUBLE_FAULT)].ist		= static_cast<igros_byte_t>(x86::native::tss::IST::DOUBLE_FAULT);
		table[vector(NON_MASKABLE_IRQ)].ist	= static_cast<igros_byte_t>(x86::native::tss::IST::NMI);
		table[vector(MACHINE_CHECK)].ist	= static_cast<igros_byte_t>(x86::native::tss::IST::MACHINE_CHECK);

#endif

		// Pointer to IDT
		constinit static idt::pointer_t pointer {
			idt::calcSize(table),
			table.data()
		};

		// Set IDT table
		idt::setPointer(pointer);

	}


}	// namespace igros

