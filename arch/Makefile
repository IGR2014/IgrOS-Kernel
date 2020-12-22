################################################################
#
#	arch folder Make script
#
#	File:	Makefile
#	Date:	14 Feb 2020
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


.PHONY: all clean rebuild run

all:
	@echo "	MAKE	$(IGROS_ARCH)"
	@$(MAKE) -C $(IGROS_ARCH)

clean:
	@echo "	CLEAN	$(IGROS_ARCH)"
	@$(MAKE) -C $(IGROS_ARCH) clean

rebuild: clean all

run: all
	@echo "	RUN	$(IGROS_ARCH)"
	@$(MAKE) -C $(IGROS_ARCH) run

