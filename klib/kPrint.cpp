////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kPrint.cpp
//	Date:	22 Dec 2023
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <algorithm>
#include <array>
#include <bit>
#include <cstdarg>
#include <limits>
#include <string_view>
#include <type_traits>
#include <utility>
// IgrOS-Kernel arch
#include <arch/irq.hpp>
// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kPrint.hpp>
#include <klib/kString.hpp>


// Kernel library code zone
namespace igros::klib {


	// Format implementation details
	namespace {


		// Argument size (length modifier)
		enum class argSize_t : igros_byte_t {
			BYTE,		// hh
			WORD,		// h
			DWORD,		// (none)
			QUAD,		// ll (l on 64-bit)
			SIZE		// z, t
		};

		// Conversion specification
		struct spec_t {
			igros_usize_t	width	{0_usize};		// Minimal field width
			argSize_t	size	{argSize_t::DWORD};	// Argument size
			bool		zero	{false};		// Pad numbers with '0' (after sign)
			bool		left	{false};		// Left-justify (pad with spaces after text)
		};


		// Output buffer writer: never writes past last position (reserved for terminator)
		class writer_t final {

			char*		mPos;		// Current position
			char* const	mEnd;		// Terminator position

		public:

			// Writer for buffer of given size (at least 1)
			writer_t(char* const buffer, const igros_usize_t size) noexcept :
				mPos	{buffer},
				mEnd	{buffer + size - 1_usize} {}

			// No space left
			[[nodiscard]]
			auto	isFull() const noexcept -> bool {
				return mPos >= mEnd;
			}

			// Character
			void	put(const char symbol) noexcept {
				if (!isFull()) [[likely]] {
					*mPos++ = symbol;
				}
			}

			// Text (clipped; no substr: it may throw)
			void	put(const std::string_view text) noexcept {
				mPos = std::ranges::copy_n(text.data(), static_cast<std::ptrdiff_t>(std::min(text.size(), space())), mPos).out;
			}

			// Repeated character (clipped)
			void	fill(const char symbol, const igros_usize_t count) noexcept {
				mPos = std::ranges::fill_n(mPos, static_cast<std::ptrdiff_t>(std::min(count, space())), symbol);
			}

			// Prefix (sign) and text padded to field width
			void	padded(const std::string_view prefix, const std::string_view text, const spec_t &spec) noexcept {
				const auto length	{prefix.size() + text.size()};
				const auto padding	{(spec.width > length) ? (spec.width - length) : 0_usize};
				if (spec.left) {
					put(prefix);
					put(text);
					fill(' ', padding);
				} else if (spec.zero) {
					put(prefix);
					fill('0', padding);
					put(text);
				} else {
					fill(' ', padding);
					put(prefix);
					put(text);
				}
			}

			// Terminate, return written length
			auto	finish(const char* const buffer) noexcept -> igros_usize_t {
				*mPos = '\0';
				return static_cast<igros_usize_t>(mPos - buffer);
			}

		private:

			// Space left
			[[nodiscard]]
			auto	space() const noexcept -> igros_usize_t {
				return static_cast<igros_usize_t>(mEnd - mPos);
			}

		};


		// Integer with sign (signed types, decimal only) and padding
		template<std::integral T>
		void putInteger(writer_t &out, const T value, const radix_t radix, const spec_t &spec, const bool upper = false) noexcept {
			using U = std::make_unsigned_t<T>;
			// Sign (compile time views: no strlen at run time)
			constexpr auto MINUS	{std::string_view {"-"}};
			constexpr auto NONE	{std::string_view {}};
			const auto isNegative	{std::cmp_less(value, static_cast<T>(0))};
			const auto magnitude	{isNegative ? static_cast<U>(static_cast<U>(0) - static_cast<U>(value)) : static_cast<U>(value)};
			auto digits		{std::array<char, std::numeric_limits<U>::digits> {}};
			out.padded(isNegative ? MINUS : NONE, kdigits(digits, magnitude, radix, upper), spec);
		}

		// Integer argument of given size: signed or unsigned
		void putArgument(writer_t &out, std::va_list &list, const radix_t radix, const spec_t &spec, const bool isSigned, const bool upper) noexcept {
			// Smaller than int arguments are promoted to int
			switch (spec.size) {
				case argSize_t::BYTE:
					isSigned ?
						putInteger(out, static_cast<igros_sbyte_t>(va_arg(list, igros_sdword_t)), radix, spec, upper) :
						putInteger(out, static_cast<igros_byte_t>(va_arg(list, igros_dword_t)), radix, spec, upper);
					break;
				case argSize_t::WORD:
					isSigned ?
						putInteger(out, static_cast<igros_sword_t>(va_arg(list, igros_sdword_t)), radix, spec, upper) :
						putInteger(out, static_cast<igros_word_t>(va_arg(list, igros_dword_t)), radix, spec, upper);
					break;
				case argSize_t::DWORD:
					isSigned ?
						putInteger(out, va_arg(list, igros_sdword_t), radix, spec, upper) :
						putInteger(out, va_arg(list, igros_dword_t), radix, spec, upper);
					break;
				case argSize_t::QUAD:
					isSigned ?
						putInteger(out, va_arg(list, igros_squad_t), radix, spec, upper) :
						putInteger(out, va_arg(list, igros_quad_t), radix, spec, upper);
					break;
				case argSize_t::SIZE:
					isSigned ?
						putInteger(out, va_arg(list, igros_ssize_t), radix, spec, upper) :
						putInteger(out, va_arg(list, igros_usize_t), radix, spec, upper);
					break;
			}
		}


	}	// namespace


