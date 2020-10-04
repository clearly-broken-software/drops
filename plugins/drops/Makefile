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
			widgets/src/TextButton.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk
include ../../sfizz/dpf.mk

# --------------------------------------------------------------
# Extra flags
BUILD_C_FLAGS += $(SFIZZ_C_FLAGS)
BUILD_CXX_FLAGS += $(SFIZZ_CXX_FLAGS)
LINK_FLAGS += $(SFIZZ_LINK_FLAGS)
BASE_FLAGS += -I. -I./widgets
#LINK_FLAGS += $(shell pkg-config --libs sfizz)

# --------------------------------------------------------------
# Enable all possible plugin types


ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif


ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
