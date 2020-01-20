////////////////////////////////////////////////////////////////
//
//	ELF header info
//
//	File:	elf.hpp
//	Date:	17 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_ELF_HPP
#define IGROS_ARCH_ELF_HPP


#include <cstdint>

#include <arch/types.hpp>


// System code zone
namespace sys {


        // ELF identity magic
        constexpr static byte_t ELF_MAGIC_0		= 0x7F;
	constexpr static byte_t ELF_MAGIC_1		= 'E';
	constexpr static byte_t ELF_MAGIC_2		= 'L';
	constexpr static byte_t ELF_MAGIC_3		= 'F';


	// ELF class type
	enum class ELF_CLASS : byte_t {
		NONE		= 0x00,				// Unknown
		CLASS32		= 0x01,				// 32-bit
		CLASS64		= 0x02				// 64-bit
	};

	// ELF data type
	enum class ELF_DATA : byte_t {
		NONE		= 0x00,				// Unknown
		LSB		= 0x01,				// LSB order
		MSB		= 0x02				// MSB order
	};

	// ELF version
	enum class ELF_VERSION : dword_t {
		NONE		= 0,				// Unknown version
		CURRENT		= 1				// Current version
	};

	// ELF OS ABI
	enum class ELF_ABI {
		SYSV		= 0x00,				// System-V
		HP_UX		= 0x01,				// HP-UX
		NETBSD		= 0x02,				// NetBSD
		LINUX		= 0x03,				// Linux
		HURD		= 0x04,				// GNU Hurd
		SOLARIS		= 0x06,				// Solaris
		AIX		= 0x07,				// AIX
		IRIS		= 0x08,				// IRIS
		FREEBSD		= 0x09,				// FreeBSD
		TRU64		= 0x0A,				// Tru64
		NOVELL		= 0x0B,				// Novell Modesto
		OPENBSD		= 0x0C,				// OpenBSD
		VMS		= 0x0D,				// OpenVMS
		NONSTOP		= 0x0E,				// NonStop Kernel
		AROS		= 0x0F,				// AROS
		FENIX		= 0x10,				// Fenix OS
		CLOUD		= 0x11				// Cloud ABI
	};


	// ELF identity class
	struct elfIdentity_t {
		byte_t		magic[4];			// ELF magic
		ELF_CLASS	classType;			// ELF class
		ELF_DATA	dataType;			// ELF data order
		ELF_VERSION	version;			// ELF version
		ELF_ABI		abiType;			// ELS OS ABI
		byte_t		abiVersion;			// ELS OS ABI version
		byte_t		reserved[7];			// Unused
	};


	// ELF type
	enum class ELF_TYPE : word_t {
		NONE		= 0x0000,			// Unknown
		REL		= 0x0001,			// Relocatable file
		EXEC		= 0x0002,			// Executable file
		DYN		= 0x0003,			// Shared object file
		CORE		= 0x0004,			// Core file
		LOOS		= 0xFE00,			// OS-specific
		HIOS		= 0xFEFF,			// --- // ---
		LOPROC		= 0xFF00,			// Processor-specific
		HIPROC		= 0xFFFF			// --- // ---
	};


	// ELF machine type
	enum class ELF_MACHINE : word_t {
		NONE		= 0x00,				// Unknown
		M32		= 0x01,				// AT&T WE 32100
		SPARC		= 0x02,				// SPARC
		I386		= 0x03,				// Intel 80386
		M68K		= 0x04,				// Motorola 68000
		M88K		= 0x05,				// Motorola 88000
		I8086		= 0x07,				// Intel 80860
		MIPS		= 0x08,				// MIPS RS3000
		POWERPC		= 0x14,				// PowerPC
		S390		= 0x16,				// IBM s390
		ARM		= 0x28,				// ARM
		SUPERH		= 0x2A,				// SuperH
		IA_64		= 0x32,				// Intel 64
		X86_64		= 0x3E,				// AMD/Intel x86-64
		AARCH64		= 0xB7,				// AArch64
		RISC_V		= 0xF3				// RISC-V
	};

	// ELF file header
        struct elfHeader_t {
		elfIdentity_t	identity;			// ELF Header identity
		ELF_TYPE	type;				// ELF Header type
                ELF_MACHINE	machine;			// ELF Header machine type
                ELF_VERSION	version;			// ELF Header version
                pointer_t	entry;				// ELF Header entry address
		std::size_t	programHeader;			// ELF Header program header
		std::size_t	sectionHeader;			// ELF Header section header
		dword_t		flags;				// ELF Header flags
		word_t		headerSize;			// ELF Header size
		word_t		programHeaderSize;		// ELF Header program header size
		word_t		programHeaderNum;		// ELF Header program header count
		word_t		sectionHeaderSize;		// ELF Header section header size
		word_t		sectionHeaderNum;		// ELF Header section header count
		word_t		sectionNamesIndex;		// ELF Header names section index
        };


	// ELF program type
	enum class ELF_PROGRAM_TYPE : dword_t {
		NONE		= 0x00000000,			// Unused
		LOAD		= 0x00000001,			// Loadable segment
		DYNAMIC		= 0x00000002,			// Dynamic linking info
		INTERP		= 0x00000003,			// Interpreter info
		NOTE		= 0x00000004,			// Auxiliary info
		SHLIB		= 0x00000005,			// Reserved
		PHDR		= 0x00000006,			// Program header table itself
		LOOS		= 0x60000000,			// Inclusive reserved ranges for OS/Proc semantic
		HIOS		= 0x6FFFFFFF,			// --- // ---
		LOPROC		= 0x70000000,			// --- // ---
		HIPROC		= 0x7FFFFFFF			// --- // ---
	};

	// ELF program header
	struct elfProgramHeader_t {

		ELF_PROGRAM_TYPE	type;			// ELF Program table type

#if 	(IGROS_ARCH == x86_64)

		dword_t			flags;			// ELF Program table 64-bit flags

#endif	// (IGROS_ARCH == x86_64)

		std::size_t		offset;			// ELF Program table segment offset
		pointer_t		vaddr;			// ELF Program table segment virtual address
		pointer_t		paddr;			// ELF Program table segment physical address
		std::size_t		fileSize;		// ELF Program table segment in-file size
		std::size_t		memorySyze;		// ELF Program table segment in-memory size

#if 	(IGROS_ARCH == i386)

		dword_t			flags;			// ELF Program table 32-bit flags

#endif	// (IGROS_ARCH == i386)

		pointer_t		align;			// ELF Program table segment alignment

	};


}	// namespace sys


#endif	// IGROS_ARCH_ELF_HPP

