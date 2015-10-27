#*****************************************************************************
# FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/Makefile,v $
# SHORTNAME      : Makefile
# SNNS VERSION   : 4.2
#
# PURPOSE        : Basic Makefile for simplyfied SNNS installation
# NOTES          : Do not change Makefiles
#
# AUTHOR         : Michael Vogt
# DATE           : 27.02.98
#
# CHANGED BY     : 
# RCS VERSION    : $Revision: 1.6 $
# LAST CHANGE    : $Date: 1998/03/18 15:24:09 $
#
#   Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
#   Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
#
#*****************************************************************************

KERNELSRCDIR       = kernel/sources
TOOLSSRCDIR        = tools/sources
XGUISRCDIR         = xgui/sources
MANSRCDIR          = man

# Default target for the very impatient user

all: makefiles compile


# Be sure that configuration is up to date

makefiles: Makefile.def config.h

Makefile.def config.h: configuration/Makefile.din configuration/config.hin configure
	@echo "Ensuring that SNNS is completely configured..."
	./configure --quiet
	@# touch config.h even if it's unchanged, so this target is done
	@touch config.h
	@echo ""

configure: configuration/configure.in
	@echo "configure script should be remade..."
	cd configuration && autoconf && mv configure ..
	@echo ""

configuration/config.hin: configuration/configure.in configuration/config.h.bot \
             configuration/acconfig.h
	@echo "config.hin should be remade..."
	cd configuration \
	&& autoheader configure.in > config.hin
	@echo ""

# Targets for compilation and installation
# Note that any install target ensures the kernel compilation through
# the subordinated Makefile

compile: compile-kernel compile-tools compile-xgui

compile-kernel:
	cd $(KERNELSRCDIR) && $(MAKE)

compile-tools:
	cd $(TOOLSSRCDIR) && $(MAKE)

compile-xgui:
	cd $(XGUISRCDIR) && $(MAKE)


install: install-tools install-xgui

install-tools:
	cd $(TOOLSSRCDIR) && $(MAKE) install-strip

install-xgui:
	cd $(XGUISRCDIR) && $(MAKE) install-strip

install-man:
	cd $(MANSRCDIR) && $(MAKE) install

# Targets for cleaning up after installation

clean: clean-kernel clean-tools clean-xgui
	rm -f config.status config.cache config.log

clean-kernel:
	cd $(KERNELSRCDIR) && $(MAKE) clean

clean-tools:
	cd $(TOOLSSRCDIR) && $(MAKE) clean

clean-xgui:
	cd $(XGUISRCDIR) && $(MAKE) clean


# Targets to remove the SNNS installation

uninstall: uninstall-kernel uninstall-tools uninstall-xgui

uninstall-kernel:
	cd $(KERNELSRCDIR) && $(MAKE) uninstall

uninstall-tools:
	cd $(TOOLSSRCDIR) && $(MAKE) uninstall

uninstall-xgui:
	cd $(XGUISRCDIR) && $(MAKE) uninstall

uninstall-man:
	cd $(MANSRCDIR) && $(MAKE) uninstall

# Target to give information for bug reports

bugreport: makefiles
	@echo "Definitions in Makefile.def:"
	@echo ""
	@grep REPORT_IF_BUG Makefile.def | sed -e 's/# REPORT_IF_BUG//g'
	@echo ""
	@echo "Definitions in config.h:"
	@echo ""
	@grep '#define' config.h

#*****************************************************************************
# End of Makefile
#*****************************************************************************
