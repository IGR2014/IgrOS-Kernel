////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Task switch registers data structure
///	@date		16 Dec 2022
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	/// @brief Read CS register
	/// @return CS value
	[[nodiscard]]
	auto	outCS() noexcept -> igros::igros_word_t;
	/// @brief Read DS register
	/// @return DS value
	[[nodiscard]]
	auto	outDS() noexcept -> igros::igros_word_t;
	/// @brief Read ES register
	/// @return ES value
	[[nodiscard]]
	auto	outES() noexcept -> igros::igros_word_t;
	/// @brief Read FS register
	/// @return FS value
	[[nodiscard]]
	auto	outFS() noexcept -> igros::igros_word_t;
	/// @brief Read GS register
	/// @return GS value
	[[nodiscard]]
	auto	outGS() noexcept -> igros::igros_word_t;
	/// @brief Read SS register
	/// @return SS value
	[[nodiscard]]
	auto	outSS() noexcept -> igros::igros_word_t;

	/// @brief Read EBP register
	/// @return EBP value
	[[nodiscard]]
	auto	outEBP() noexcept -> igros::igros_dword_t;
	/// @brief Read ESP register
	/// @return ESP value
	[[nodiscard]]
	auto	outESP() noexcept -> igros::igros_dword_t;

	/// @brief Read EFLAGS register
	/// @return EFLAGS value
	[[nodiscard]]
	auto	outEFLAGS() noexcept -> igros::igros_dword_t;

	/// @brief Read EAX register
	/// @return EAX value
	[[nodiscard]]
	auto	outEAX() noexcept -> igros::igros_dword_t;
	/// @brief Read EBX register
	/// @return EBX value
	[[nodiscard]]
	auto	outEBX() noexcept -> igros::igros_dword_t;
	/// @brief Read ECX register
	/// @return ECX value
	[[nodiscard]]
	auto	outECX() noexcept -> igros::igros_dword_t;
	/// @brief Read EDX register
	/// @return EDX value
	[[nodiscard]]
	auto	outEDX() noexcept -> igros::igros_dword_t;
	/// @brief Read EDI register
	/// @return EDI value
	[[nodiscard]]
	auto	outEDI() noexcept -> igros::igros_dword_t;
	/// @brief Read ESI register
	/// @return ESI value
	[[nodiscard]]
	auto	outESI() noexcept -> igros::igros_dword_t;

	/// @brief Read EIP register
	/// @return EIP value
	[[nodiscard]]
	auto	outEIP() noexcept -> igros::igros_dword_t;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 namespace
namespace igros::i386 {


#pragma pack(push, 1)

	/// @brief x86 task switch registers save
	struct register_t {

		// Segment registers pushed by us
		igros_dword_t	gs;		///< Extra3 segment register
		igros_dword_t	fs;		///< Extra2 segment register
		igros_dword_t	es;		///< Extra segment register
		igros_dword_t	ds;		///< Data segment register
		// "All" registers pushed by pushal instruction
		igros_dword_t	edi;		///< Destination index register
		igros_dword_t	esi;		///< Source index register
		igros_dword_t	ebp;		///< Stack base pointer register
		igros_dword_t	esp;		///< Stack pointer register
		igros_dword_t	ebx;		///< Base register
		igros_dword_t	edx;		///< Data register
		igros_dword_t	ecx;		///< Counter register
		igros_dword_t	eax;		///< Accumulator register
		// Exception number pushed by us
		igros_dword_t	number;		///< Exception number
		// Param pushed by processor or (0x00) by us
		igros_dword_t	param;		///< Exception param (or null if no param provided)
		// This pushed to stack by processor
		igros_dword_t	eip;		///< Instruction pointer register
		igros_dword_t	cs;		///< Code segment register
		igros_dword_t	eflags;		///< Flags register
		igros_dword_t	userEsp;	///< User stack pointer register
		igros_dword_t	ss;		///< Stack segment register

	};

#pragma pack(pop)


	/// @brief Print registers info
	void	dumpRegisters() noexcept;


}	// namespace igros::i386

