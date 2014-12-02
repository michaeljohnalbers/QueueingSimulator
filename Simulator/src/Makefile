#************************************************************
# Makefile for Queueing Simulator.
#
# Author: Michael Albers
#************************************************************
LOCAL_DIR = Simulator

include $(QS_BASE_DIR)/Common.mk

EXE_BASE := QueueingSimulator

EXE_NAME := $(EXE_BASE)
EXE := $(BIN_DIR)/$(EXE_NAME)

OBJ_DIR_SERIAL := $(OBJ_DIR)/serial
EXE_NAME_SERIAL := $(EXE_BASE)_serial
EXE_SERIAL := $(BIN_DIR)/$(EXE_NAME_SERIAL)

EXE_SRCS := Arrive.cpp \
            Bucket.cpp \
            CollisionAvoidance.cpp \
            EigenHelper.cpp \
            Evade.cpp \
            Exit.cpp \
            Flee.cpp \
            Individual.cpp \
            Lattice.cpp \
            Logger.cpp \
            NearestN.cpp \
            RankedLeaderFollow.cpp \
            ReBucketer.cpp \
            Seek.cpp \
            SimulatorMain.cpp

OBJS := $(addprefix $(OBJ_DIR)/, $(EXE_SRCS:%.cpp=%.o))
OBJS_SERIAL := $(addprefix $(OBJ_DIR_SERIAL)/, $(EXE_SRCS:%.cpp=%.o))

ALL_EXES := $(EXE) $(EXE_SERIAL)
ALL_OBJS := $(LIB_OBJS) $(OBJS) $(OBJS_SERIAL)
ALL_SRCS := $(LIB_SRCS) $(EXE_SRCS)

all: $(EXE) $(EXE_SERIAL)

$(EXE): $(OBJS)
	@echo "Linking $(EXE_NAME)"
	@$(LD) $(LDFLAGS) -o $(EXE) $(OBJS)
	@echo "" # Just for spacing in the console

$(EXE_SERIAL): $(OBJS_SERIAL)
	@echo "Linking $(EXE_NAME_SERIAL)"
	@$(LD) $(LDFLAGS) -o $(EXE_SERIAL) $(OBJS_SERIAL)

$(OBJ_DIR)/%.o:%.cpp
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR_SERIAL)/%.o:%.cpp
	@echo "Compiling $< (serial)"
	@$(CC) $(CFLAGS) -DSERIAL -o $@ -c $<

include $(TARGETS_FILE)
-include $(DEPEND_FILE)
