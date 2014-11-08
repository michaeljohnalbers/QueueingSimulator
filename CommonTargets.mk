#************************************************************
# Makefile containing common targets for other Makefiles
#
# Author: Michael Albers
#************************************************************
include $(QS_BASE_DIR)/Common.mk

.PHONY: clean
clean:
	@echo "Cleaning $(notdir $(LOCAL_DIR))"
	@$(RM) $(ALL_OBJS) $(ALL_EXES) $(ALL_LIBS) $(DEPEND_FILE)

.PHONY: depend
depend:
	@echo "Building dependencies for: $(ALL_SRCS)"
	@/bin/cat < /dev/null > $(DEPEND_FILE); \
	 for file in $(ALL_SRCS) ; do \
	  srcDepend=$(SRC_DIR)/$${file}.d; \
	  objFile=`echo $$file | /bin/sed -e 's~\.cpp~.o~'`; \
	  $(CC) $(CFLAGS) -MM -MT $(OBJ_DIR)/$$objFile \
             -MF $$srcDepend $(SRC_DIR)/$$file; \
	  /bin/cat < $$srcDepend >> $(DEPEND_FILE); \
	  $(RM) $$srcDepend; \
	done
