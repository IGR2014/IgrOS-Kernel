////////////////////////////////////////////////////////////////
//
//	Programmable interval timer (8253/8254 PIT) driver tests
//
//	File:	test_pit.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <deque>
#include <vector>
// IgrOS-Kernel arch
#include <arch/x86/irq.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/pit.hpp>
// Tests
#include <tests/hw.hpp>


// Common ISR entry (called by assembly stubs)
extern "C" void isrHandler(const igros::x86::register_t* regs) noexcept;


using namespace igros::arch;
namespace hw = igros::test::hw;


// PIT ports
constexpr auto PIT_CHANNEL_0	{std::uint16_t {0x40}};
constexpr auto PIT_CONTROL	{std::uint16_t {0x43}};


// Channel 0 counter reads (low byte, high byte)
static auto counterBytes {std::deque<std::uint8_t> {}};

static auto pitRead(const std::uint16_t port) -> std::uint32_t {
	if ((PIT_CHANNEL_0 == port) && !counterBytes.empty()) {
		const auto value {counterBytes.front()};
		counterBytes.pop_front();
		return value;
	}
	return hw::portValue[port];
}

// Latched counter value for next read
static void latchCounter(const std::uint16_t counter) {
	counterBytes = {static_cast<std::uint8_t>(counter & 0xFF), static_cast<std::uint8_t>(counter >> 8)};
}

static void resetPIT() {
	hw::reset();
	hw::portReadHook = pitRead;
	counterBytes.clear();
}


// Frequency to divisor: mode 2 on channel 0, divisor low then high byte
static void testFrequency() {
	resetPIT();
	pitSetupFrequency(1000);
	IGROS_CHECK((std::vector<std::uint32_t> {0x34}) == hw::writes(PIT_CONTROL));
	// 1193181 / 1000 = 1193 = 0x04A9
	IGROS_CHECK((std::vector<std::uint32_t> {0xA9, 0x04}) == hw::writes(PIT_CHANNEL_0));
	IGROS_CHECK(hw::printed("PIT frequency:\t1000 Hz"));
	// Too low frequency is clamped to divisor limit
	resetPIT();
	pitSetupFrequency(1);
	IGROS_CHECK(hw::printed("PIT frequency:\t19 Hz"));
	const auto divisor {hw::writes(PIT_CHANNEL_0)};
	IGROS_CHECK((2 == divisor.size()) && ((divisor[1] << 8 | divisor[0]) <= 0xFFFF) && ((divisor[1] << 8 | divisor[0]) > 0xF000));
}


// Ticks: elapsed counts in current period (counter runs down from divisor)
static void testTicks() {
	resetPIT();
	pitSetupFrequency(1000);
	latchCounter(1193);
	IGROS_CHECK(0 == pitGetTicks());
	latchCounter(193);
	IGROS_CHECK(1000 == pitGetTicks());
	// Latch command before reading
	IGROS_CHECK((std::vector<std::uint32_t> {0x34, 0x00, 0x00}) == hw::writes(PIT_CONTROL));
}


// Setup: IRQ 0 unmasked, time printed once per second of ticks
static void testSetup() {
	resetPIT();
	hw::portValue[igros::x86::PIC_MASTER_DATA]	= 0xFF;
	hw::portValue[igros::x86::PIC_SLAVE_DATA]	= 0xFF;
	IGROS_CHECK(pitSetup());
	IGROS_CHECK(0xFE == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	IGROS_CHECK(hw::printed("PIT frequency:\t100 Hz"));
	auto regs {igros::x86::register_t {}};
	regs.number = static_cast<decltype(regs.number)>(igros::x86::irqVector(igros::x86::irq_t::PIT));
	hw::console.clear();
	for (auto i {0}; i < 99; ++i) {
		isrHandler(&regs);
	}
	IGROS_CHECK(hw::console.empty());
	// 100th tick: counter just reloaded (full period left)
	latchCounter(11931);
	isrHandler(&regs);
	IGROS_CHECK(hw::printed("[PIT]"));
	IGROS_CHECK(hw::printed("Time:\t00:00:00.999 (~1 sec.)"));
}


int main() {
	testFrequency();
	testTicks();
	testSetup();
	return igros::test::result();
}

