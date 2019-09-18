////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.cpp
//	Date:	12 Sep 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kprint.hpp>
#include <klib/kstring.hpp>


// Kernel library code zone
namespace klib {


	// Default temporary buffer for kitoa
	constexpr std::size_t	KITOA_BUFF_LEN		= 65;
	// Constant integer symbols values buffer
	static const sbyte_t	KITOA_CONST_BUFFER[]	= {"0123456789ABCDEF"};


	// Kernel integer to string template function 
	template<typename T>
	inline sbyte_t* kitoaHelper(sbyte_t* buffer, std::size_t size, T value, const base radix) {

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN] {};
		// Setup counter to last - 1 position in temporary buffer
		T	pos = KITOA_BUFF_LEN - 2;
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

	// Kernel unsigned byte to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, byte_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}

	// Kernel signed byte to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, sbyte_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}


	// Kernel unsigned word to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, word_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}

	// Kernel signed word to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, sword_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}


	// Kernel unsigned double word to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, dword_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}

	// Kernel signed double word to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, sdword_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}


	// Kernel unsigned quad word to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, quad_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}

	// Kernel signed quad word to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, squad_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}


}	// namespace klib

