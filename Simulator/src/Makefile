LOCAL_DIR = Simulator

include $(QS_BASE_DIR)/Common.mk

EXE_NAME := QueueingSimulator
EXE := $(OBJ_DIR)/$(EXE_NAME)

SRCS := SimulatorMain.cpp \
	CommandLineOptionsProcessor.cpp

OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))

$(EXE): $(OBJS)
	@echo "Linking $(EXE_NAME)"
	@$(LD) $(LDFLAGS) -o $(EXE) $(OBJS)

$(OBJ_DIR)/%.o:%.cpp
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo "Cleaning $(EXE_NAME)"
	@$(RM) $(OBJS) $(EXE)

