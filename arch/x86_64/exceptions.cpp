////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.cpp
//	Date:	13 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <exceptions.hpp>
#include <taskRegs.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


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
	void except::exDefaultHandler(const taskRegs_t* regs) noexcept {
		// Print exception name
		klib::kprintf("Exception:\t%s", except::NAME[regs->number]);
		// Hang CPU
		while(true) {};
	}


}	// namespace arch

