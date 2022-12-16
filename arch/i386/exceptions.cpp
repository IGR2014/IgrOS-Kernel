////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.cpp
//	Date:	16 Dec 2022
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
		except::install(DIVIDE_BY_ZERO,			except::exDefaultHandler);
		except::install(DEBUG,				except::exDefaultHandler);
		except::install(NON_MASKABLE_IRQ,		except::exDefaultHandler);
		except::install(BREAKPOINT,			except::exDefaultHandler);
		except::install(INTO_DETECTED_OVERFLOW,		except::exDefaultHandler);
		except::install(BOUND_RANGE_EXCEEDED,		except::exDefaultHandler);
		except::install(INVALID_OPCODE,			except::exDefaultHandler);
		except::install(NO_COPROCESSOR,			except::exDefaultHandler);
		except::install(DOUBLE_FAULT,			except::exDefaultHandler);
		except::install(COPROCESSOR_SEGMENT_OVERRUN,	except::exDefaultHandler);
		except::install(INVALID_TSS,			except::exDefaultHandler);
		except::install(SEGMENT_NOT_PRESENT,		except::exDefaultHandler);
		except::install(STACK_FAULT,			except::exDefaultHandler);
		except::install(GENERAL_PROTECTION_FAULT,	except::exDefaultHandler);
		except::install(PAGE_FAULT,			except::exDefaultHandler);
		except::install(UNKNOWN_IRQ,			except::exDefaultHandler);
		except::install(COPROCESSOR_FAULT,		except::exDefaultHandler);
		except::install(ALIGNMENT_CHECK,		except::exDefaultHandler);
		except::install(MACHINE_CHECK,			except::exDefaultHandler);
	}


	// Default exception handler
	void except::exDefaultHandler(const register_t* regs) noexcept {
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

