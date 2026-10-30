////////////////////////////////////////////////////////////////
//
//	Serial port (UART 16550) driver tests
//
//	File:	test_serial.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstring>
#include <deque>
#include <span>
#include <string>
#include <vector>
// IgrOS-Kernel arch
#include <arch/x86/irq.hpp>
// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>
// Tests
#include <tests/hw.hpp>


// Common ISR entry (called by assembly stubs)
extern "C" void isrHandler(const igros::x86::register_t* regs) noexcept;


using namespace igros::arch;
namespace hw = igros::test::hw;


// COM1 registers
constexpr auto COM1_DATA	{std::uint16_t {0x3F8}};
constexpr auto COM1_IER		{std::uint16_t {0x3F9}};
constexpr auto COM1_FCR		{std::uint16_t {0x3FA}};
constexpr auto COM1_LCR		{std::uint16_t {0x3FB}};
constexpr auto COM1_MCR		{std::uint16_t {0x3FC}};
constexpr auto COM1_LSR		{std::uint16_t {0x3FD}};

// Line status bits
constexpr auto LSR_DATA_READY	{0x01u};
constexpr auto LSR_THR_EMPTY	{0x20u};
constexpr auto LSR_TX_IDLE	{0x40u};
// Modem control loopback bit
constexpr auto MCR_LOOPBACK	{0x10u};


// UART model: transmitter state and receive queue
static auto	txReady	{true};
static auto	rxQueue	{std::deque<char> {}};

static auto uart(const std::uint16_t port) -> std::uint32_t {
	switch (port) {
		case COM1_LSR:
			// Loopback: written byte is received at once
			return (txReady ? (LSR_THR_EMPTY | LSR_TX_IDLE) : 0u) |
				((rxQueue.empty() && (0u == (hw::portValue[COM1_MCR] & MCR_LOOPBACK))) ? 0u : LSR_DATA_READY);
		case COM1_DATA:
			if (!rxQueue.empty()) {
				const auto symbol {rxQueue.front()};
				rxQueue.pop_front();
				return static_cast<std::uint8_t>(symbol);
			}
			// Loopback: last written byte
			return hw::portValue[port];
		default:
			return hw::portValue[port];
	}
}

static void resetUART() {
	hw::reset();
	hw::portReadHook	= uart;
	txReady			= true;
	rxQueue.clear();
}

// Data bytes sent as string
[[nodiscard]]
static auto transmitted() -> std::string {
	auto text {std::string {}};
	for (const auto value : hw::writes(COM1_DATA)) {
		text += static_cast<char>(value);
	}
	return text;
}


// Initialization sequence: divisor, line format, FIFO, loopback test
static void testInit() {
	resetUART();
	IGROS_CHECK(serialInit(SERIAL_PORT::COM1, BAUD_RATE::BAUD_9600, DATA_SIZE::CHAR_8, STOP_BITS::STOP_1, PARITY::NONE));
	// Divisor 115200 / 9600 = 12 with DLAB set, then 8N1
	IGROS_CHECK((std::vector<std::uint32_t> {0x80, 0x03}) == hw::writes(COM1_LCR));
	IGROS_CHECK((std::vector<std::uint32_t> {0x00, 0x00}) == hw::writes(COM1_IER));
	// (followed by printed port info: kprintf outputs to serial too)
	const auto data {hw::writes(COM1_DATA)};
	IGROS_CHECK((data.size() > 2) && (0x0C == data[0]) && (0xA5 == data[1]));
	IGROS_CHECK((std::vector<std::uint32_t> {0xC7}) == hw::writes(COM1_FCR));
	// Loopback for test, then normal mode
	IGROS_CHECK((std::vector<std::uint32_t> {0x0B, 0x1E, 0x0F}) == hw::writes(COM1_MCR));
	IGROS_CHECK(hw::printed("Serial port 0x3f8:\t9600 8N1"));
}


// Faulty chip: loopback byte not returned
static void testInitFault() {
	resetUART();
	hw::portReadHook = [](const std::uint16_t) -> std::uint32_t { return 0xFF; };
	IGROS_CHECK(!serialInit(SERIAL_PORT::COM1, BAUD_RATE::BAUD_115200, DATA_SIZE::CHAR_8, STOP_BITS::STOP_1, PARITY::NONE));
	// Not switched to normal mode, nothing printed
	IGROS_CHECK((std::vector<std::uint32_t> {0x0B, 0x1E}) == hw::writes(COM1_MCR));
	IGROS_CHECK(hw::console.empty());
	IGROS_CHECK(!serialSetup());
}


