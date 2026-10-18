////////////////////////////////////////////////////////////////
//
//	Interrupt service routines dispatch and exceptions tests
//
//	File:	test_isr.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <vector>
// IgrOS-Kernel arch
#include <arch/x86/exceptions.hpp>
#include <arch/x86/irq.hpp>
#include <arch/x86/isr.hpp>
// Tests
#include <tests/hw.hpp>


// Common ISR entry (called by assembly stubs)
extern "C" void isrHandler(const igros::x86::register_t* regs) noexcept;


using namespace igros::x86;
namespace hw = igros::test::hw;


// Last handled vector
static auto handled {std::vector<igros::igros_usize_t> {}};

// Test handler
static void handler(const igros::x86::register_t* const regs) noexcept {
	handled.push_back(static_cast<igros::igros_usize_t>(regs->number));
}


// Dispatch vector to common handler
static void dispatch(const igros::igros_usize_t vector) {
	auto regs {igros::x86::register_t {}};
	regs.number = static_cast<decltype(regs.number)>(vector);
	isrHandler(&regs);
}


// Installed exception handler is called, no EOI
static void testException() {
	hw::reset();
	handled.clear();
	except::install<except::NUMBER::BREAKPOINT, handler>();
	dispatch(3);
	IGROS_CHECK((std::vector<igros::igros_usize_t> {3}) == handled);
	IGROS_CHECK(hw::portLog.empty());
	except::uninstall<except::NUMBER::BREAKPOINT>();
}


// Installed IRQ handler is called and acknowledged
static void testIRQ() {
	hw::reset();
	handled.clear();
	irq::install<irq_t::RTC, handler>();
	dispatch(irqVector(irq_t::RTC));
	IGROS_CHECK((std::vector<igros::igros_usize_t> {irqVector(irq_t::RTC)}) == handled);
	IGROS_CHECK((std::vector<std::uint32_t> {0x20}) == hw::writes(PIC_SLAVE_CONTROL));
	IGROS_CHECK((std::vector<std::uint32_t> {0x20}) == hw::writes(PIC_MASTER_CONTROL));
	irq::uninstall<irq_t::RTC>();
}


// Spurious IRQs: handler not called, no EOI (IRQ 15 acknowledges cascade on master)
static void testSpurious() {
	hw::reset();
	handled.clear();
	irq::install<irq_t::LPT1, handler>();
	irq::install<irq_t::ATA_SECONDARY, handler>();
	hw::portReadHook = [](const std::uint16_t) -> std::uint32_t { return 0x00; };
	dispatch(irqVector(irq_t::LPT1));
	IGROS_CHECK(handled.empty());
	IGROS_CHECK((std::vector<std::uint32_t> {0x0B}) == hw::writes(PIC_MASTER_CONTROL));
	hw::portLog.clear();
	dispatch(irqVector(irq_t::ATA_SECONDARY));
	IGROS_CHECK(handled.empty());
	IGROS_CHECK((std::vector<std::uint32_t> {0x0B}) == hw::writes(PIC_SLAVE_CONTROL));
	IGROS_CHECK((std::vector<std::uint32_t> {0x20}) == hw::writes(PIC_MASTER_CONTROL));
	// Real IRQ 7 is handled
	hw::portReadHook = [](const std::uint16_t) -> std::uint32_t { return 0x80; };
	dispatch(irqVector(irq_t::LPT1));
	IGROS_CHECK((std::vector<igros::igros_usize_t> {irqVector(irq_t::LPT1)}) == handled);
	irq::uninstall<irq_t::LPT1>();
	irq::uninstall<irq_t::ATA_SECONDARY>();
}


// Unhandled vectors halt CPU with interrupts disabled
static void testUnhandled() {
	hw::reset();
	hw::irqEnabled = true;
	IGROS_CHECK_HALTS(dispatch(6));
	IGROS_CHECK(!hw::irqEnabled);
	IGROS_CHECK(hw::printed("EXCEPTION -> [#6]"));
	IGROS_CHECK(hw::printed("UNHANDLED! CPU halted!"));
	IGROS_CHECK(hw::printed("Registers dump:"));
	hw::reset();
	IGROS_CHECK_HALTS(dispatch(irqVector(irq_t::MOUSE)));
	IGROS_CHECK(hw::printed("IRQ -> [#12]"));
}


// Default exception handlers print name and halt
static void testDefaultHandlers() {
	except::init();
	hw::reset();
	hw::irqEnabled = true;
	IGROS_CHECK_HALTS(dispatch(0));
	IGROS_CHECK(!hw::irqEnabled);
	IGROS_CHECK(hw::printed("Exception:\tDIVIDE BY ZERO"));
	hw::reset();
	IGROS_CHECK_HALTS(dispatch(18));
	IGROS_CHECK(hw::printed("Exception:\tMACHINE CHECK"));
	// Reserved vectors have no default handler
	hw::reset();
	IGROS_CHECK_HALTS(dispatch(20));
	IGROS_CHECK(hw::printed("UNHANDLED!"));
}


int main() {
	testException();
	testIRQ();
	testSpurious();
	testUnhandled();
	testDefaultHandlers();
	return igros::test::result();
}