	// Kernel vsnprintf function
	auto kvsnprintf(char* const buffer, const igros_usize_t size, const char* const format, std::va_list list) noexcept -> igros_usize_t {

		// Error checks
		if ((nullptr == buffer) || (0_usize == size) || (nullptr == format)) [[unlikely]] {
			return 0_usize;
		}

		// Output
		auto out {writer_t {buffer, size}};
		// Local copy of arguments (parameter may be decayed array, it can't be passed by reference)
		std::va_list args;
		va_copy(args, list);

		// Format string position (stops when output is full: remaining arguments are not needed)
		for (auto fmt {format}; ('\0' != *fmt) && !out.isFull(); ++fmt) {

			// Plain character
			if ('%' != *fmt) [[likely]] {
				out.put(*fmt);
				continue;
			}

			// Flags
			auto spec {spec_t {}};
			for (;; ++fmt) {
				if ('0' == fmt[1]) {
					spec.zero = true;
				} else if ('-' == fmt[1]) {
					spec.left = true;
				} else {
					break;
				}
			}

			// Field width (any number of digits)
			for (; ('0' <= fmt[1]) && ('9' >= fmt[1]); ++fmt) {
				spec.width = spec.width * 10_usize + static_cast<igros_usize_t>(fmt[1] - '0');
			}

			// Length modifier
			switch (fmt[1]) {
				case 'h':
					spec.size = ('h' == fmt[2]) ? argSize_t::BYTE : argSize_t::WORD;
					fmt += (argSize_t::BYTE == spec.size) ? 2 : 1;
					break;
				case 'l':
					// Long is 32 bits on i386, 64 bits on x86_64
					spec.size = (('l' == fmt[2]) || (sizeof(long) == sizeof(igros_quad_t))) ? argSize_t::QUAD : argSize_t::DWORD;
					fmt += ('l' == fmt[2]) ? 2 : 1;
					break;
				case 'z':
				case 't':
					spec.size = argSize_t::SIZE;
					++fmt;
					break;
				default:
					break;
			}

			// Unterminated placeholder at format end
			if ('\0' == fmt[1]) [[unlikely]] {
				break;
			}

			// Conversion
			switch (*++fmt) {
				// Placeholder symbol itself
				case '%':
					out.put('%');
					break;
				// Character (promoted to int)
				case 'c': {
					const auto symbol {static_cast<char>(va_arg(args, igros_sdword_t))};
					out.padded({}, std::string_view {&symbol, 1_usize}, spec);
				} break;
				// String (null prints nothing)
				case 's': {
					const auto* const str {va_arg(args, const char*)};
					out.padded({}, std::string_view {str, kstrlen(str)}, spec);
				} break;
				// Signed decimal
				case 'd':
				case 'i':
					putArgument(out, args, radix_t::DEC, spec, true, false);
					break;
				// Unsigned integers
				case 'u':
					putArgument(out, args, radix_t::DEC, spec, false, false);
					break;
				case 'b':
					putArgument(out, args, radix_t::BIN, spec, false, false);
					break;
				case 'o':
					putArgument(out, args, radix_t::OCT, spec, false, false);
					break;
				case 'x':
					putArgument(out, args, radix_t::HEX, spec, false, false);
					break;
				case 'X':
					putArgument(out, args, radix_t::HEX, spec, false, true);
					break;
				// Pointer: all hex digits
				case 'p':
					putInteger(out, std::bit_cast<igros_usize_t>(va_arg(args, igros_pointer_t)), radix_t::HEX, spec_t {.width = sizeof(igros_pointer_t) << 1, .zero = true});
					break;
				// Unknown conversion
				default:
					out.put('?');
					break;
			}

		}

		// Done with arguments
		va_end(args);
		// Insert null terminator
		return out.finish(buffer);

	}


	// Kernel snprintf function
	auto ksnprintf(char* const buffer, const igros_usize_t size, const char* const format, ...) noexcept -> igros_usize_t {
		// Kernel variadic argument list
		std::va_list list {};
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		const auto length {kvsnprintf(buffer, size, format, list)};
		// End variadic arguments list
		va_end(list);
		return length;
	}


	// Kernel printf function
	void kprintf(const char* const format, ...) noexcept {
		// Line end (compile time: no strlen at run time)
		constexpr auto NEWLINE {std::string_view {"\n"}};
		// Shared buffer and output must not be interrupted by IRQ handler printing too
		const arch::irq::guard noInterrupts {};
		// Text buffer
		constinit static auto buffer {std::array<char, 1024_usize> {}};
		// Kernel variadic argument list
		std::va_list list {};
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		const auto text {std::string_view {buffer.data(), kvsnprintf(buffer.data(), buffer.size(), format, list)}};
		// End variadic arguments list
		va_end(list);
		// Output to screen and serial
		arch::vmemWrite(text);
		arch::vmemWrite(NEWLINE);
		arch::serialWrite(text);
		arch::serialWrite(NEWLINE);
	}


}	// namespace igros::klib

