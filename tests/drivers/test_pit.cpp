////////////////////////////////////////////////////////////////
//
//	Programmable interval timer (8253/8254 PIT) driver tests
//
//	File:	test_pit.cpp
//	Date:	03 Nov 2026
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
#include <arch/x86/isr.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/pit.hpp>
// Tests
#include <tests/hw.hpp>


using namespace igros::arch;
namespace hw = igros::test::hw;


// PIT ports
constexpr auto PIT_CHANNEL_0 {std::uint16_t {0x40}};
constexpr auto PIT_CONTROL {std::uint16_t {0x43}};


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
	auto timer {pit::timer_t {}};
	timer.setFrequency(1000);
	IGROS_CHECK((std::vector<std::uint32_t> {0x34}) == hw::writes(PIT_CONTROL));
	// 1193181 / 1000 = 1193 = 0x04A9
	IGROS_CHECK((std::vector<std::uint32_t> {0xA9, 0x04}) == hw::writes(PIT_CHANNEL_0));
	IGROS_CHECK(hw::printed("PIT frequency:\t1000 Hz"));
	IGROS_CHECK(1000 == timer.frequency());
	// Too low frequency is clamped to divisor limit
	resetPIT();
	timer.setFrequency(1);
	IGROS_CHECK(hw::printed("PIT frequency:\t19 Hz"));
	const auto divisor {hw::writes(PIT_CHANNEL_0)};
	IGROS_CHECK((2 == divisor.size()) && ((divisor[1] << 8 | divisor[0]) <= 0xFFFF) && ((divisor[1] << 8 | divisor[0]) > 0xF000));
}


// Ticks: elapsed counts in current period (counter runs down from divisor)
static void testTicks() {
	resetPIT();
	auto timer {pit::timer_t {}};
	timer.setFrequency(1000);
	latchCounter(1193);
	IGROS_CHECK(0 == timer.ticks());
	latchCounter(193);
	IGROS_CHECK(1000 == timer.ticks());
	// Latch command before reading
	IGROS_CHECK((std::vector<std::uint32_t> {0x34, 0x00, 0x00}) == hw::writes(PIT_CONTROL));
	// Each period adds divisor ticks
	timer.interrupt();
	latchCounter(1193);
	IGROS_CHECK(1193 == timer.ticks());
}


// Timer interrupt vector
static void pitIRQ() {
	auto regs {igros::x86::register_t {}};
	regs.number = static_cast<decltype(regs.number)>(igros::x86::irqVector(igros::x86::irq_t::PIT));
	isrHandler(&regs);
}


// Setup: IRQ 0 unmasked and handled by initialized timer, time printed once per second of ticks
static void testSetup() {
	resetPIT();
	hw::portValue[igros::x86::PIC_MASTER_DATA] = 0xFF;
	hw::portValue[igros::x86::PIC_SLAVE_DATA]  = 0xFF;
	auto timer {pit::timer_t {}};
	IGROS_CHECK(timer.init());
	IGROS_CHECK(0xFE == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	IGROS_CHECK(hw::printed("PIT frequency:\t100 Hz"));
	hw::console.clear();
	for (auto i {0}; i < 99; ++i) {
		pitIRQ();
	}
	IGROS_CHECK(hw::console.empty());
	// 100th tick: counter just reloaded (full period left)
	latchCounter(11931);
	pitIRQ();
	IGROS_CHECK(hw::printed("[PIT]"));
	IGROS_CHECK(hw::printed("Time:\t00:00:00.999 (~1 sec.)"));
	timer.deinit();
}


// Control requests through driver interface
static void testControl() {
	resetPIT();
	auto	   timer {pit::timer_t {}};
	const auto driver {igros::drivers::make("timer", timer)};
	auto	   frequency {igros::igros_word_t {1000}};
	IGROS_CHECK(igros::drivers::ioctl(driver, pit::IOCTL::SET_FREQUENCY, &frequency));
	IGROS_CHECK((std::vector<std::uint32_t> {0xA9, 0x04}) == hw::writes(PIT_CHANNEL_0));
	// Real frequency: 1193181 / 1193 = 1000
	frequency = 0;
	IGROS_CHECK(igros::drivers::ioctl(driver, pit::IOCTL::GET_FREQUENCY, &frequency));
	IGROS_CHECK(1000 == frequency);
	latchCounter(193);
	auto ticks {igros::igros_quad_t {0}};
	IGROS_CHECK(igros::drivers::ioctl(driver, pit::IOCTL::GET_TICKS, &ticks));
	IGROS_CHECK(1000 == ticks);
	// No argument, unknown request
	IGROS_CHECK(!igros::drivers::ioctl(driver, pit::IOCTL::GET_TICKS));
	IGROS_CHECK(!driver.ioctl(driver.context, 0xFFu, &ticks));
	// System timer: lifecycle and controls, no data stream
	IGROS_CHECK(&pit::timer == pit::DRIVER.context);
	IGROS_CHECK((nullptr != pit::DRIVER.init) && (nullptr != pit::DRIVER.deinit) && (nullptr != pit::DRIVER.ioctl));
	IGROS_CHECK((nullptr == pit::DRIVER.read) && (nullptr == pit::DRIVER.write));
}


// Deinit masks IRQ 0 and removes handler
static void testDeinit() {
	resetPIT();
	hw::portValue[igros::x86::PIC_MASTER_DATA] = 0xFF;
	hw::portValue[igros::x86::PIC_SLAVE_DATA]  = 0xFF;
	IGROS_CHECK(igros::drivers::init(pit::DRIVER));
	igros::drivers::deinit(pit::DRIVER);
	IGROS_CHECK(0xFF == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	IGROS_CHECK_HALTS(pitIRQ());
	// Timer without frequency counts periods silently
	auto idle {pit::timer_t {}};
	hw::console.clear();
	idle.interrupt();
	IGROS_CHECK(hw::console.empty());
	// Stopping another timer doesn't detach active one from interrupts
	resetPIT();
	IGROS_CHECK(pit::timer.init());
	idle.deinit();
	IGROS_CHECK(igros::drivers::init(pit::DRIVER));
	hw::console.clear();
	pitIRQ();
	IGROS_CHECK(hw::console.empty());
	igros::drivers::deinit(pit::DRIVER);
}


auto main() -> int {
	testFrequency();
	testTicks();
	testSetup();
	testControl();
	testDeinit();
	return igros::test::result();
}