// Transmitter never idle: port is not touched (pending output must not be clobbered)
static void testInitBusy() {
	resetUART();
	txReady = false;
	IGROS_CHECK(!serialInit(SERIAL_PORT::COM1, BAUD_RATE::BAUD_115200, DATA_SIZE::CHAR_8, STOP_BITS::STOP_1, PARITY::NONE));
	IGROS_CHECK(hw::writes(COM1_LCR).empty());
	IGROS_CHECK(hw::writes(COM1_MCR).empty());
}


// Looped back byte never arrives (data ready not set)
static void testInitNoLoopback() {
	resetUART();
	hw::portReadHook = [](const std::uint16_t port) -> std::uint32_t {
		return (COM1_LSR == port) ? (LSR_THR_EMPTY | LSR_TX_IDLE) : 0xA5u;
	};
	IGROS_CHECK(!serialInit(SERIAL_PORT::COM1, BAUD_RATE::BAUD_115200, DATA_SIZE::CHAR_8, STOP_BITS::STOP_1, PARITY::NONE));
	IGROS_CHECK((std::vector<std::uint32_t> {0x0B, 0x1E}) == hw::writes(COM1_MCR));
}


// Write: LF becomes CRLF, stops when transmitter never gets ready
static void testWrite() {
	resetUART();
	IGROS_CHECK(serialReadyWrite(SERIAL_PORT::COM1));
	IGROS_CHECK(6 == serialWrite("ab\ncd\n"));
	IGROS_CHECK("ab\r\ncd\r\n" == transmitted());
	resetUART();
	IGROS_CHECK(3 == serialWrite(std::string_view {"xyz"}));
	IGROS_CHECK("xyz" == transmitted());
	// Transmitter busy
	resetUART();
	txReady = false;
	IGROS_CHECK(!serialReadyWrite(SERIAL_PORT::COM1));
	IGROS_CHECK(0 == serialWrite("busy"));
	IGROS_CHECK(transmitted().empty());
}


// Read: only bytes available, never past buffer size
static void testRead() {
	resetUART();
	IGROS_CHECK(!serialReadyRead(SERIAL_PORT::COM1));
	char buffer[8] {};
	IGROS_CHECK(0 == serialRead(SERIAL_PORT::COM1, buffer));
	rxQueue = {'h', 'e', 'l', 'l', 'o'};
	IGROS_CHECK(serialReadyRead(SERIAL_PORT::COM1));
	IGROS_CHECK(3 == serialRead(SERIAL_PORT::COM1, std::span {buffer}.first(3)));
	IGROS_CHECK(0 == std::memcmp(buffer, "hel", 3));
	IGROS_CHECK(2 == serialRead(SERIAL_PORT::COM1, buffer));
	IGROS_CHECK(0 == std::memcmp(buffer, "lo", 2));
}


// Setup: receive interrupt enabled, IRQ 4 unmasked, handler prints received data
static void testSetup() {
	resetUART();
	hw::portValue[igros::x86::PIC_MASTER_DATA]	= 0xFF;
	hw::portValue[igros::x86::PIC_SLAVE_DATA]	= 0xFF;
	IGROS_CHECK(serialSetup());
	IGROS_CHECK(0x01 == hw::portValue[COM1_IER]);
	IGROS_CHECK(0xEF == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	// Receive interrupt
	hw::console.clear();
	rxQueue = {'h', 'i'};
	auto regs {igros::x86::register_t {}};
	regs.number = static_cast<decltype(regs.number)>(igros::x86::irqVector(igros::x86::irq_t::UART1));
	isrHandler(&regs);
	IGROS_CHECK(hw::printed("[UART1]"));
	IGROS_CHECK(hw::printed("00002 bytes = hi"));
	IGROS_CHECK(rxQueue.empty());
}


int main() {
	testInit();
	testInitFault();
	testInitBusy();
	testInitNoLoopback();
	testWrite();
	testRead();
	testSetup();
	return igros::test::result();
}

