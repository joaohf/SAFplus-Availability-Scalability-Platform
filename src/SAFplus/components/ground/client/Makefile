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
################################################################################
# ModuleName  : ground
# File        : Makefile
################################################################################
# Description :
#
# Makefile for dummy component client library
#
# This Makefile assumes the following:
#	- CLOVIS_ROOT environment variable is specified properly
#	- Entire source tree under $(CLOVIS_ROOT)/SAFplus is checked out
#
#
################################################################################

# Component name (using all lowercase):

COMPNAME	:= ground

# List local source files needed for the component server:

SRC_FILES	:= clGroundAlarm.c clGroundBuffer.c clGroundCli.c clGroundCm.c \
		   clGroundCor.c clGroundDbal.c clGroundDiag.c clGroundEo.c clGroundGms.c \
		   clGroundHal.c clGroundHpi.c clGroundIoc.c clGroundLog.c clGroundName.c \
		   clGroundOm.c clGroundOsal.c clGroundRmd.c clGroundTimer.c clGroundTrace.c \
		   clGroundTxn.c clGroundDebug.c clGroundOampRt.c clGroundProv.c clGroundMem.c \
           clGroundOMAlarmStruct.c clGroundProvTxnStruct.c clGroundMso.c clGroundPM.c clGroundCustom.c

# List other SAFplus components of which client libraries your component
# depends on:

DEP_COMP_LIST	:= om cor cnt ioc osal timer heap utils buffer alarm mso prov rmd pm event txn

# Library name:

LIB_NAMES	= libClGroundClient

# If there are more than one client libraries to be built here, you
# need to list the object files that will make up each library:
#
# Example:
#     libClTmpcomp_basic: file1.o file2.o
#     libClTmpcomp_advanced: file1.o file2.o file3.o file4.o
#
# If your component builds one library only, than this list will automatically
# obtained from the SRC_FILES variable above, so you don't need to create it.
#
#
#
CPPFLAGS   += -I$(CLOVIS_ROOT)/SAFplus/components/eo/include

include $(CLOVIS_ROOT)/SAFplus/mk/make-client.mk
