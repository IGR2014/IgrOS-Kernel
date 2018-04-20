################################################################
#
#	arch folder Make script
#
#	File:	Makefile
#	Date:	20 Apr. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


export IGROS_DIR_BUILD		= $(CURDIR)/../build
export IGROS_DIR_RELEASE	= $(CURDIR)/../release

IGROS_ARCH			=

.PHONY: all clean rebuild run

all:
	@mkdir -p $(IGROS_DIR_BUILD)
	@mkdir -p $(IGROS_DIR_RELEASE)
	@$(MAKE) -C $(IGROS_ARCH)

clean:
	@rm -rf $(IGROS_DIR_BUILD)
	@rm -rf $(IGROS_DIR_RELEASE)
	@$(MAKE) -C $(IGROS_ARCH) clean

rebuild:
	@$(MAKE) -C $(IGROS_ARCH) rebuild

run:
	@$(MAKE) -C $(IGROS_ARCH) run

