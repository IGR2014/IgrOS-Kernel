////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.cpp
//	Date:	16 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch i386
#include <arch/i386/cpu.hpp>
#include <arch/i386/exceptions.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/register.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// i386 namespace
namespace igros::i386 {


	// Init exceptions
	void except::init() noexcept {
		// Using enum
		using enum except::NUMBER;
		// Set default exception handlers
		except::install<DIVIDE_BY_ZERO,			except::defaultHandler>();
		except::install<DEBUG,				except::defaultHandler>();
		except::install<NON_MASKABLE_IRQ,		except::defaultHandler>();
		except::install<BREAKPOINT,			except::defaultHandler>();
		except::install<INTO_DETECTED_OVERFLOW,		except::defaultHandler>();
		except::install<BOUND_RANGE_EXCEEDED,		except::defaultHandler>();
		except::install<INVALID_OPCODE,			except::defaultHandler>();
		except::install<NO_COPROCESSOR,			except::defaultHandler>();
		except::install<DOUBLE_FAULT,			except::defaultHandler>();
		except::install<COPROCESSOR_SEGMENT_OVERRUN,	except::defaultHandler>();
		except::install<INVALID_TSS,			except::defaultHandler>();
		except::install<SEGMENT_NOT_PRESENT,		except::defaultHandler>();
		except::install<STACK_FAULT,			except::defaultHandler>();
		except::install<GENERAL_PROTECTION_FAULT,	except::defaultHandler>();
		except::install<PAGE_FAULT,			except::defaultHandler>();
		except::install<UNKNOWN_IRQ,			except::defaultHandler>();
		except::install<COPROCESSOR_FAULT,		except::defaultHandler>();
		except::install<ALIGNMENT_CHECK,		except::defaultHandler>();
		except::install<MACHINE_CHECK,			except::defaultHandler>();
	}


	// Default exception handler
	[[noreturn]]
	void except::defaultHandler(const register_t* regs) noexcept {
		// Disable interrupts
		irq::disable();
		// Print exception name
		klib::kprintf(
			"Exception:\t%s",
			except::NAME[regs->number]
		);
		// Dump registres
		cpu::dumpRegisters(regs);
		// Hang CPU
		cpu::halt();
	}


}	// namespace igros::i386

