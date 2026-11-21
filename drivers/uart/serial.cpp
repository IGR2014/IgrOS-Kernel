////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		UART driver
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


// C++
#include <array>
#include <initializer_list>
// IgrOS-Kernel arch
#include <arch/io.hpp>
#include <arch/irq.hpp>
// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>
#include <klib/kPrint.hpp>
#include <klib/kString.hpp>


// Arch-dependent code zone
namespace igros::arch {


	/// @brief Anonymous namespace
	namespace {

		/// @brief Serial port registers (offsets from port base)
		enum class SERIAL_REG : igros_word_t {
			DATA	= 0x00_u16, ///< Data (DLAB = 0) / divisor low byte (DLAB = 1)
			IER	= 0x01_u16, ///< Interrupt enable (DLAB = 0) / divisor high byte (DLAB = 1)
			IIR	= 0x02_u16, ///< Interrupt identification (read) / FIFO control (write)
			LCR	= 0x03_u16, ///< Line control
			MCR	= 0x04_u16, ///< Modem control
			LSR	= 0x05_u16, ///< Line status
			MSR	= 0x06_u16, ///< Modem status
			SCRATCH = 0x07_u16, ///< Scratch
		};

		/// @brief Line status register bits
		enum class SERIAL_LSR : igros_byte_t {
			DATA_READY = 0x01_u8, ///< Received byte available
			THR_EMPTY  = 0x20_u8, ///< Transmitter holding register empty
			TX_IDLE	   = 0x40_u8, ///< Holding and shift registers empty
		};

		/// @brief Interrupt enable register bits
		enum class SERIAL_IER : igros_byte_t {
			RX_READY	= 0x01_u8,	///< Received data available
		};

		/// @brief Modem control register bits
		enum class SERIAL_MCR : igros_byte_t {
			DTR		= 0x01_u8,	///< Data terminal ready
			RTS		= 0x02_u8,	///< Request to send
			OUT1		= 0x04_u8,	///< Auxiliary output 1
			OUT2		= 0x08_u8,	///< Auxiliary output 2 (IRQ line enable on PC)
			LOOPBACK	= 0x10_u8,	///< Loopback test mode
		};

	} // namespace


	/// @brief Serial port register address
	/// @param port Serial port
	/// @param reg Register
	/// @return Register port
	[[nodiscard]]
	constexpr static auto serialRegister(const SERIAL_PORT port, const SERIAL_REG reg) noexcept -> port_t {
		return static_cast<port_t>(static_cast<igros_word_t>(port) + static_cast<igros_word_t>(reg));
	}

	/// @brief Write serial port register
	/// @param port Serial port
	/// @param reg Register
	/// @param value Written value
	static void serialRegWrite(const SERIAL_PORT port, const SERIAL_REG reg, const igros_byte_t value) noexcept {
		io::writePort8(serialRegister(port, reg), value);
	}

	/// @brief Read serial port register
	/// @param port Serial port
	/// @param reg Register
	/// @return Register value
	[[nodiscard]]
	static auto serialRegRead(const SERIAL_PORT port, const SERIAL_REG reg) noexcept -> igros_byte_t {
		return io::readPort8(serialRegister(port, reg));
	}

	/// @brief Read line status register
	/// @param port Serial port
	/// @return Line status bits
	[[nodiscard]]
	static auto serialStatus(const SERIAL_PORT port) noexcept -> klib::kFlags<SERIAL_LSR> {
		return klib::kFlags<SERIAL_LSR>::from(serialRegRead(port, SERIAL_REG::LSR));
	}

	/// @brief Wait until line status bit is set (bounded, so missing UART can't hang the kernel)
	/// @param port Serial port
	/// @param status Awaited status bit
	/// @return false if bit wasn't set in time
	[[nodiscard]]
	static auto serialWaitStatus(const SERIAL_PORT port, const SERIAL_LSR status) noexcept -> bool {
		for (auto spin {0_usize}; spin < 100000_usize; ++spin) {
			if (serialStatus(port).isSet(status)) [[likely]] {
				return true;
			}
		}
		return false;
	}


