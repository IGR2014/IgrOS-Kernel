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

	} // namespace

	/// @brief Interrupt enable register bits
	constexpr auto SERIAL_IER_RX {0x01_u8};


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
		// IRQs enabled, RTS/DSR set
		serialRegWrite(port, SERIAL_REG::MCR, 0x0B_u8);
		// Set loopback mode, test the serial chip
		serialRegWrite(port, SERIAL_REG::MCR, 0x1E_u8);
		// Test port with 0xA5 byte
		serialRegWrite(port, SERIAL_REG::DATA, 0xA5_u8);

		// Check loopback (byte comes back once shifted out: one character time)
		if (!serialWaitStatus(port, SERIAL_LSR::DATA_READY) || (0xA5_u8 != serialRegRead(port, SERIAL_REG::DATA))) {
			// Could not setup serial port
			return false;
		}

		// Set normal mode (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
		serialRegWrite(port, SERIAL_REG::MCR, 0x0F_u8);

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


	/// @brief Serial #1 | #3 IRQ handler
	/// @param regs Registers saved on interrupt (unused)
	static void serialInterruptHandler1([[maybe_unused]] const register_t* const regs) noexcept {
		// Zero-initialized
		auto	   data {std::array<char, 128_usize> {}};
		// Read from COM1 (keep space for null terminator, reading also acknowledges interrupt)
		const auto read {serialRead(SERIAL_PORT::COM1, std::span {data}.first(data.size() - 1_usize))};
		// Debug data
		klib::kprintf(
			"IRQ #%u\t[UART1]\n"
			"Read:\t%05zu bytes = %s\n",
			static_cast<igros_dword_t>(irq_t::UART1),
			read,
			data.data()
		);
	}


	// Setup COM1 (115200 8N1) with receive interrupt
	[[nodiscard]]
	auto serialSetup() noexcept -> bool {

		// Init serial port
		if (!serialInit(SERIAL_PORT::COM1, BAUD_RATE::BAUD_115200, DATA_SIZE::CHAR_8, STOP_BITS::STOP_1, PARITY::NONE)) {
			// Fail
			return false;
		}

		// Install UART1 interrupt handler
		irq::install<irq_t::UART1, serialInterruptHandler1>();
		// Enable receive interrupt
		serialRegWrite(SERIAL_PORT::COM1, SERIAL_REG::IER, SERIAL_IER_RX);
		// Unmask UART1 interrupts
		irq::unmask(irq_t::UART1);

		// Success
		return true;
	}


} // namespace igros::arch

