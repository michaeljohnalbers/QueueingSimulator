#************************************************************
# Makefile for Queueing Simulator.
#
# Author: Michael Albers
#************************************************************
LOCAL_DIR = Simulator

include $(QS_BASE_DIR)/Common.mk

QS_SERIAL := 

EXE_NAME := QueueingSimulator
EXE := $(OBJ_DIR)/$(EXE_NAME)

LIB_NAME := 
LIB := 

LIB_SRCS := 
LIB_OBJS := $(addprefix $(OBJ_DIR)/, $(LIB_SRCS:%.cpp=%.o))

EXE_SRCS := Bucket.cpp \
            CollisionAvoidance.cpp \
            EigenHelper.cpp \
            Exit.cpp \
            Individual.cpp \
            Lattice.cpp \
            Logger.cpp \
            NearestN.cpp \
            ReBucketer.cpp \
            Seek.cpp \
            SimulatorMain.cpp

OBJS := $(addprefix $(OBJ_DIR)/, $(EXE_SRCS:%.cpp=%.o))

ALL_EXES := $(EXE)
ALL_LIBS := $(LIB)
ALL_OBJS := $(LIB_OBJS) $(OBJS)
ALL_SRCS := $(LIB_SRCS) $(EXE_SRCS)

all: $(LIB) $(EXE)

$(EXE): $(OBJS)
	@echo "Linking $(EXE_NAME)"
	@$(LD) $(LDFLAGS) -o $(EXE) $(OBJS)

$(OBJ_DIR)/%.o:%.cpp
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(QS_SERIAL) -o $@ -c $<

$(LIB): $(LIB_OBJS)
	@echo "Building library $(LIB_NAME)"
	@$(AR) $(ARFLAGS) $(LIB) $(LIB_OBJS)

include $(TARGETS_FILE)
-include $(DEPEND_FILE)
