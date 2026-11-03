////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Interrupts low-level operations
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch x86
#include <arch/x86/io.hpp>
#include <arch/x86/isr.hpp>


#ifdef __cplusplus

extern "C" {

#endif // __cplusplus


	/// @brief IRQ handlers addresses, indexed by IRQ line (irq.s)
	extern const std::add_pointer_t<void() noexcept> irqHandlerTable[16];

	/// @brief Save flags and disable interrupts
	/// @return Saved flags register
	[[nodiscard]]
	auto irqSave() noexcept -> igros::igros_usize_t;
	/// @brief Restore flags (interrupts state)
	/// @param flags Flags register saved by irqSave()
	void irqRestore(const igros::igros_usize_t flags) noexcept;


#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus


// x86 namespace
namespace igros::x86 {


	// Master PIC ports
	constexpr auto PIC_MASTER_CONTROL {static_cast<port_t>(0x0020_u16)};		  ///< Master PIC command port
	constexpr auto PIC_MASTER_DATA {static_cast<port_t>(PIC_MASTER_CONTROL + 1_u16)}; ///< Master PIC data (mask) port
	// Slave PIC ports
	constexpr auto PIC_SLAVE_CONTROL {static_cast<port_t>(0x00A0_u16)};		///< Slave PIC command port
	constexpr auto PIC_SLAVE_DATA {static_cast<port_t>(PIC_SLAVE_CONTROL + 1_u16)}; ///< Slave PIC data (mask) port


	/// @brief Interrupts number enumeration (PIC lines)
	enum class irq_t : igros_dword_t {
		PIT	      = 0_u32,
		KEYBOARD      = 1_u32,
		CASCADE	      = 2_u32, ///< Slave PIC
		UART2	      = 3_u32,
		UART1	      = 4_u32,
		LPT2	      = 5_u32,
		FLOPPY	      = 6_u32,
		LPT1	      = 7_u32, ///< Master PIC spurious
		RTC	      = 8_u32,
		ACPI	      = 9_u32,
		FREE_10	      = 10_u32,
		FREE_11	      = 11_u32,
		MOUSE	      = 12_u32,
		FPU	      = 13_u32,
		ATA_PRIMARY   = 14_u32,
		ATA_SECONDARY = 15_u32, ///< Slave PIC spurious
	};

	/// @brief Number of PIC interrupt lines
	constexpr auto IRQ_LINES {16_usize};


	/// @brief Interrupt vector of IRQ line
	/// @param line IRQ line
	/// @return Interrupt vector
	[[nodiscard]]
	constexpr auto irqVector(const irq_t line) noexcept -> igros_usize_t {
		return IRQ_OFFSET + static_cast<igros_usize_t>(line);
	}

	/// @brief Check if interrupt vector belongs to IRQ line
	/// @param vector Interrupt vector
	/// @return true if vector is an IRQ vector
	[[nodiscard]]
	constexpr auto isIrqVector(const igros_usize_t vector) noexcept -> bool {
		return (vector >= IRQ_OFFSET) && (vector < (IRQ_OFFSET + IRQ_LINES));
	}

	/// @brief IRQ line of interrupt vector (vector must be IRQ vector)
	/// @param vector IRQ interrupt vector
	/// @return IRQ line
	[[nodiscard]]
	constexpr auto irqFromVector(const igros_usize_t vector) noexcept -> irq_t {
		return static_cast<irq_t>(vector - IRQ_OFFSET);
	}


	/// @brief IRQ structure
	class irq final {

	public:

		/// @brief Default c-tor
		irq() noexcept = default;
		/// @brief D-tor
		~irq() noexcept = default;

		/// @brief Copy c-tor
		irq(const irq &other) = delete;
		/// @brief Copy assignment
		auto operator=(const irq &other) -> irq & = delete;
		/// @brief Move c-tor
		irq(irq &&other) = delete;
		/// @brief Move assignment
		auto operator=(irq &&other) -> irq & = delete;

		/// @brief Init IRQ
		static void init() noexcept;

		/// @brief Enable interrupts
		static void enable() noexcept;
		/// @brief Disable interrupts
		static void disable() noexcept;

		/// @brief Mask interrupt
		/// @param number IRQ line (lines past 15 are ignored)
		static void mask(const irq_t number) noexcept;
		/// @brief Unmask interrupt
		/// @param number IRQ line (lines past 15 are ignored)
		static void unmask(const irq_t number) noexcept;

		/// @brief Set interrupts mask
		/// @param mask Mask of both PICs (bit set - line masked)
		static void setMask(const igros_word_t mask = 0xFFFF_u16) noexcept;
		/// @brief Get interrupts mask
		/// @return Mask of both PICs (bit set - line masked)
		[[nodiscard]]
		static auto getMask() noexcept -> igros_word_t;

		/// @brief Install IRQ handler
		template<irq_t N, isr_t HANDLE>
		static void install() noexcept;
		/// @brief Uninstall IRQ handler
		template<irq_t N>
		static void uninstall() noexcept;

		/// @brief Send EOI (IRQ done)
		/// @param number Handled IRQ line
		static void eoi(const irq_t number) noexcept;

		/// @brief Check if IRQ is spurious (only lines 7 and 15 can be)
		/// @param number IRQ line
		/// @return true if PIC raised the line without a request
		[[nodiscard]]
		static auto isSpurious(const irq_t number) noexcept -> bool;

		/// @brief Disable interrupts in scope (restores previous state on exit)
		class guard;
	};


	/// @brief Disable interrupts in scope (restores previous state on exit)
	class [[nodiscard("Unnamed guard re-enables IRQs immediately")]] irq::guard final {

		/// @brief Saved flags (interrupts are disabled while saving)
		igros_usize_t mFlags {::irqSave()};


	public:

		/// @brief Save flags and disable interrupts
		guard() noexcept = default;
		/// @brief Restore flags
		~guard() noexcept {
			::irqRestore(mFlags);
		}

		/// @brief Copy c-tor
		guard(const guard &other) = delete;
		/// @brief Copy assignment
		auto operator=(const guard &other) -> guard & = delete;
	};


	/// @brief Install handler
	template<irq_t N, isr_t HANDLE>
	inline void irq::install() noexcept {
		// Install ISR
		isrHandlerInstall(irqVector(N), HANDLE);
	}

	/// @brief Uninstall handler
	template<irq_t N>
	inline void irq::uninstall() noexcept {
		// Uninstall ISR
		isrHandlerUninstall(irqVector(N));
	}


} // namespace igros::x86

