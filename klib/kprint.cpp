////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.cpp
//	Date:	07 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kprint.hpp>
#include <klib/kstring.hpp>
#include <klib/kmath.hpp>


// Kernel library code zone
namespace klib {


	// Default temporary buffer for kitoa
	constexpr std::size_t	KITOA_BUFF_LEN		= 65;
	// Constant integer symbols values buffer
	static const sbyte_t	KITOA_CONST_BUFFER[]	= {"0123456789ABCDEF"};


	// Kernel integer to string template function
	inline static sbyte_t* kitoaHelper(sbyte_t* buffer, std::size_t size, dword_t value, const base radix) {

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN] {};
		// Setup counter to last - 1 position in temporary buffer
		dword_t	pos = KITOA_BUFF_LEN - 2;
		// Check if sign is negative and value should be represented
		// as decimal (binary, octal and hexidemical values have no sign)
		if ((value < 0)	&& (base::DEC == radix)) {
			// Write minus sign to buffer
			*buffer++ = '-';
			// Decrement size counter
			--size;
			// Made value positive
			value = -value;
		}

		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate digit index
			auto digitIndex		= value % byte_t(radix);
			// Calculate new value
			auto newValue		= value	/ byte_t(radix);
			// Save current digit to temporary buffer
			tempBuffer[pos--]	= KITOA_CONST_BUFFER[digitIndex];
			// Divide value by base to remove current digit
			value			= newValue;
		} while (value > 0);

		// Resulting string length
		std::size_t strLength = (KITOA_BUFF_LEN - pos);
		// Check size fit
		if (strLength > size) {
			return buffer;
		}
		// Revert temporary buffer we created from value to src buffer
		return kstrcpy(&tempBuffer[++pos], buffer, strLength);

	}


	// Kernel long integer to string template function 
	inline static sbyte_t* kltoaHelper(sbyte_t* buffer, std::size_t size, quad_t value, const base radix) {

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN] {};
		// Setup counter to last - 1 position in temporary buffer
		quad_t	pos = KITOA_BUFF_LEN - 2;
		// Check if sign is negative and value should be represented
		// as decimal (binary, octal and hexidemical values have no sign)
		if ((value < 0)	&& (base::DEC == radix)) {
			// Write minus sign to buffer
			*buffer++ = '-';
			// Decrement size counter
			--size;
			// Made value positive
			value = -value;
		}

		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate digit index
			udivmod_t divres	= kudivmod(value, dword_t(radix));
			// Save current digit to temporary buffer
			tempBuffer[pos--]	= KITOA_CONST_BUFFER[divres.reminder];
			// Divide value by base to remove current digit
			value			= divres.quotient;
		} while (value > 0);

		// Resulting string length
		std::size_t strLength = (KITOA_BUFF_LEN - pos);
		// Check size fit
		if (strLength > size) {
			return buffer;
		}
		// Revert temporary buffer we created from value to src buffer
		return kstrcpy(&tempBuffer[++pos], buffer, strLength);

	}


	// Kernel unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const dword_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}

	// Kernel large unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const quad_t value, const base radix) {
		return kltoaHelper(buffer, size, value, radix);
	}


	// Kernel size type to string function
	sbyte_t* kstoa(sbyte_t* buffer, std::size_t size, const std::size_t value, const base radix) {

#if defined(IGROS_ARCH_i386)

		return kitoaHelper(buffer, size, std::size_t(value), radix);

#else

		return kltoaHelper(buffer, size, std::size_t(value), radix);

#endif

	}


}	// namespace klib

