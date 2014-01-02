# Copyright (C) 2002-2012 OpenClovis Solutions Inc.  All Rights Reserved.
# This file is available  under  a  commercial  license  from  the
# copyright  holder or the GNU General Public License Version 2.0.
#
# The source code for  this program is not published  or otherwise 
# divested of  its trade secrets, irrespective  of  what  has been 
# deposited with the U.S. Copyright office.
# 
# This program is distributed in the  hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied  warranty  of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
# General Public License for more details.
# 
# For more  information,  see the  file COPYING provided with this
# material.


# This file is included at the top of all other makefiles
# It discovers the environment and sets standard variables.

ifdef S7  # SAFplus v7

SAFPLUS_TOOLCHAIN_DIR := /opt/clovis/6.1/buildtools/local

MAKE_DIR := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
SAFPLUS_SRC_DIR ?= $(dir $(MAKE_DIR)../../)
SAFPLUS_INC_DIR ?= $(SAFPLUS_SRC_DIR)/SAFplus/include7
BOOST_DIR ?= $(SAFPLUS_SRC_DIR)../../boost
CPP_FLAGS += -I$(SAFPLUS_SRC_DIR)/SAFplus/include -I$(SAFPLUS_INC_DIR)
CPP_FLAGS += -I$(BOOST_DIR)  -I.
COMPILE_CPP = g++ -g -O0 -fPIC -c $(CPP_FLAGS) -o
LINK_SO     = g++ -g -shared -o 
LINK = g++ -g -O0 -fPIC $(LINK_FLAGS) -o $@

LINK_LIBS ?=
LINK_STD_LIBS += $(SAFPLUS_TOOLCHAIN_DIR)/lib/libboost_system.a -lpthread -lrt

endif

# If the chassis manager directory is not defined go look for it

# Pull in the SAFplus prebuild or the customer model's definitions
ifdef SAFPLUS_MODEL_DIR
include $(SAFPLUS_MODEL_DIR)/build/$(TOOLCHAIN_DIR)/mk/define.mk
else # If no model is defined, attempt to find the prebuild area
thisdir := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
prebuildCandidate := $(abspath $(thisdir)/../../../prebuild/asp/build/local/mk)
defineMk := $(wildcard $(prebuildCandidate)/define.mk)
ifeq ($(defineMk),) # Nope (we must be running from source control source), so check the standard install location
defineMk := $(wildcard /opt/clovis/*/sdk/prebuild/asp/build/local/mk/define.mk)
endif
ifeq ($(defineMk),)
$(error To identify the model (or the SAFplus build), please set SAFPLUS_MODEL_DIR to your model base directory)
else
include $(defineMk)
endif
endif

ifndef CM_DIR

BUILDTOOLS_GLIB = $(wildcard $(TOOLCHAIN_DIR)/include/glib-2.0)

ifneq ($(BUILDTOOLS_GLIB),)
GLIB_INC = -I$(TOOLCHAIN_DIR)/include/glib-2.0 -I$(TOOLCHAIN_DIR)/lib/glib-2.0/include
else

GLIB_INC = -I/usr/include/glib-2.0 -I$(wildcard /usr/lib/*/glib-2.0/include/)
endif

# Possible CM directories
CM_SEARCH_PATH := $(CLOVIS_ROOT)/../PSP/src/cm $(CLOVIS_ROOT)/../../PSP/src/cm

USING_CM := $(wildcard $(CM_SEARCH_PATH))
ifneq ($(USING_CM),)

ifeq ($(HCL_CM),1)
        $(warning Using the actual chassis manager from the Platform Support Package located at $(USING_CM) and Radisys HPI)
	HPI_LIBS = -L$(TOOLCHAIN_DIR)/lib -lhcl -lopenhpiutils
	HPI_CFLAGS = -I$(TOOLCHAIN_DIR)/include/openhpi -I$(TOOLCHAIN_DIR)/include/radisys $(OPENHPICFLAGS)
else
ifeq ($(HPI_EMERSON),1)
        $(warning Using the actual chassis manager from the Platform Support Package located at $(USING_CM) and Emerson HPI)
	HPI_LIBS = -L$(TOOLCHAIN_DIR)/emerson/lib -lbbs-hpibmultishelf -lbbs-hpibcommon -lbbs-hpibutils
	HPI_CFLAGS = -I$(TOOLCHAIN_DIR)/emerson/include $(GLIB_INC)

else
        $(warning Using the actual chassis manager from the Platform Support Package located at $(USING_CM) and OpenHPI)
	HPI_LIBS = $(OPENHPILIBS)
	HPI_CFLAGS += $(OPENHPICFLAGS)
endif
endif

CM_DIR      := $(realpath $(USING_CM))

# The chassis mgr uses hpi data structures so I need the HPI includes but not the library
CL_CM       := -lClCm
CM_CFLAGS   := -DCL_USE_CHASSIS_MANAGER $(HPI_CFLAGS) -I$(CM_DIR)/include

CM_COMP_DEP := cm/client
CM_CONFLICT_RESOLVE_OBJS :=

else
HPI_LIBS :=
HPI_CFLAGS :=
CM_DIR :=
CL_CM  :=
CM_CFLAGS :=
$(warning Not using the chassis manager)
endif

endif
