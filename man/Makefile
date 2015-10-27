#*****************************************************************************
# FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/man/Makefile,v $
# SHORTNAME      : Makefile
# SNNS VERSION   : 4.2
#
# PURPOSE        : Makefile for tools make
# NOTES          : Do not change Makefiles manually
#
# AUTHOR         : Michael Vogt
# DATE           : 18.03.98
#
# CHANGED BY     : 
# RCS VERSION    : $Revision: 1.2 $
# LAST CHANGE    : $Date: 1998/03/19 12:57:52 $
# CHANGE LOG     : $Log: Makefile,v $
# CHANGE LOG     : Revision 1.2  1998/03/19 12:57:52  mamier
# CHANGE LOG     : introduced isnns manual page
# CHANGE LOG     :
# CHANGE LOG     : Revision 1.1  1998/03/18 15:24:20  vogtm
# CHANGE LOG     : man page installation included
# CHANGE LOG     :
#
#   Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
#   Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
#
#*****************************************************************************

# Read all global definitions, common for all source Makefiles

include ../Makefile.def

MAN1FILES          = analyze.1 \
                     batchman.1 \
                     ff_bignet.1 \
                     isnns.1 \
                     linknets.1 \
                     snns.1 \
                     snns2c.1 \
                     td_bignet.1 \
                     xgui.1

# Targets

all:
	@echo "nothing to make"

$(MANDIR):
	-mkdir $(MANDIR)

$(MAN1DIR): $(MANDIR)
	-mkdir $(MAN1DIR)

install-one: $(MAN1DIR)
	for mantarget in $(MAN1FILES) ; do \
            if [ ! -e $(MAN1DIR)/$$mantarget ] ; then \
               $(INSTALL_DATA) man1/$$mantarget $(MAN1DIR) ; \
            fi ; \
        done

install: install-one

install-strip: install

uninstall-one: 
	if [ $(MAN1DIR) != $(SNNSDIR)/man/man1 ] ; then \
            for mantarget in $(MAN1FILES) ; do \
                rm -f $(MAN1DIR)/$$mantarget ; \
            done ; \
        fi

uninstall: uninstall-one

mostlyclean:
	@echo "nothing to clean"

clean: mostlyclean

distclean: clean

maintainer-clean: distclean

depend:
	@echo "nothing to depend on"

#*****************************************************************************
# End of man Makefile
#*****************************************************************************
