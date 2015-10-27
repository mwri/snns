#*****************************************************************************
# FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/Makefile,v $
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
# RCS VERSION    : $Revision: 2.12 $
# LAST CHANGE    : $Date: 1998/03/18 15:24:26 $
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
                     -I../../$(XGUIICONSDIR) \
                     $(X_CFLAGS) $(ENZOFLAGS)

# Here you may change the default C-compiler flags as set in
# ../../Makefile.def (not recommended)
# CFLAGS           = -g -O

# -- Source files --

BN3DSOURCES_C      = net_analyzer.c \
                     d3_xUtils.c \
                     d3_global.c \
                     d3_anageo.c \
                     d3_shade.c \
                     d3_lists.c \
                     d3_point.c \
                     d3_dither.c \
                     d3_zgraph.c \
                     d3_fonts.c \
                     d3_graph.c \
                     d3_draw.c \
                     d3_disp.c \
                     d3_move.c \
                     d3_zValue.c \
                     d3_setup.c \
                     d3_units.c \
                     d3_links.c \
                     d3_model.c \
                     d3_project.c \
                     d3_light.c \
                     d3_pannels.c \
                     d3_main.c \
                     bn_bignet.c \
                     bn_basics.c \
                     bn_art1.c \
                     bn_art2.c \
                     bn_artmap.c \
                     bn_TD_bignet.c \
                     bn_kohonen.c \
                     bn_assoz.c \
                     o_graph.c \
                     bn_JordElm.c \
                     ui_class.c \
                     bn_bighop.c

XGUISOURCES1_C     = ui_xWidgets.c \
                     ui_confirmer.c \
                     ui_main.c \
                     ui_mainP.c \
                     ui_status.c \
                     ui_info.c \
                     ui_infoP.c \
                     ui_maspar.c \
                     ui_control.c \
                     ui_controlP.c \
                     ui_file.c \
                     ui_fileP.c \
                     ui_setup.c \
                     ui_setupP.c \
                     ui_xGraphic.c \
                     ui_netGraph.c \
                     ui_utilP.c \
                     ui_netUpdate.c

XGUISOURCES2_C     = ui_event.c \
                     ui_selection.c \
                     ui_action.c \
                     ui_key.c \
                     ui_textP.c \
                     ui_lists.c \
                     ui_layer.c \
                     ui_layerP.c \
                     ui_display.c \
                     ui_edit.c \
                     ui_config.c \
                     ui_color.c \
                     ui_inversion.c \
                     ui_funcdispl.c \
                     ui_displmap.c \
                     ui_displwght.c \
                     ui_print.c \
                     ui_colEdit.c \
                     ui_result.c \
                     cc_main.c \
                     pr_main.c \
                     ui_kohonen.c

# -- Modul files --

MODULES            = $(BN3DSOURCES_C) \
                     $(XGUISOURCES1_C) \
                     $(XGUISOURCES2_C)

# -- Object files --

BN3DOBJECTS        = $(BN3DSOURCES_C:.c=.o)
XGUIOBJECTS1       = $(XGUISOURCES1_C:.c=.o)
XGUIOBJECTS2       = $(XGUISOURCES2_C:.c=.o)

# -- Libraries --

LIBXGUI1	   = libxgui1.a
LIBXGUI2	   = libxgui2.a
BN3DLIB		   = lib3d.a

KERNELLIBS         = ../../$(KERNELSRCDIR)/libkernel.a \
	             ../../$(KERNELSRCDIR)/libfunc.a

KERNELALLLIBS      = $(KERNELLIBS) -lm $(LEXLIB)

# -- Programs --

PROGRAM            = xgui

# -- Targets --

all: $(PROGRAM)

$(XGUIBINDIR):
	-mkdir $(XGUIBINDIR)

install: all $(XGUIBINDIR)
	$(INSTALL_PROGRAM) $(PROGRAM) $(XGUIBINDIR)

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

uninstall:
	rm -f $(XGUIBINDIR)/$(PROGRAM)

mostlyclean:
	rm -f $(XGUIOBJECTS1) $(XGUIOBJECTS2) $(BN3DOBJECTS)

clean: mostlyclean
	rm -f $(PROGRAM) $(LIBXGUI1) $(LIBXGUI2) $(BN3DLIB)

distclean: clean
	rm -f *~ *% *.bak

maintainer-clean: distclean

depend:
	-$(MAKEDEPEND) -- $(CPPFLAGS) -- -w3 -f- $(MODULES) | \
	grep -v ': */' > xgui.ddt

# Building of libraries

$(LIBXGUI1): $(XGUIOBJECTS1)
	$(AR) $(ARFLAGS) $(LIBXGUI1) $(XGUIOBJECTS1)
	-$(RANLIB) $(LIBXGUI1)

$(LIBXGUI2): $(XGUIOBJECTS2)
	$(AR) $(ARFLAGS) $(LIBXGUI2) $(XGUIOBJECTS2)
	-$(RANLIB) $(LIBXGUI2)

$(BN3DLIB): $(BN3DOBJECTS)
	$(AR) $(ARFLAGS) $(BN3DLIB) $(BN3DOBJECTS)
	-$(RANLIB) $(BN3DLIB)

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

# Linking of xgui

xgui: $(LIBXGUI1) $(LIBXGUI2) $(BN3DLIB) $(KERNELLIBS)
	$(CC) $(LIBXGUI1) $(LIBXGUI2) $(BN3DLIB) \
	$(X_LIBS) -lXaw3d -lXmu -lXt -lXext $(X_PRE_LIBS) -lX11 $(X_EXTRA_LIBS) \
	$(KERNELALLLIBS) -o xgui 

# Dependencies

$(XGUIOBJECTS1): Makefile ../../Makefile.def
$(XGUIOBJECTS2): Makefile ../../Makefile.def
$(BN3DOBJECTS):  Makefile ../../Makefile.def

include xgui.ddt

#*****************************************************************************
# End of xgui Makefile
#*****************************************************************************
