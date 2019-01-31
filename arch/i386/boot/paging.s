################################################################
#
#	Memory paging low-level operations
#
#	File:	paging.s
#	Date:	21 Jun. 2018
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code32


.section .bss
.balign 4096
.global	pageDirectory
.global	pageTable


pageDirectory:
	.skip 4096
pageTable:
	.skip 4096

