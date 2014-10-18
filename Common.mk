PROJECT_DIR=$(QS_BASE_DIR)/$(LOCAL_DIR)
INC_DIR = $(PROJECT_DIR)/inc
OBJ_DIR = $(PROJECT_DIR)/obj
SRC_DIR = $(PROJECT_DIR)/src

MAKEFLAGS := --no-print-directory

CC := g++
CFLAGS := --std=c++11 -g -Wall -I$(INC_DIR)

LD := g++
LDFLAGS := 

RM := /bin/rm -f
