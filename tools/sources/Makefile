#*****************************************************************************
# FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/Makefile,v $
# SHORTNAME      : Makefile
# SNNS VERSION   : 4.2
#
# PURPOSE        : Makefile for tools make
# NOTES          : Do not change Makefiles manually
#
# AUTHOR         : Michael Vogt
# DATE           : 27.02.98
#
# CHANGED BY     : 
# RCS VERSION    : $Revision: 2.16 $
# LAST CHANGE    : $Date: 1998/05/26 14:56:26 $
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
CPPFLAGS           += -I../.. \
                     -I../../$(KERNELSRCDIR) \
                     $(ENZOFLAGS)

# Here you may change the default C-compiler flags as set in
# ../../Makefile.def (not recommended)
# CFLAGS           = -g -O

# Source files

TOOLS_SOURCES_C    = analyze.c \
		     pat_sel_simple.c \
                     mkhead.c \
                     mkout.c \
                     mkpat.c

KRTOOLS_SOURCES_C  = snns2c.c \
                     snns2clib.c \
                     pat_sel.c \
                     netlearn.c \
                     td_bignet.c \
                     ff_bignet.c \
                     netperf.c \
                     snnsbat.c \
                     feedback-gennet.c \
                     convert2snns.c \
                     isnns.c \
                     linknets.c

BATCH_SOURCES_YACC = gram1.y
BATCH_SOURCES_LEX  = scan1.l \
                     scan2.l
BATCH_SOURCES_C    = batchman.c \
                     gram1.tab.c \
                     lex.yyz.c \
                     icopjmp.c \
                     ictab.c \
                     symtab.c \
                     backpatch.c \
                     memory.c \
                     ic_snns.c \
                     error.c \
                     arglist.c \
                     snns_intfce.c \
                     special_jacket.c \
                     lex.yyy.c 

# Modul files

MODULES            = $(TOOLS_SOURCES_C) \
                     $(KRTOOLS_SOURCES_C) \
                     $(BATCH_SOURCES_C)

# Object files

TOOLS_OBJECTS      = $(TOOLS_SOURCES_C:.c=.o)
KRTOOLS_OBJECTS    = $(KRTOOLS_SOURCES_C:.c=.o)
BATCH_OBJECTS      = $(BATCH_SOURCES_C:.c=.o)

# Programs

TOOLS_PRG          = $(TOOLS_SOURCES_C:.c=)
KRTOOLS_PRG        = snns2c pat_sel netlearn td_bignet ff_bignet \
                     netperf snnsbat feedback-gennet convert2snns \
                     isnns linknets
BATCH_PRG          = batchman

# Libraries

KERNELLIBS         = ../../$(KERNELSRCDIR)/libkernel.a \
	             ../../$(KERNELSRCDIR)/libfunc.a

KERNELALLLIBS      = $(KERNELLIBS) -lm $(LEXLIB)

# Targets

all:         tools

simpletools: $(TOOLS_PRG)
kerneltools: $(KRTOOLS_PRG)
batchtools:  $(BATCH_PRG)
tools:       $(TOOLS_PRG) $(KRTOOLS_PRG) $(BATCH_PRG)

$(TOOLSBINDIR):
	-mkdir $(TOOLSBINDIR)

install: tools $(TOOLSBINDIR)
	for tooltarget in $(TOOLS_PRG) $(KRTOOLS_PRG) $(BATCH_PRG) ; do \
            $(INSTALL_PROGRAM) $$tooltarget $(TOOLSBINDIR) ; \
        done

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

uninstall:
	for tooltarget in $(TOOLS_PRG) $(KRTOOLS_PRG) $(BATCH_PRG) ; do \
            rm -f $(TOOLSBINDIR)/$$tooltarget ; \
        done

mostlyclean:
	rm -f $(TOOLS_OBJECTS) $(KRTOOLS_OBJECTS) $(BATCH_OBJECTS)

clean: mostlyclean
	rm -f $(TOOLS_PRG) $(KRTOOLS_PRG) $(BATCH_PRG)

distclean: clean
	rm -f *~ *% *.bak

maintainer-clean: distclean
	@echo "This command deletes files that may need special tools to rebuild."
	@echo "You need to have flex to rebuild a scanner."
	@echo "You need to have bison to rebuild a parser."
	rm -f lex.yyy.c lex.yyz.c gram1.tab.c gram1.tab.h

depend:
	-$(MAKEDEPEND) -- $(CPPFLAGS) -- -w3 -f- $(MODULES) | \
	grep -v ': */' > tools.ddt

# Linking of single stand alone programs

analyze: analyze.o
	$(CC) analyze.o -lm -o analyze

