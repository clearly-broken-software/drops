#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = drops

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
			DropsPlugin.cpp

FILES_UI  = \
			DropsUI.cpp \
			DropsGeometry.cpp \
			widgets/src/TextButton.cpp \
			widgets/src/ScrollBar.cpp \
			widgets/src/Knob.cpp \
			widgets/src/DropDown.cpp \
			widgets/src/Menu.cpp \
			widgets/src/Slider.cpp \
			widgets/src/HBox.cpp \
			widgets/src/VBox.cpp \
			widgets/src/FileOpenButton.cpp \
			widgets/src/SVGButton.cpp \
			widgets/src/PopUp.cpp \
			widgets/src/CheckBox.cpp \
			SVGImage.cpp \
			fonts.cpp \
			SVG_Icons.cpp \
			initTabAmp.cpp \
			initTabFilter.cpp \
			initTabPitch.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk
include ../../sfizz/dpf.mk

# --------------------------------------------------------------
# Extra flags
BUILD_C_FLAGS += $(SFIZZ_C_FLAGS)
BUILD_CXX_FLAGS += $(SFIZZ_CXX_FLAGS)
LINK_FLAGS += $(SFIZZ_LINK_FLAGS) $(shell $(PKG_CONFIG) --libs sndfile )
BASE_FLAGS += -I. -I./widgets -I./external/src
#LINK_FLAGS += $(shell pkg-config --libs sfizz)

# --------------------------------------------------------------
# Enable all possible plugin types


ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif


ifeq ($(HAVE_DGL),true)
TARGETS += lv2
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
