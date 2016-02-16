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

ALL_EXES := $(EXE)
ALL_OBJS := $(LIB_OBJS) $(OBJS)
ALL_SRCS := $(LIB_SRCS) $(EXE_SRCS)

all: $(EXE)

$(EXE): $(OBJS)
	@echo "Linking $(EXE_NAME)"
	@$(LD) $(LDFLAGS) -o $(EXE) $(OBJS)

$(OBJ_DIR)/%.o:%.cpp
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(QS_SERIAL) -o $@ -c $<

include $(TARGETS_FILE)
-include $(DEPEND_FILE)