	// Initialize serial port
	[[nodiscard]]
	auto serialInit(const SERIAL_PORT port, const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept -> bool {

		// Calculate BAUD rate divisor
		const auto rate {static_cast<igros_word_t>(115200_u32 / static_cast<igros_dword_t>(baudRate))};
		// LCR value
		const auto lcr = static_cast<igros_byte_t>(
			(static_cast<igros_dword_t>(dataSize) & 0x03_u32) |
			((static_cast<igros_dword_t>(stopBits) & 0x01_u32) << 2) |
			((static_cast<igros_dword_t>(parity) & 0x03_u32) << 3)
		);

		// Let previous output finish (reprogramming would clobber byte being sent)
		if (!serialWaitStatus(port, SERIAL_LSR::TX_IDLE)) [[unlikely]] {
			return false;
		}

		// Disable SERIAL interrupts
		serialRegWrite(port, SERIAL_REG::IER, 0x00_u8);
		// Set DLAB to access BAUD rate divisor
		serialRegWrite(port, SERIAL_REG::LCR, 0x80_u8);
		// Write BAUD rate divisor low byte
		serialRegWrite(port, SERIAL_REG::DATA, static_cast<igros_byte_t>(rate & 0x00FF_u16));
		// Write BAUD rate divisor high byte
		serialRegWrite(port, SERIAL_REG::IER, static_cast<igros_byte_t>((static_cast<igros_dword_t>(rate) >> 8) & 0x00FF_u32));
		// Write LCR params (also clears DLAB)
		serialRegWrite(port, SERIAL_REG::LCR, lcr);
		// Enable FIFO, clear them with 14-byte threshold
		serialRegWrite(port, SERIAL_REG::IIR, 0xC7_u8);
		// IRQs enabled, DTR/RTS set
		serialRegWrite(port, SERIAL_REG::MCR, klib::make_kflags<SERIAL_MCR>(SERIAL_MCR::DTR, SERIAL_MCR::RTS, SERIAL_MCR::OUT2).value());
		// Set loopback mode, test the serial chip
		serialRegWrite(port, SERIAL_REG::MCR, klib::make_kflags<SERIAL_MCR>(SERIAL_MCR::RTS, SERIAL_MCR::OUT1, SERIAL_MCR::OUT2, SERIAL_MCR::LOOPBACK).value());
		// Test port with 0xA5 byte
		serialRegWrite(port, SERIAL_REG::DATA, 0xA5_u8);

		// Check loopback (byte comes back once shifted out: one character time)
		if (!serialWaitStatus(port, SERIAL_LSR::DATA_READY) || (0xA5_u8 != serialRegRead(port, SERIAL_REG::DATA))) {
			// Could not setup serial port
			return false;
		}

		// Set normal mode (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
		serialRegWrite(port, SERIAL_REG::MCR, klib::make_kflags<SERIAL_MCR>(SERIAL_MCR::DTR, SERIAL_MCR::RTS, SERIAL_MCR::OUT1, SERIAL_MCR::OUT2).value());

		// Debug
		klib::kprintf(
			"Serial port 0x%x:\t%d %d%c%d",
			static_cast<igros_dword_t>(port),
			static_cast<igros_dword_t>(baudRate),
			static_cast<igros_dword_t>(dataSize) + 5_u32,
			(parity == PARITY::NONE) ? 'N' : '?',
			(static_cast<igros_dword_t>(stopBits) & 0x01_u32) + 1_u32
		);

		// Success
		return true;
	}


	// Is write ready? (LSR bit 5 - transmitter holding register empty)
	[[nodiscard]]
	auto serialReadyWrite(const SERIAL_PORT port) noexcept -> bool {
		return serialStatus(port).isSet(SERIAL_LSR::THR_EMPTY);
	}

	// Is read ready? (LSR bit 0 - data ready)
	[[nodiscard]]
	auto serialReadyRead(const SERIAL_PORT port) noexcept -> bool {
		return serialStatus(port).isSet(SERIAL_LSR::DATA_READY);
	}

	/// @brief Wait until write ready
	/// @param port Serial port
	/// @return false if port wasn't ready in time
	[[nodiscard]]
	static auto serialWaitWrite(const SERIAL_PORT port) noexcept -> bool {
		return serialWaitStatus(port, SERIAL_LSR::THR_EMPTY);
	}


	/// @brief Wait for transmitter and send one byte
	/// @param port Serial port
	/// @param value Byte
	/// @return false if port wasn't ready in time
	[[nodiscard]]
	static auto serialPut(const SERIAL_PORT port, const igros_byte_t value) noexcept -> bool {
		if (!serialWaitWrite(port)) [[unlikely]] {
			return false;
		}
		serialRegWrite(port, SERIAL_REG::DATA, value);
		return true;
	}


	/// @brief Send one symbol (new line is sent as CR LF)
	/// @param port Serial port
	/// @param symbol Character
	/// @return false if port wasn't ready in time
	[[nodiscard]]
	static auto serialPutSymbol(const SERIAL_PORT port, const char symbol) noexcept -> bool {
		if (('\n' == symbol) && !serialPut(port, '\r')) [[unlikely]] {
			return false;
		}
		return serialPut(port, static_cast<igros_byte_t>(symbol));
	}

	/// @brief Receive one symbol (only if available)
	/// @param port Serial port
	/// @param symbol Received character
	/// @return true if a character was received
	[[nodiscard]]
	static auto serialGetSymbol(const SERIAL_PORT port, char &symbol) noexcept -> bool {
		if (!serialReadyRead(port)) {
			return false;
		}
		symbol = static_cast<char>(serialRegRead(port, SERIAL_REG::DATA));
		return true;
	}


	// Serial write
	[[maybe_unused]]
	auto serialWrite(const SERIAL_PORT port, const std::string_view text) noexcept -> igros_usize_t {
		// Write data one-by-one until transmitter fails
		auto written {0_usize};
		while ((written < text.size()) && serialPutSymbol(port, text[written])) {
			++written;
		}
		// Return written size
		return written;
	}

	// Serial read
	[[maybe_unused]]
	auto serialRead(const SERIAL_PORT port, const std::span<char> buffer) noexcept -> igros_usize_t {
		// Read data one-by-one while available (never past buffer size)
		auto read {0_usize};
		while ((read < buffer.size()) && serialGetSymbol(port, buffer[read])) {
			++read;
		}
		// Return read size
		return read;
	}


	// Serial write to COM1 (kernel console)
	[[maybe_unused]]
	auto serialWrite(const std::string_view text) noexcept -> igros_usize_t {
		return serialWrite(SERIAL_PORT::COM1, text);
	}

	// Serial write to COM1 (kernel console)
	[[maybe_unused]]
	auto serialWrite(const char* const src) noexcept -> igros_usize_t {
		return serialWrite(SERIAL_PORT::COM1, klib::kstrview(src));
	}


	/// @brief Standard ports count
	constexpr auto SERIAL_PORTS_COUNT {4_usize};

	/// @brief Ports handling receive interrupt (index is COM number - 1)
	constinit static auto serialActive {std::array<const serial::uart_t*, SERIAL_PORTS_COUNT> {}};

	/// @brief Standard port index
	/// @param port Serial port
	/// @return Index in serialActive
	[[nodiscard]]
	constexpr static auto serialIndex(const SERIAL_PORT port) noexcept -> igros_usize_t {
		switch (port) {
			case SERIAL_PORT::COM2:
				return 1_usize;
			case SERIAL_PORT::COM3:
				return 2_usize;
			case SERIAL_PORT::COM4:
				return 3_usize;
			default:
				return 0_usize;
		}
	}

	/// @brief Port sharing IRQ line (COM1 and COM3, COM2 and COM4)
	/// @param index Port index
	/// @return Index of other port on same line
	[[nodiscard]]
	constexpr static auto serialPartner(const igros_usize_t index) noexcept -> igros_usize_t {
		return index ^ 2_usize;
	}


	/// @brief Serial IRQ handler (both ports on line are checked)
	/// @tparam LINE IRQ line (UART1 - COM1/COM3, UART2 - COM2/COM4)
	/// @param regs Registers saved on interrupt (unused)
	template<irq_t LINE>
	static void serialInterruptHandler([[maybe_unused]] const register_t* const regs) noexcept {
		constexpr auto FIRST {(irq_t::UART1 == LINE) ? 0_usize : 1_usize};
		for (const auto index : {FIRST, serialPartner(FIRST)}) {
			if (const auto* const port {serialActive[index]}; nullptr != port) {
				port->interrupt();
			}
		}
	}

	/// @brief Install serial IRQ handler and unmask line
	/// @tparam LINE IRQ line
	template<irq_t LINE>
	static void serialIRQEnable() noexcept {
		irq::install<LINE, serialInterruptHandler<LINE>>();
		irq::unmask(LINE);
	}

	/// @brief Mask line and uninstall serial IRQ handler
	/// @tparam LINE IRQ line
	template<irq_t LINE>
	static void serialIRQDisable() noexcept {
		irq::mask(LINE);
		irq::uninstall<LINE>();
	}


	// Receive interrupt
	void serial::uart_t::interrupt() const noexcept {
		// Zero-initialized
		auto	   data {std::array<char, 128_usize> {}};
		// Read port (keep space for null terminator, reading also acknowledges interrupt)
		const auto count {serialRead(mConfig.port, std::span {data}.first(data.size() - 1_usize))};
		// Debug data (port without data didn't raise interrupt)
		if (0_usize != count) {
			const auto index {serialIndex(mConfig.port)};
			klib::kprintf(
				"IRQ #%u\t[COM%u]\n"
				"Read:\t%05zu bytes = %s\n",
				static_cast<igros_dword_t>((0_usize == (index & 1_usize)) ? irq_t::UART1 : irq_t::UART2),
				static_cast<igros_dword_t>(index + 1_usize),
				count,
				data.data()
			);
		}
	}


	// Setup port with receive interrupt
	[[nodiscard]]
	auto serial::uart_t::init() const noexcept -> bool {

		// Init serial port
		if (!serialInit(mConfig.port, mConfig.baudRate, mConfig.dataSize, mConfig.stopBits, mConfig.parity)) {
			// Fail
			return false;
		}

		// This port handles its interrupts
		const auto index {serialIndex(mConfig.port)};
		serialActive[index] = this;
		// Install interrupt handler of port line
		if (0_usize == (index & 1_usize)) {
			serialIRQEnable<irq_t::UART1>();
		} else {
			serialIRQEnable<irq_t::UART2>();
		}
		// Enable receive interrupt
		serialRegWrite(mConfig.port, SERIAL_REG::IER, klib::make_kflags<SERIAL_IER>(SERIAL_IER::RX_READY).value());

		// Success
		return true;
	}

	// Stop receive interrupt
	void serial::uart_t::deinit() const noexcept {
		// Disable receive interrupt
		serialRegWrite(mConfig.port, SERIAL_REG::IER, 0x00_u8);
		// Port no longer handles interrupts
		const auto index {serialIndex(mConfig.port)};
		if (this == serialActive[index]) {
			serialActive[index] = nullptr;
		}
		// Release line once other port on it is stopped too
		if (nullptr != serialActive[serialPartner(index)]) {
			return;
		}
		if (0_usize == (index & 1_usize)) {
			serialIRQDisable<irq_t::UART1>();
		} else {
			serialIRQDisable<irq_t::UART2>();
		}
	}

	// Read available bytes
	[[nodiscard]]
	auto serial::uart_t::read(const std::span<std::byte> buffer) const noexcept -> igros_usize_t { // flawfinder: ignore (device read, bounded by span size)
		// Read data one-by-one while available (never past buffer size)
		auto count {0_usize};
		auto symbol {'\0'};
		while ((count < buffer.size()) && serialGetSymbol(mConfig.port, symbol)) {
			buffer[count] = static_cast<std::byte>(symbol);
			++count;
		}
		// Return read size
		return count;
	}

	// Write bytes
	[[nodiscard]]
	auto serial::uart_t::write(const std::span<const std::byte> data) const noexcept -> igros_usize_t {
		// Write data one-by-one until transmitter fails
		auto written {0_usize};
		while ((written < data.size()) && serialPutSymbol(mConfig.port, static_cast<char>(data[written]))) {
			++written;
		}
		// Return written size
		return written;
	}


} // namespace igros::arch

