#*****************************************************************************
# FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/Makefile,v $
# SHORTNAME      : Makefile
# SNNS VERSION   : 4.2
#
# PURPOSE        : Makefile for kernel make
# NOTES          : Do not change Makefiles manually
#
# AUTHOR         : Michael Vogt
# DATE           : 27.02.98
#
# CHANGED BY     : 
# RCS VERSION    : $Revision: 2.13 $
# LAST CHANGE    : $Date: 1998/03/18 15:24:16 $
#
#   Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
#   Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
#
#*****************************************************************************

# Read all global definitions, common for all source Makefiles

include ../../Makefile.def

# Final compiler definitions

# Define final C-preprocessor options for the kernel
# Additional definitions like -I... and -D... go here
CPPFLAGS           = -I../.. \
                     $(ENZOFLAGS)

# Here you may change the default C-compiler flags as set in
# ../../Makefile.def (not recommended)
# CFLAGS           = -g -O

# Source files

KERNELSOURCES_YACC = kr_pat_parse_bison.y
KERNELSOURCES_LEX  = kr_pat_scan.l
KERNELSOURCES_C    = kernel.c \
                     kr_io.c \
                     kr_mem.c \
                     kr_ui.c \
                     kr_inversion.c \
                     art_ui.c \
                     kr_art.c \
                     kr_art1.c \
                     kr_art2.c \
                     kr_amap.c \
		     kr_pat_parse.c \
                     kr_newpattern.c\
                     kr_JordElm.c\
		     kr_rand.c

USR_FUNC_SOURCES_C = func_tbl.c \
                     init_f.c \
                     kr_td.c \
                     kr_funcs.c \
                     learn_f.c \
                     prun_f.c \
                     tbl_func.c \
                     trans_f.c \
                     update_f.c \
                     remap_f.c \
                     matrix.c \
                     arttr_f.c \
                     cc_learn.c \
                     cc_prune.c \
                     cc_modify.c \
                     cc_glob.c \
                     cc_display.c \
                     tacoma_learn.c \
                     dlvq_learn.c \
                     stochastic_learn_f.c \
		     scaled_conj_grad.c

UTILITY_SOURCES_C  = make_tbl.c \
                     calc_err.c

# Modul files

MODULES            = $(KERNELSOURCES_C) \
                     $(KERNELSOURCES_LEX) \
                     $(USR_FUNC_SOURCES_C) \
                     $(UTILITY_SOURCES_C)

# Object files

KERNELOBJECTS	   = $(KERNELSOURCES_LEX:.l=.o) \
                     $(KERNELSOURCES_C:.c=.o)
USR_FUNC_OBJECTS   = $(USR_FUNC_SOURCES_C:.c=.o)
UTILITY_OBJECTS    = $(UTILITY_SOURCES_C:.c=.o)

# Libraries

LIBKERNEL	   = libkernel.a
LIBFUNCS	   = libfunc.a

# Targets

all: $(LIBKERNEL) $(LIBFUNCS)

$(KERNELBINDIR):
	-mkdir $(KERNELBINDIR)

install: all $(KERNELBINDIR)
	@echo "nothing to install"

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

uninstall:
	@echo "nothing to uninstall"

mostlyclean:
	rm -f $(KERNELOBJECTS) $(USR_FUNC_OBJECTS) $(SUBSTOBJECTS)

clean: mostlyclean
	rm -f $(LIBKERNEL) $(LIBFUNCS)

distclean: clean
	rm -f *~ *% *.bak

maintainer-clean: distclean
	@echo "This command deletes files that may need special tools to rebuild."
	@echo "You need to have flex to rebuild a scanner."
	@echo "You need to have bison to rebuild a parser."
	rm -f kr_pat_scan.c kr_pat_parse.c y.tab.h

depend:
	-$(MAKEDEPEND) -- $(CPPFLAGS) -- -w3 -f- $(MODULES) | \
	grep -v ': */' > kernel.ddt

# Building of libraries

$(LIBKERNEL): $(KERNELOBJECTS)
	$(AR) $(ARFLAGS) $(LIBKERNEL) $(KERNELOBJECTS)
	@echo ""
	@echo "Ignore possible error messages for the following command:"
	-$(RANLIB) $(LIBKERNEL)
	@echo "END OF COMMANDS THAT MAY FAIL"
	@echo ""

$(LIBFUNCS): $(USR_FUNC_OBJECTS) $(SUBSTOBJECTS)
	$(AR) $(ARFLAGS) $(LIBFUNCS) $(USR_FUNC_OBJECTS) $(SUBSTOBJECTS)
	@echo ""
	@echo "Ignore possible error messages for the following command:"
	-$(RANLIB) $(LIBFUNCS)
	@echo "END OF COMMANDS THAT MAY FAIL"
	@echo ""

# Parser and Scanner generation

kr_pat_scan.c: kr_pat_scan.l
	@echo ""
	@echo "You need to have flex to rebuild a scanner."
	@echo "If the following commands fail you will either have to"
	@echo "install flex and rerun 'configure' in $(SNNSDIR)" 
	@echo "or to 'touch' the unchanged file kr_pat_scan.c from the SNNS distribution."
	-$(FLEX) kr_pat_scan.l
	-mv kr_pat_scan.c kr_pat_scan.c.bak
	-mv lex.yy.c kr_pat_scan.c
	@echo "END OF COMMANDS THAT MAY FAIL"
	@echo ""

kr_pat_parse.c y.tab.h: kr_pat_parse_bison.y
	@echo ""
	@echo "You need to have bison to rebuild a parser."
	@echo "If the following commands fail you will either have to"
	@echo "install bison and rerun 'configure' in $(SNNSDIR),"
	@echo "or to 'touch' the original files kr_pat_parse.c and y.tab.h"
	@echo "from the SNNS distribution."
	@echo "Also read the notes about 'bison' versions in Readme.install."
	-mv kr_pat_parse.c kr_pat_parse.c.bak
	-mv y.tab.h y.tab.h.bak
	-$(BISON) $(BISONFLAGS) pp kr_pat_parse_bison.y
	-mv y.tab.c kr_pat_parse.c
	@echo "END OF COMMANDS THAT MAY FAIL"
	@echo ""

# Dependencies

$(KERNELOBJECTS): Makefile ../../Makefile.def
$(USR_FUNC_OBJECTS): Makefile ../../Makefile.def

include kernel.ddt

#*****************************************************************************
# End of kernel Makefile
#*****************************************************************************