pat_sel_simple: pat_sel_simple.o
	$(CC) pat_sel_simple.o -lm -o pat_sel_simple

mkhead: mkhead.o
	$(CC) mkhead.o -lm -o mkhead

mkout: mkout.o
	$(CC) mkout.o -lm -o mkout

mkpat: mkpat.o
	$(CC) mkpat.o -lm -o mkpat


# Make sure that kernel libs are available and up to date

$(KERNELLIBS): ../../Makefile.def ../../$(KERNELSRCDIR)/Makefile \
               ../../config.h
	@echo ""
	@echo "need to (re)make kernel libraries first..."
	@echo ""
	cd ../../$(KERNELSRCDIR) && $(MAKE)
	@echo ""
	@echo "end of (re)making kernel libraries"
	@echo ""

# Linking of single programs based on kernel parts

snns2c: snns2c.o snns2clib.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) snns2c.o snns2clib.o $(KERNELALLLIBS) -o snns2c

pat_sel: pat_sel.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) pat_sel.o $(KERNELALLLIBS) -o pat_sel

netlearn: netlearn.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) netlearn.o $(KERNELALLLIBS) -o netlearn

td_bignet: td_bignet.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) td_bignet.o $(KERNELALLLIBS) -o td_bignet

ff_bignet: ff_bignet.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) ff_bignet.o $(KERNELALLLIBS) -o ff_bignet

netperf: netperf.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) netperf.o $(KERNELALLLIBS) -o netperf

snnsbat: snnsbat.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) snnsbat.o $(KERNELALLLIBS) -o snnsbat

feedback-gennet: feedback-gennet.o $(KERNELLIBS)
#	$(CC) $(LDFLAGS) feedback-gennet.o $(KERNELALLLIBS) -o feedback-gennet
	$(CC) $(LDFLAGS) feedback-gennet.o $(KERNELLIBS) -o feedback-gennet

convert2snns: convert2snns.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) convert2snns.o $(KERNELALLLIBS) -o convert2snns

isnns: isnns.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) isnns.o $(KERNELALLLIBS) -o isnns

linknets: linknets.o snns2clib.o $(KERNELLIBS)
	$(CC) $(LDFLAGS) linknets.o snns2clib.o $(KERNELALLLIBS) -o linknets


# Linking of batchman

batchman: $(BATCH_OBJECTS) $(KERNELLIBS)
	$(CC) $(LDFLAGS) $(BATCH_OBJECTS) $(KERNELALLLIBS) -o batchman

# Parser generation for batchman

lex.yyz.c: scan1.l
	@echo ""
	@echo "You need to have flex to rebuild a scanner."
	@echo "If the following commands fail you will either have to"
	@echo "install flex and rerun 'configure' in $(SNNSDIR)" 
	@echo "or to 'touch' the unchanged file lex.yyz.c from the SNNS distribution."
	-mv lex.yyz.c lex.yyz.c.bak
	-$(FLEX) -Pyyz scan1.l
	@echo "END OF COMMANDS THAT MAY FAIL"
	@echo ""

lex.yyy.c: scan2.l
	@echo ""
	@echo "You need to have flex to rebuild a scanner."
	@echo "If the following commands fail you will either have to"
	@echo "install flex and rerun 'configure' in $(SNNSDIR)" 
	@echo "or to 'touch' the unchanged file lex.yyy.c from the SNNS distribution."
	-mv lex.yyy.c lex.yyy.c.bak
	-$(FLEX) -Pyyy scan2.l
	@echo "END OF COMMANDS THAT MAY FAIL"
	@echo ""

gram1.tab.c gram1.tab.h: gram1.y
	@echo ""
	@echo "You need to have bison to rebuild a parser."
	@echo "If the following commands fail you will either have to"
	@echo "install bison and rerun 'configure' in $(SNNSDIR),"
	@echo "or to 'touch' the original files gram1.tab.c and gram1.tab.h"
	@echo "from the SNNS distribution."
	@echo "Also read the notes about 'bison' versions in Readme.install."
	-mv gram1.tab.c gram1.tab.c.bak
	-mv gram1.tab.h gram1.tab.h.bak
	-$(BISON) $(BISONFLAGS) yyz gram1.y
	-mv y.tab.c gram1.tab.c
	-mv y.tab.h gram1.tab.h
	@echo "END OF COMMANDS THAT MAY FAIL"
	@echo ""

# Dependencies

$(TOOLS_OBJECTS): Makefile ../../Makefile.def
$(KRTOOLS_OBJECTS): Makefile ../../Makefile.def
$(BATCH_OBJECTS): Makefile ../../Makefile.def

include tools.ddt

#*****************************************************************************
# End of tools Makefile
#*****************************************************************************
