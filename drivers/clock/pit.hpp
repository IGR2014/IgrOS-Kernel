////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Programmable interrupt timer
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel drivers
#include <drivers/driver.hpp>


// Arch-dependent code zone
namespace igros::arch {


	/// @brief PIT frequency (1.193181(3) MHz)
	constexpr auto PIT_MAIN_FREQUENCY = 1193181_u32;
	/// @brief Default pit frequency (100 Hz)
	constexpr auto PIT_DEFAULT_FREQUENCY = 100_u32;


	/// @brief PIT driver
	namespace pit {

		/// @brief PIT control requests
		enum class IOCTL : igros_dword_t {
			SET_FREQUENCY = 0x01_u32, ///< Set interrupt frequency (arg: igros_word_t*, Hz)
			GET_FREQUENCY = 0x02_u32, ///< Get real interrupt frequency (arg: igros_word_t*, Hz)
			GET_TICKS     = 0x03_u32, ///< Get expired ticks (arg: igros_quad_t*, PIT_MAIN_FREQUENCY ticks)
		};

		/// @brief Programmable interval timer (channel 0, system timer)
		class timer_t final {

		public:

			/// @brief Setup timer (default frequency, IRQ 0 is handled by this timer)
			/// @return true if timer is set up
			[[nodiscard]]
			auto init() noexcept -> bool;
			/// @brief Stop timer interrupts
			void deinit() noexcept;
			/// @brief Timer control
			/// @param request Request (IOCTL)
			/// @param arg Request argument
			/// @return true if request succeeded
			[[nodiscard]]
			auto ioctl(const igros_dword_t request, const igros_pointer_t arg) noexcept -> bool;

			/// @brief Setup timer frequency
			/// @param frequency Interrupt frequency in Hz (clamped to divisor limit)
			void setFrequency(const igros_word_t frequency) noexcept;
			/// @brief Get real timer frequency
			/// @return Interrupt frequency in Hz
			[[nodiscard]]
			auto frequency() const noexcept -> igros_word_t;
			/// @brief Get expired ticks
			/// @return PIT_MAIN_FREQUENCY ticks since setup
			[[nodiscard]]
			auto ticks() const noexcept -> igros_quad_t;

			/// @brief Timer interrupt (counts period, prints time once per second)
			void interrupt() noexcept;

		private:

			igros_usize_t mPeriods {0_usize}; ///< Interrupts count
			igros_word_t  mFrequency {0_u16}; ///< Current frequency
			igros_word_t  mDivisor {1_u16};	  ///< Current divisor
		};

		/// @brief System timer
		inline constinit timer_t timer {};

		/// @brief System timer driver description
		inline constexpr drivers::driver_t DRIVER {drivers::make("PIT", timer)};

	} // namespace pit


} // namespace igros::arch
