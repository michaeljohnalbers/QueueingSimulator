#************************************************************
# Makefile for InputGenerator tool.
#
# Author: Michael Albers
#************************************************************
LOCAL_DIR = tools/InputGenerator

include $(QS_BASE_DIR)/Common.mk

EXE_NAME := InputGenerator
EXE := $(OBJ_DIR)/$(EXE_NAME)

SRCS := InputGenerator.cpp

OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))

ALL_EXES := $(EXE)
ALL_OBJS := $(OBJS)
AlL_SRCS := $(SRCS)

$(EXE): $(OBJS)
	@echo "Linking $(EXE_NAME)"
	@$(LD) $(LDFLAGS) -o $(EXE) $(OBJS) \
         -L$(SIMULATOR_LIB_DIR) -l$(ACTOR_LIB_NAME)

$(OBJS): $(SRCS)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $<

include $(TARGETS_FILE)
-include $(DEPEND_FILE)
