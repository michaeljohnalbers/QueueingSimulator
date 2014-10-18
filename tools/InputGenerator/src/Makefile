LOCAL_DIR = tools/InputGenerator

include $(QS_BASE_DIR)/Common.mk

EXE_NAME := InputGenerator
EXE := $(OBJ_DIR)/$(EXE_NAME)

SRCS := InputGenerator.cpp

OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))

$(EXE): $(OBJS)
	@echo "Linking $(EXE_NAME)"
	@$(LD) $(LDFLAGS) -o $(EXE) $(OBJS)

$(OBJS): $(SRCS)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo "Cleaning $(EXE_NAME)"
	@$(RM) $(OBJS) $(EXE)
