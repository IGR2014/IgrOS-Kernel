////////////////////////////////////////////////////////////////
//
//	ELF header info
//
//	File:	elf.hpp
//	Date:	09 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>

#include <arch/types.hpp>


// System code zone
namespace igros::sys {


        // ELF identity magic
        constexpr static auto	ELF_MAGIC_0			{0x7F_u8};		// 127
	constexpr static auto	ELF_MAGIC_1			{0x45_u8};		// 'E'
	constexpr static auto	ELF_MAGIC_2			{0x4C_u8};		// 'L'
	constexpr static auto	ELF_MAGIC_3			{0x46_u8};		// 'F'


	// ELF class type
	enum class ELF_CLASS : igros_byte_t {
		NONE		= 0x00_u8,			// Unknown
		CLASS32		= 0x01_u8,			// 32-bit
		CLASS64		= 0x02_u8			// 64-bit
	};

	// ELF data type
	enum class ELF_DATA : igros_byte_t {
		NONE		= 0x00_u8,			// Unknown
		LSB		= 0x01_u8,			// LSB order
		MSB		= 0x02_u8			// MSB order
	};

	// ELF version
	enum class ELF_VERSION : igros_byte_t {
		NONE		= 0x00_u8,			// Unknown version
		CURRENT		= 0x01_u8			// Current version
	};

	// ELF OS ABI
	enum class ELF_ABI : igros_byte_t {
		SYSV		= 0x00_u8,			// System-V
		HP_UX		= 0x01_u8,			// HP-UX
		NETBSD		= 0x02_u8,			// NetBSD
		LINUX		= 0x03_u8,			// Linux
		HURD		= 0x04_u8,			// GNU Hurd
		SOLARIS		= 0x06_u8,			// Solaris
		AIX		= 0x07_u8,			// AIX
		IRIS		= 0x08_u8,			// IRIS
		FREEBSD		= 0x09_u8,			// FreeBSD
		TRU64		= 0x0A_u8,			// Tru64
		NOVELL		= 0x0B_u8,			// Novell Modesto
		OPENBSD		= 0x0C_u8,			// OpenBSD
		VMS		= 0x0D_u8,			// OpenVMS
		NONSTOP		= 0x0E_u8,			// NonStop Kernel
		AROS		= 0x0F_u8,			// AROS
		FENIX		= 0x10_u8,			// Fenix OS
		CLOUD		= 0x11_u8,			// Cloud ABI
		VOS		= 0x12_u8			// OpenVOS
	};


	// ELF identity class
	struct elfIdentity_t {
		igros_byte_t		magic[4_usize];			// ELF magic
		ELF_CLASS		classType;			// ELF class
		ELF_DATA		dataType;			// ELF data order
		ELF_VERSION		version;			// ELF version
		ELF_ABI			abiType;			// ELS OS ABI
		igros_byte_t		abiVersion;			// ELS OS ABI version
		igros_byte_t		reserved[7_usize];		// Unused
	};


	// ELF type
	enum class ELF_TYPE : igros_word_t {
		NONE		= 0x0000_u16,				// Unknown
		REL		= 0x0001_u16,				// Relocatable file
		EXEC		= 0x0002_u16,				// Executable file
		DYN		= 0x0003_u16,				// Shared object file
		CORE		= 0x0004_u16,				// Core file
		LOOS		= 0xFE00_u16,				// OS-specific
		HIOS		= 0xFEFF_u16,				// --- // ---
		LOPROC		= 0xFF00_u16,				// Processor-specific
		HIPROC		= 0xFFFF_u16				// --- // ---
	};


	// ELF machine type
	enum class ELF_MACHINE : igros_word_t {
		NONE		= 0x00_u16,				// Unknown
		M32		= 0x01_u16,				// AT&T WE 32100
		SPARC		= 0x02_u16,				// SPARC
		I386		= 0x03_u16,				// Intel 80386
		M68K		= 0x04_u16,				// Motorola 68000
		M88K		= 0x05_u16,				// Motorola 88000
		I8086		= 0x07_u16,				// Intel 80860
		MIPS		= 0x08_u16,				// MIPS RS3000
		POWERPC		= 0x14_u16,				// PowerPC
		S390		= 0x16_u16,				// IBM s390
		ARM		= 0x28_u16,				// ARM
		SUPERH		= 0x2A_u16,				// SuperH
		IA_64		= 0x32_u16,				// Intel 64
		X86_64		= 0x3E_u16,				// AMD/Intel x86-64
		AARCH64		= 0xB7_u16,				// AArch64
		RISC_V		= 0xF3_u16				// RISC-V
	};

	// ELF file header
        struct elfHeader_t {
		elfIdentity_t		identity;			// ELF Header identity
		ELF_TYPE		type;				// ELF Header type
                ELF_MACHINE		machine;			// ELF Header machine type
                ELF_VERSION		version;			// ELF Header version
                igros_pointer_t		entry;				// ELF Header entry address
		igros_usize_t		programHeader;			// ELF Header program header
		igros_usize_t		sectionHeader;			// ELF Header section header
		igros_dword_t		flags;				// ELF Header flags
		igros_word_t		headerSize;			// ELF Header size
		igros_word_t		programHeaderSize;		// ELF Header program header size
		igros_word_t		programHeaderNum;		// ELF Header program header count
		igros_word_t		sectionHeaderSize;		// ELF Header section header size
		igros_word_t		sectionHeaderNum;		// ELF Header section header count
		igros_word_t		sectionNamesIndex;		// ELF Header names section index
        };


	// ELF program type
	enum class ELF_PROGRAM_TYPE : igros_dword_t {
		NONE		= 0x00000000_u32,			// Unused
		LOAD		= 0x00000001_u32,			// Loadable segment
		DYNAMIC		= 0x00000002_u32,			// Dynamic linking info
		INTERP		= 0x00000003_u32,			// Interpreter info
		NOTE		= 0x00000004_u32,			// Auxiliary info
		SHLIB		= 0x00000005_u32,			// Reserved
		PHDR		= 0x00000006_u32,			// Program header table itself
		LOOS		= 0x60000000_u32,			// Inclusive reserved ranges for OS/Proc semantic
		HIOS		= 0x6FFFFFFF_u32,			// --- // ---
		LOPROC		= 0x70000000_u32,			// --- // ---
		HIPROC		= 0x7FFFFFFF_u32			// --- // ---
	};

	// ELF program header
	struct elfProgramHeader_t {

		ELF_PROGRAM_TYPE	type;				// ELF Program table type

#if 	(IGROS_ARCH == x86_64)

		igros_dword_t		flags;				// ELF Program table 64-bit flags

#endif	// (IGROS_ARCH == x86_64)

		igros_usize_t		offset;				// ELF Program table segment offset
		igros_pointer_t		vaddr;				// ELF Program table segment virtual address
		igros_pointer_t		paddr;				// ELF Program table segment physical address
		igros_usize_t		fileSize;			// ELF Program table segment in-file size
		igros_usize_t		memorySyze;			// ELF Program table segment in-memory size

#if 	(IGROS_ARCH == i386)

		igros_dword_t		flags;				// ELF Program table 32-bit flags

#endif	// (IGROS_ARCH == i386)

		igros_pointer_t		align;				// ELF Program table segment alignment

	};


}	// namespace igros::sys

