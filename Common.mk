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

RM := /bin/rm -f

DEPEND_FILE := $(SRC_DIR)/.dependlist

TARGETS_FILE := $(QS_BASE_DIR)/CommonTargets.mk

SIMULATOR_LIB_DIR := $(QS_BASE_DIR)/Simulator/lib
SIMULATOR_INC_DIR := $(QS_BASE_DIR)/Simulator/inc

ACTOR_LIB_NAME := QS_Actor
ACTOR_LIB := $(SIMULATOR_LIB_DIR)/lib$(ACTOR_LIB_NAME).a

CC := g++
CFLAGS := --std=c++11 -g -Wall -I$(INC_DIR) -I$(SIMULATOR_INC_DIR)

LD := g++
LDFLAGS := 

AR := /usr/bin/ar
ARFLAGS := cr

