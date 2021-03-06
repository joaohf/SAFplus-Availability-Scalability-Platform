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
# ModuleName  : txn
# File        : Makefile
################################################################################
# Description :
#
# Makefile for transaction component client library
#
# This Makefile assumes the following:
#	- CLOVIS_ROOT environment variable is specified properly
#	- Entire source tree under $(CLOVIS_ROOT)/SAFplus is checked out
#
#
################################################################################

# Component name (using all lowercase):

COMPNAME	:= txn

# List local source files needed for the component server:

TXN_PATH        = $(CLOVIS_ROOT)/SAFplus/components/txn

SRC_FILES	:=  clTxnApi.c                                      \
                    clTxnClientImpl.c                               \
                    clTxnAgent.c                                    \
                    clTxnJobProcess.c                               \
                    $(TXN_PATH)/common/clTxnCommonUtil.c            \
                    $(TXN_PATH)/common/clTxnStreamIpi.c             \
                    $(TXN_PATH)/common/clTxnDb.c                    \
                    $(TXN_PATH)/common/clTxnCommIfc.c               \
					clTxnAgentRegister.c							\
                    $(wildcard $(TXN_PATH)/common/idl/xdr/*.c)      \

# List other SAFplus components of which client libraries your component
# depends on:

DEP_COMP_LIST	:= osal timer buffer rmd eo ioc cnt utils debug sm  amf ckpt log

EXTRA_CFLAGS := -I$(TXN_PATH)/common -I$(TXN_PATH)/common/idl/xdr -I$(TXN_PATH)/server

# Library name:

LIB_NAMES	= libClTxnClient  \
                  libClTxnAgent

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

SRC_FILES_libClTxnClient  = clTxnApi.c                                      \
                            clTxnClientImpl.c                               \
                            $(TXN_PATH)/common/clTxnCommonUtil.c            \
                            $(TXN_PATH)/common/clTxnStreamIpi.c             \
                            $(TXN_PATH)/common/clTxnDb.c                    \
                            $(TXN_PATH)/common/clTxnCommIfc.c               \
                            $(wildcard $(TXN_PATH)/common/idl/xdr/*.c)      \

SRC_FILES_libClTxnAgent   = clTxnAgent.c                                    \
                            clTxnJobProcess.c                               \
                            $(TXN_PATH)/common/clTxnCommonUtil.c            \
                            $(TXN_PATH)/common/clTxnStreamIpi.c             \
                            $(TXN_PATH)/common/clTxnDb.c                    \
                            $(TXN_PATH)/common/clTxnCommIfc.c               \
							clTxnAgentRegister.c							\
                            $(wildcard $(TXN_PATH)/common/idl/xdr/*.c)      \

include $(CLOVIS_ROOT)/SAFplus/mk/make-client.mk
