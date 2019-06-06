################################################################
#
#	Memory paging low-level operations
#
#	File:	paging.s
#	Date:	06 Jun 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code64


.section .bss
.balign	4096
.global	pageMapLevel4Table
.global	pageDirectoryPointer
.global	pageDirectory
.global	pageTable


pageMapLevel4Table:
	.skip	4096
pageDirectoryPointer:
	.skip	4096
pageDirectory:
	.skip	4096
pageTable:
	.skip	4096

