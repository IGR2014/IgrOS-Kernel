////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.cpp
//	Date:	16 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/i386/exceptions.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/register.hpp>
#include <arch/i386/cpu.hpp>

#include <klib/kprint.hpp>


// i386 namespace
namespace igros::i386 {


	// Init exceptions
	void except::init() noexcept {
		// Set default exception handlers
		except::install(except::NUMBER::DIVIDE_BY_ZERO,			except::exDefaultHandler);
		except::install(except::NUMBER::DEBUG,				except::exDefaultHandler);
		except::install(except::NUMBER::NON_MASKABLE_IRQ,		except::exDefaultHandler);
		except::install(except::NUMBER::BREAKPOINT,			except::exDefaultHandler);
		except::install(except::NUMBER::INTO_DETECTED_OVERFLOW,		except::exDefaultHandler);
		except::install(except::NUMBER::BOUND_RANGE_EXCEEDED,		except::exDefaultHandler);
		except::install(except::NUMBER::INVALID_OPCODE,			except::exDefaultHandler);
		except::install(except::NUMBER::NO_COPROCESSOR,			except::exDefaultHandler);
		except::install(except::NUMBER::DOUBLE_FAULT,			except::exDefaultHandler);
		except::install(except::NUMBER::COPROCESSOR_SEGMENT_OVERRUN,	except::exDefaultHandler);
		except::install(except::NUMBER::INVALID_TSS,			except::exDefaultHandler);
		except::install(except::NUMBER::SEGMENT_NOT_PRESENT,		except::exDefaultHandler);
		except::install(except::NUMBER::STACK_FAULT,			except::exDefaultHandler);
		except::install(except::NUMBER::GENERAL_PROTECTION_FAULT,	except::exDefaultHandler);
		except::install(except::NUMBER::PAGE_FAULT,			except::exDefaultHandler);
		except::install(except::NUMBER::UNKNOWN_IRQ,			except::exDefaultHandler);
		except::install(except::NUMBER::COPROCESSOR_FAULT,		except::exDefaultHandler);
		except::install(except::NUMBER::ALIGNMENT_CHECK,		except::exDefaultHandler);
		except::install(except::NUMBER::MACHINE_CHECK,			except::exDefaultHandler);
	}


	// Default exception handler
	void except::exDefaultHandler(const register_t* regs) noexcept {
		// Disable interrupts
		irq::disable();
		// Print exception name
		klib::kprintf("Exception:\t%s", except::NAME[regs->number]);
		// Hang CPU
		cpu::halt();
	}


	// Install handler
	void except::install(const NUMBER exNumber, const isr_t handler) noexcept {
		// Install ISR
		isrHandlerInstall(static_cast<dword_t>(exNumber), handler);
	}

	// Uninstall handler
	void except::uninstall(const NUMBER exNumber) noexcept {
		// Uninstall ISR
		isrHandlerUninstall(static_cast<dword_t>(exNumber));
	}


}	// namespace igros::i386

