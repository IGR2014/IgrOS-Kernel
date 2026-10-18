////////////////////////////////////////////////////////////////
//
//	Fake hardware state for host unit tests (see stubs/cpu.cpp)
//
//	File:	hw.hpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <csetjmp>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <map>
#include <string>
#include <vector>
// Tests
#include <tests/test.hpp>


// Kernel symbol name (with platform label prefix) for inline assembly
#define IGROS_TEST_STR2(x)		#x
#define IGROS_TEST_STR(x)		IGROS_TEST_STR2(x)
#define IGROS_TEST_SYMBOL(name)		IGROS_TEST_STR(__USER_LABEL_PREFIX__) #name


// Fake hardware namespace
namespace igros::test::hw {


	// Port access record
	struct portAccess {
		std::uint16_t	port;		// Port address
		std::uint32_t	value;		// Value read or written
		bool		write;		// Write access
	};

	// All port accesses in order
	inline std::vector<portAccess>				portLog;
	// Port values: reads return last written (or preset) value, like registers
	inline std::map<std::uint16_t, std::uint32_t>		portValue;
	// Optional device model: overrides port reads
	inline std::function<std::uint32_t (std::uint16_t)>	portReadHook;

	// Interrupts flag
	inline bool			irqEnabled	{false};
	// Control registers
	inline std::uintptr_t		cr2		{0};
	inline std::uintptr_t		cr3		{0};
	// Invalidated TLB entries
	inline std::vector<const void*>	invalidated;

	// Console output (kprintf through VGA stub)
	inline std::string		console;

	// CPU halt jumps back to IGROS_CHECK_HALTS
	inline std::jmp_buf		haltJump;
	inline bool			haltArmed	{false};


	// Reset fake hardware
	inline void reset() {
		portLog.clear();
		portValue.clear();
		portReadHook	= nullptr;
		irqEnabled	= false;
		cr2		= 0;
		cr3		= 0;
		invalidated.clear();
		console.clear();
	}

	// Values written to port (in order)
	[[nodiscard]]
	inline auto writes(const std::uint16_t port) -> std::vector<std::uint32_t> {
		auto values {std::vector<std::uint32_t> {}};
		for (const auto &access : portLog) {
			if (access.write && (port == access.port)) {
				values.push_back(access.value);
			}
		}
		return values;
	}

	// Console contains text
	[[nodiscard]]
	inline auto printed(const char* const text) -> bool {
		return std::string::npos != console.find(text);
	}


}	// namespace igros::test::hw


// Check statement halts CPU (cpuHalt), continue after it
#define IGROS_CHECK_HALTS(statement)									\
	do {												\
		igros::test::hw::haltArmed = true;							\
		if (0 == setjmp(igros::test::hw::haltJump)) {						\
			statement;									\
			++igros::test::failures;							\
			std::printf("%s:%d: did not halt: %s\n", __FILE__, __LINE__, #statement);	\
		}											\
		igros::test::hw::haltArmed = false;							\
	} while (false)

