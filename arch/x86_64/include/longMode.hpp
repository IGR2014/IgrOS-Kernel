#ifndef LONG_MODE_H
#define LONG_MODE_H


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {

	// Set GDT
	extern "C" {

		// Check if loaded with multiboot loader
		t_u8	checkMultiboot();
		// Check CPUID is extended
		t_u8	checkCPUID();
		// Check CPUID for long mode existance
		t_u8	checkLongMode();

		// Setup page tables
		void	setupPageTables();
		// Enable paging
		void	enablePaging();

	}

}	// arch

#endif	// GDT_H

