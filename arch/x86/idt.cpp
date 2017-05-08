#include <include/idt.hpp>


namespace arch {

	// Create IDT entry
	idtEntry idtSetEntry(const t_u32 &offset, const t_u16 &selector, const t_u8 &type) {

		idtEntry entry;	

		entry.offsetLow		= offset & 0xFFFF;
		entry.selector		= selector;
		entry.reserved		= 0x00;
		entry.type		= type;
		entry.offsetHigh	= (offset & 0xFFFF0000) >> 16;

		return entry;

	}

}	// arch

