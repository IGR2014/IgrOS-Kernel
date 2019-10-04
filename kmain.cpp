////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	04 Oct 2019
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
extern byte_t _SECTION_KERNEL_START_;
extern byte_t _SECTION_KERNEL_END_;


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Kernel main function
	void kmain(multiboot::info_t* multiboot, dword_t magic) {

		// Init VGA memory
		arch::vmemInit();
		arch::vmemWrite("IgrOS kernel\r\n\r\n");

		// Print buffer
		sbyte_t text[64];

		// Check multiboot magic
		if (!multiboot::check(magic)) {

			arch::vmemWrite("\tBAD MULTIBOOT MAGIC!!!\r\n");
			// Print multiboot magic
			arch::vmemWrite("\tMAGIC:\t\t0x");
			arch::vmemWrite(klib::kitoa(text, 20, magic, klib::base::HEX));
			// Print multiboot address
			arch::vmemWrite("\r\n\tADDRESS:\t0x");
			arch::vmemWrite(klib::kptoa(text, 20, multiboot));

			// Hang CPU
			while (true) {};

		}

		// Dump multiboot flags
		//multiboot->dumpFlags();

		// Write Multiboot info message
		arch::vmemWrite("Bootloader info\r\n");
		// Dump multiboot command line
		//arch::vmemWrite("\tCommand line:\t");
		//arch::vmemWrite(multiboot->commandLine());
		//arch::vmemWrite("\r\n");
		// Dump multiboot bootloader name
		//arch::vmemWrite("\tLoader name:\t");
		//arch::vmemWrite(multiboot->loaderName());
		//arch::vmemWrite("\r\n");
		// Dump memory info
		if (multiboot->hasInfoMemory()) {
			arch::vmemWrite("\tMemory info:\r\n");
			arch::vmemWrite("\t\tLow:\t");
			arch::vmemWrite(klib::kitoa(text, 64, multiboot->memLow));
			arch::vmemWrite("\r\n\t\tHigh:\t");
			arch::vmemWrite(klib::kitoa(text, 64, multiboot->memHigh));
			arch::vmemWrite("\r\n");
		} else {
			arch::vmemWrite("\tNo memory info provided...\r\n");
		}
		// Dump multiboot memory map
		if (multiboot->hasInfoMemoryMap()) {
			arch::vmemWrite("\tMemory map:\r\n");
			arch::vmemWrite("\t\tSize:\t");
			arch::vmemWrite(klib::kitoa(text, 64, multiboot->mmapLength));
			arch::vmemWrite("\r\n\t\tAddr:\t0x");
			arch::vmemWrite(klib::kitoa(text, 64, multiboot->mmapAddr, klib::base::HEX));
			arch::vmemWrite("\r\n");
			// Get pointer to memory map
			multiboot::memoryMapEntry_t* memoryMap = reinterpret_cast<multiboot::memoryMapEntry_t*>(multiboot->mmapAddr);
			// Loop through memory map
			while (quad_t(memoryMap) < (multiboot->mmapAddr + multiboot->mmapLength)) {
				arch::vmemWrite("\t\t\t[");
				arch::vmemWrite(klib::kitoa(text, 64, dword_t(memoryMap->type), klib::base::HEX));
				arch::vmemWrite("]\t0x");
				arch::vmemWrite(klib::kitoa(text, 64, memoryMap->address, klib::base::HEX));
				arch::vmemWrite(" - 0x");
				arch::vmemWrite(klib::kitoa(text, 64, (memoryMap->address + memoryMap->length), klib::base::HEX));
				arch::vmemWrite("\r\n");
				// Move to next memory map entry
				memoryMap = reinterpret_cast<multiboot::memoryMapEntry_t*>(quad_t(memoryMap) + memoryMap->size + sizeof(memoryMap->size));
			}
		} else {
			arch::vmemWrite("\tNo memory map provided...\r\n");
		}
		arch::vmemWrite("\r\n");

		// Write Kernel info message
		arch::vmemWrite("Kernel info\r\n");
		arch::vmemWrite("\tStart addr:\t0x");
		arch::vmemWrite(klib::kitoa(text, 64, quad_t(&_SECTION_KERNEL_START_), klib::base::HEX));
		arch::vmemWrite("\r\n\tEnd addr:\t0x");
		arch::vmemWrite(klib::kitoa(text, 64, quad_t(&_SECTION_KERNEL_END_), klib::base::HEX));
		arch::vmemWrite("\r\n\tSize:\t\t");
		arch::vmemWrite(klib::kitoa(text, 64, quad_t(&_SECTION_KERNEL_END_ - &_SECTION_KERNEL_START_)));
		arch::vmemWrite(" bytes \r\n");
		arch::vmemWrite("\tBuild:\t\t" __DATE__ ", " __TIME__ "\r\n");
		arch::vmemWrite("\tVersion:\tv");
		arch::vmemWrite(klib::kitoa(text, 20, IGROS_VERSION_MAJOR));
		arch::vmemWrite(".");
		arch::vmemWrite(klib::kitoa(text, 20, IGROS_VERSION_MINOR));
		arch::vmemWrite(".");
		arch::vmemWrite(klib::kitoa(text, 20, IGROS_VERSION_BUILD));
		arch::vmemWrite(" (");
		arch::vmemWrite(IGROS_VERSION_NAME);
		arch::vmemWrite(")\r\n");
		arch::vmemWrite("\tAuthor:\t\tIgor Baklykov (c) ");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(2017)));
		arch::vmemWrite(" - ");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(2019)));
		arch::vmemWrite("\r\n\r\n");

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
		arch::vmemWrite("\r\nBooted successfully\r\n\r\n");

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
		arch::vmemWrite("0x");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(ptr), klib::base::HEX));
		arch::vmemWrite(" = 0x");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(arch::pagingVirtToPhys(pointer_t(ptr))), klib::base::HEX));
		arch::vmemWrite("\r\n\r\n");
		// Rewrite 'IgrOS' text 'O' and 'S' green symbols with white ones
		*ptr = 0x0700 | 'O';
		++ptr;
		*ptr = 0x0700 | 'S';
		*/

		/*
		// Numbers print test
		arch::vmemWrite("0x");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(0x7FFFFFFF), klib::base::HEX));
		arch::vmemWrite(" = ");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(0x7FFFFFFF)));
		arch::vmemWrite("\r\n");
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

