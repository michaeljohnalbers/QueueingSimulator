#************************************************************
# Makefile include file containing common variable definitions
# useful for other makefiles.
#
# Author: Michael Albers
#************************************************************
PROJECT_DIR=$(QS_BASE_DIR)/$(LOCAL_DIR)
INC_DIR = $(PROJECT_DIR)/inc
OBJ_DIR = $(PROJECT_DIR)/obj
SRC_DIR = $(PROJECT_DIR)/src
LIB_DIR = $(PROJECT_DIR)/lib
BIN_DIR = $(PROJECT_DIR)/bin

MAKEFLAGS := --no-print-directory

CP := /bin/cp
RM := /bin/rm -f

DEPEND_FILE := $(SRC_DIR)/.dependlist

TARGETS_FILE := $(QS_BASE_DIR)/CommonTargets.mk

SIMULATOR_LIB_DIR := $(QS_BASE_DIR)/Simulator/lib
SIMULATOR_INC_DIR := $(QS_BASE_DIR)/Simulator/inc

ifeq ($(QS_EIGEN_BASE_DIR), )
  $(error Eigen library base directory environment variable, QS_EIGEN_BASE_DIR, is not set. Should be: $$QS_EIGEN_BASE_DIR/eigen)
endif
EIGEN_DIR := $(QS_EIGEN_BASE_DIR)/eigen

INC_DIRS := -I$(INC_DIR) -I$(SIMULATOR_INC_DIR) -I$(EIGEN_DIR)

CC := g++
CFLAGS := --std=c++11 -g -Wall -pthread -fopenmp $(INC_DIRS)

LD := g++
LDFLAGS := -pthread -fopenmp

AR := /usr/bin/ar
ARFLAGS := cr

