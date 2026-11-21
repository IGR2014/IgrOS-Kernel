////////////////////////////////////////////////////////////////
//
//	Serial port (UART 16550) driver tests
//
//	File:	test_serial.cpp
//	Date:	03 Nov 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <cstddef>
#include <cstring>
#include <deque>
#include <span>
#include <string>
#include <vector>
// IgrOS-Kernel arch
#include <arch/x86/irq.hpp>
#include <arch/x86/isr.hpp>
// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>
// Tests
#include <tests/hw.hpp>


using namespace igros::arch;
namespace hw = igros::test::hw;


// COM1 registers
constexpr auto COM1_DATA {std::uint16_t {0x3F8}};
constexpr auto COM1_IER {std::uint16_t {0x3F9}};
constexpr auto COM1_FCR {std::uint16_t {0x3FA}};
constexpr auto COM1_LCR {std::uint16_t {0x3FB}};
constexpr auto COM1_MCR {std::uint16_t {0x3FC}};
constexpr auto COM1_LSR {std::uint16_t {0x3FD}};

// Line status bits
constexpr auto LSR_DATA_READY {0x01u};
constexpr auto LSR_THR_EMPTY {0x20u};
constexpr auto LSR_TX_IDLE {0x40u};
// Modem control loopback bit
constexpr auto MCR_LOOPBACK {0x10u};


// UART model: transmitter state and receive queue
static auto txReady {true};
static auto rxQueue {std::deque<char> {}};

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
	hw::portReadHook = uart;
	txReady		 = true;
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
	IGROS_CHECK(!serial::com1.init());
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


// Interrupt on IRQ line
static void uartIRQ(const igros::x86::irq_t line) {
	auto regs {igros::x86::register_t {}};
	regs.number = static_cast<decltype(regs.number)>(igros::x86::irqVector(line));
	isrHandler(&regs);
}

// Port without model: idle transmitter, loopback through data register
static void fakePort(const std::uint16_t base, const bool dataReady) {
	hw::portValue[base + 5] = LSR_THR_EMPTY | LSR_TX_IDLE | (dataReady ? LSR_DATA_READY : 0u);
}

// Both PICs fully masked
static void maskAll() {
	hw::portValue[igros::x86::PIC_MASTER_DATA] = 0xFF;
	hw::portValue[igros::x86::PIC_SLAVE_DATA]  = 0xFF;
}


// Setup: receive interrupt enabled, IRQ 4 unmasked, handler prints received data
static void testSetup() {
	resetUART();
	maskAll();
	IGROS_CHECK(igros::drivers::init(serial::COM1));
	IGROS_CHECK(0x01 == hw::portValue[COM1_IER]);
	IGROS_CHECK(0xEF == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	// Receive interrupt
	hw::console.clear();
	rxQueue = {'h', 'i'};
	uartIRQ(igros::x86::irq_t::UART1);
	IGROS_CHECK(hw::printed("IRQ #4\t[COM1]"));
	IGROS_CHECK(hw::printed("00002 bytes = hi"));
	IGROS_CHECK(rxQueue.empty());
	// Interrupt without data prints nothing
	hw::console.clear();
	uartIRQ(igros::x86::irq_t::UART1);
	IGROS_CHECK(hw::console.empty());
	igros::drivers::deinit(serial::COM1);
}


// Driver interface: port bytes in and out, deinit stops receive interrupt
static void testDriver() {
	resetUART();
	IGROS_CHECK(5 == igros::drivers::write(serial::COM1, "ab\ncd"));
	IGROS_CHECK("ab\r\ncd" == transmitted());
	auto buffer {std::array<std::byte, 4> {}};
	IGROS_CHECK(0 == igros::drivers::read(serial::COM1, buffer));
	rxQueue = {'o', 'k'};
	IGROS_CHECK(2 == igros::drivers::read(serial::COM1, buffer));
	IGROS_CHECK((std::byte {'o'} == buffer[0]) && (std::byte {'k'} == buffer[1]));
	// Instances: names, port contexts, no control requests
	IGROS_CHECK((&serial::com1 == serial::COM1.context) && (&serial::com4 == serial::COM4.context));
	IGROS_CHECK(SERIAL_PORT::COM3 == serial::com3.config().port);
	IGROS_CHECK(std::string {"COM3"} == serial::COM3.name);
	IGROS_CHECK(nullptr == serial::COM1.ioctl);
	// Port object with own configuration: 9600 baud divisor 12, 7 data bits, even parity
	resetUART();
	auto slow {serial::uart_t {{SERIAL_PORT::COM1, BAUD_RATE::BAUD_9600, DATA_SIZE::CHAR_7, STOP_BITS::STOP_1, PARITY::EVEN}}};
	IGROS_CHECK(slow.init());
	IGROS_CHECK(0x0C == hw::writes(COM1_DATA).front());
	IGROS_CHECK((std::vector<std::uint32_t> {0x80, 0x12}) == hw::writes(COM1_LCR));
	slow.deinit();
	// Deinit after setup: receive interrupt disabled, IRQ 4 masked, handler removed
	resetUART();
	maskAll();
	IGROS_CHECK(igros::drivers::init(serial::COM1));
	igros::drivers::deinit(serial::COM1);
	IGROS_CHECK(0x00 == hw::portValue[COM1_IER]);
	IGROS_CHECK(0xFF == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	IGROS_CHECK_HALTS(uartIRQ(igros::x86::irq_t::UART1));
}


// COM1 and COM3 share IRQ 4: line is kept until both are stopped, COM2 uses IRQ 3
static void testSharedLine() {
	constexpr auto COM2 {std::uint16_t {0x2F8}};
	constexpr auto COM3 {std::uint16_t {0x3E8}};
	resetUART();
	maskAll();
	fakePort(COM3, true);
	IGROS_CHECK(igros::drivers::init(serial::COM1));
	IGROS_CHECK(igros::drivers::init(serial::COM3));
	IGROS_CHECK(0x01 == hw::portValue[COM3 + 1]);
	IGROS_CHECK(0xEF == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	// COM3 has no data now, COM1 data is reported
	fakePort(COM3, false);
	hw::console.clear();
	rxQueue = {'h', 'i'};
	uartIRQ(igros::x86::irq_t::UART1);
	IGROS_CHECK(hw::printed("[COM1]") && !hw::printed("[COM3]"));
	// COM1 stopped: line still serves COM3
	igros::drivers::deinit(serial::COM1);
	IGROS_CHECK(0xEF == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	hw::console.clear();
	uartIRQ(igros::x86::irq_t::UART1);
	IGROS_CHECK(hw::console.empty());
	// Last port on line stopped
	igros::drivers::deinit(serial::COM3);
	IGROS_CHECK(0xFF == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	IGROS_CHECK_HALTS(uartIRQ(igros::x86::irq_t::UART1));
	// COM2 on IRQ 3
	fakePort(COM2, true);
	IGROS_CHECK(igros::drivers::init(serial::COM2));
	IGROS_CHECK(0xF7 == hw::portValue[igros::x86::PIC_MASTER_DATA]);
	igros::drivers::deinit(serial::COM2);
	IGROS_CHECK(0xFF == hw::portValue[igros::x86::PIC_MASTER_DATA]);
}


auto main() -> int {
	testInit();
	testInitFault();
	testInitBusy();
	testInitNoLoopback();
	testWrite();
	testRead();
	testSetup();
	testDriver();
	testSharedLine();
	return igros::test::result();
}

