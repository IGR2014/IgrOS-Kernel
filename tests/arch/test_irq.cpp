////////////////////////////////////////////////////////////////
//
//	8259 PIC (IRQ controller) tests
//
//	File:	test_irq.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <vector>
// IgrOS-Kernel arch
#include <arch/x86/irq.hpp>
// Tests
#include <tests/hw.hpp>


using namespace igros::x86;
namespace hw = igros::test::hw;


// Vector helpers
static void testVectors() {
	IGROS_CHECK(32 == irqVector(irq_t::PIT));
	IGROS_CHECK(47 == irqVector(irq_t::ATA_SECONDARY));
	IGROS_CHECK(!isIrqVector(31));
	IGROS_CHECK(isIrqVector(32));
	IGROS_CHECK(isIrqVector(47));
	IGROS_CHECK(!isIrqVector(48));
	IGROS_CHECK(irq_t::KEYBOARD == irqFromVector(33));
}


// Initialization: ICW1-ICW4 to both PICs, all lines masked
static void testInit() {
	hw::reset();
	irq::init();
	IGROS_CHECK((std::vector<std::uint32_t> {0x11}) == hw::writes(PIC_MASTER_CONTROL));
	IGROS_CHECK((std::vector<std::uint32_t> {0x11}) == hw::writes(PIC_SLAVE_CONTROL));
	IGROS_CHECK((std::vector<std::uint32_t> {0x20, 0x04, 0x01, 0xFF}) == hw::writes(PIC_MASTER_DATA));
	IGROS_CHECK((std::vector<std::uint32_t> {0x28, 0x02, 0x01, 0xFF}) == hw::writes(PIC_SLAVE_DATA));
	IGROS_CHECK(0xFFFF == irq::getMask());
}


// Masking single lines on both PICs
static void testMask() {
	hw::reset();
	irq::setMask();
	irq::unmask(irq_t::KEYBOARD);
	IGROS_CHECK(0xFFFD == irq::getMask());
	irq::unmask(irq_t::RTC);
	IGROS_CHECK(0xFEFD == irq::getMask());
	irq::mask(irq_t::KEYBOARD);
	IGROS_CHECK(0xFEFF == irq::getMask());
	// Lines past 15 are ignored
	irq::mask(static_cast<irq_t>(16));
	irq::unmask(static_cast<irq_t>(16));
	IGROS_CHECK(0xFEFF == irq::getMask());
	irq::setMask(0x1234);
	IGROS_CHECK(0x34 == hw::portValue[PIC_MASTER_DATA]);
	IGROS_CHECK(0x12 == hw::portValue[PIC_SLAVE_DATA]);
}


// End of interrupt: slave lines acknowledge both PICs
static void testEOI() {
	hw::reset();
	irq::eoi(irq_t::KEYBOARD);
	IGROS_CHECK((std::vector<std::uint32_t> {0x20}) == hw::writes(PIC_MASTER_CONTROL));
	IGROS_CHECK(hw::writes(PIC_SLAVE_CONTROL).empty());
	hw::reset();
	irq::eoi(irq_t::RTC);
	IGROS_CHECK((std::vector<std::uint32_t> {0x20}) == hw::writes(PIC_MASTER_CONTROL));
	IGROS_CHECK((std::vector<std::uint32_t> {0x20}) == hw::writes(PIC_SLAVE_CONTROL));
}


// Spurious IRQ 7 / 15: in-service register (OCW3 0x0B) bit 7 clear
static void testSpurious() {
	hw::reset();
	IGROS_CHECK(!irq::isSpurious(irq_t::KEYBOARD));
	IGROS_CHECK(hw::portLog.empty());
	// Real IRQ 7: in-service bit set
	hw::portReadHook = [](const std::uint16_t) -> std::uint32_t { return 0x80; };
	IGROS_CHECK(!irq::isSpurious(irq_t::LPT1));
	IGROS_CHECK((std::vector<std::uint32_t> {0x0B}) == hw::writes(PIC_MASTER_CONTROL));
	// Spurious IRQ 15
	hw::reset();
	hw::portReadHook = [](const std::uint16_t) -> std::uint32_t { return 0x00; };
	IGROS_CHECK(irq::isSpurious(irq_t::ATA_SECONDARY));
	IGROS_CHECK((std::vector<std::uint32_t> {0x0B}) == hw::writes(PIC_SLAVE_CONTROL));
}


// Interrupts flag and guard
static void testEnable() {
	hw::reset();
	irq::enable();
	IGROS_CHECK(hw::irqEnabled);
	{
		const irq::guard noInterrupts {};
		IGROS_CHECK(!hw::irqEnabled);
	}
	IGROS_CHECK(hw::irqEnabled);
	irq::disable();
	IGROS_CHECK(!hw::irqEnabled);
}


int main() {
	testVectors();
	testInit();
	testMask();
	testEOI();
	testSpurious();
	testEnable();
	return igros::test::result();
}

