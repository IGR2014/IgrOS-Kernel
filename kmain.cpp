////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	08 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <version.hpp>
#include <multiboot.hpp>
#include <platform.hpp>
#include <flags.hpp>

#include <arch/types.hpp>
#include <arch/gdt.hpp>
#include <arch/idt.hpp>
#include <arch/interrupts.hpp>
#include <arch/paging.hpp>

#include <drivers/vmem.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/pit.hpp>

#include <klib/kstring.hpp>
#include <klib/kprint.hpp>

#include <mem/mmap.hpp>


// Kernel start and end
extern const byte_t _SECTION_KERNEL_START_;
extern const byte_t _SECTION_KERNEL_END_;


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Kernel main function
	void kmain(multiboot::info_t* multiboot, dword_t magic) {

		// Init VGA memory
		arch::vmemInit();

		// Print buffer
		sbyte_t text[1024];

		// Check multiboot magic
		if (!multiboot::check(magic)) {

			// Write Multiboot magic error message message
			klib::ksprint(text,	"IgrOS kernel\r\n\r\n"
						"\tBAD MULTIBOOT MAGIC!!!\r\n"
						"\tMAGIC:\t\t0x%x\r\n"
						"\tADDRESS:\t0x%p\r\n", magic, multiboot);
			arch::vmemWrite(text);

			// Hang CPU
			while (true) {};

		}

		// Write Multiboot magic error message message
		klib::ksprint(text, "IgrOS kernel\r\n\r\n");
		arch::vmemWrite(text);

		// Write Multiboot info message
		klib::ksprint(text,	"BOOTLOADER INFO\r\n"
					"Command line:\t%s\r\n"
					"Loader name:\t%s\r\n"
					"\r\n",
					multiboot->commandLine(),
					multiboot->loaderName());
		arch::vmemWrite(text);

		// Dump memory info
		if (multiboot->hasInfoMemory()) {
			klib::ksprint(text,	"MEMORY INFO:\r\n"
						"\tLow:\t%d Kb\r\n"
						"\tHigh:\t%d Mb.\r\n",
						multiboot->memLow,
						multiboot->memHigh >> 10);
			arch::vmemWrite(text);
		} else {
			klib::ksprint(text,	"\tNo memory info provided...\r\n");
			arch::vmemWrite(text);
		}
		klib::ksprint(text, "\r\n");
		arch::vmemWrite(text);

		// Dump multiboot memory map
		if (multiboot->hasInfoMemoryMap()) {
			klib::ksprint(text,	"MEMORY MAP:\r\n"
						"\tSize:\t%d bytes\r\n"
						"\tAddr:\t0x%p\r\n",
						multiboot->mmapLength,
						multiboot->mmapAddr);
			arch::vmemWrite(text);
			// Get pointer to memory map
			auto memoryMap = reinterpret_cast<multiboot::memoryMapEntry_t*>(multiboot->mmapAddr);
			// Loop through memory map
			while (quad_t(memoryMap) < (multiboot->mmapAddr + multiboot->mmapLength)) {
				klib::ksprint(text,	"\t[%d] 0x%xll - 0x%xll\r\n",
							memoryMap->type,
							memoryMap->address,
							memoryMap->address + memoryMap->length);
				arch::vmemWrite(text);
				// Move to next memory map entry
				memoryMap = reinterpret_cast<multiboot::memoryMapEntry_t*>(quad_t(memoryMap) + memoryMap->size + sizeof(memoryMap->size));
			}
		} else {
			klib::ksprint(text,	"\tNo memory map provided...\r\n");
			arch::vmemWrite(text);
		}
		klib::ksprint(text, "\r\n");
		arch::vmemWrite(text);

		klib::ksprint(text,	"KERNEL INFO\r\n"
					"Arch:\t\t%s\r\n"
					"Start addr:\t0x%p\r\n"
					"End addr:\t0x%p\r\n"
					"Size:\t\t%d Kb.\r\n"
					"Build:\t\t" __DATE__ ", " __TIME__ "\r\n"
					"Version:\tv%d.%d.%d [%s]\r\n"
					"Author:\t\tIgor Baklykov (c) %d - %d\r\n"
					"\r\n",
					(IGROS_ARCH),
					&_SECTION_KERNEL_START_,
					&_SECTION_KERNEL_END_,
					(&_SECTION_KERNEL_END_ - &_SECTION_KERNEL_START_) >> 10,
					IGROS_VERSION_MAJOR,
					IGROS_VERSION_MINOR,
					IGROS_VERSION_BUILD,
					IGROS_VERSION_NAME,
					2017,
					2019);
		arch::vmemWrite(text);

		// Dump multiboot flags
		//multiboot->dumpFlags();

		// Setup Interrupts Descriptor Table
		arch::idtSetup();

		// Init interrupts
		arch::irqInit();

		// Enable interrupts
		arch::irqEnable();

		// Setup Global Descriptors Table
		arch::gdtSetup();

		// Setup physical memory map
		//mem::mmapInit(reinterpret_cast<multiboot::memoryMapEntry_t*>(multiboot->mmapAddr), multiboot->mmapAddr + multiboot->mmapLength);

		// Setup paging (And identity map first 4MB where kernel physically is)
		arch::pagingSetup();

		// Setup keyboard
		arch::keyboardSetup();
		// Setup PIT
		arch::pitSetup();

		// Write "Booted successfully" message
		klib::ksprint(text, "\r\nBooted successfully\r\n\r\n");
		arch::vmemWrite(text);

		/*
		byte_t* ptr = reinterpret_cast<byte_t*>(0xA0000);
		for (dword_t j = 0; j < 10000000; j+=4) {
			ptr[j + 0] = 0x00;
			ptr[j + 1] = 0xFF;
			ptr[j + 2] = 0x00;
		}
		*/

		/*
		// Page mapping test (higher half test)
		volatile word_t* ptr = reinterpret_cast<word_t*>(0xC00B8006);
		klib::ksprint(text,	"Paging test:\t0x%p = 0x%p\r\n"
					"\r\n",
					ptr,
					arch::pagingVirtToPhys(pointer_t(ptr)));
		arch::vmemWrite(text);
		// Rewrite 'IgrOS' text 'O' and 'S' green symbols with white ones
		*ptr = 0x0700 | 'O';
		++ptr;
		*ptr = 0x0700 | 'S';
		*/

		/*
		// Page Fault Exception test
		volatile word_t* ptr2 = reinterpret_cast<word_t*>(0xA0000000);
		*ptr2 = 0x4000;
		*/

		/*
		// Divide by Zero Exception Test
		volatile dword_t x = 10;
		volatile dword_t y = 0;
		volatile dword_t z = x / y;
		*/

	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

