#************************************************************
# Top level Makefile for Queueing Simulator
#
# Author: Michael Albers
#************************************************************

ifeq ($(QS_BASE_DIR), )
  $(error QueueingSimulator base directory environment variable, QS_BASE_DIR, is not set. Should be directory were this Makefile is located. )
endif

include $(QS_BASE_DIR)/Common.mk

ALL_PROJECTS := $(dir $(shell /usr/bin/find $(QS_BASE_DIR) -name Makefile))

.PHONY: all
all:
	@export target=""; \
	$(MAKE) internal_all

.PHONY: clean
clean:
	@export target=clean; \
	$(MAKE) internal_clean

.PHONY: depend
depend:
	@export target=depend; \
	$(MAKE) internal_depend

.PHONY: internal_all
internal_all: $(ALL_PROJECTS)

.PHONY: internal_clean
internal_clean: $(ALL_PROJECTS)

.PHONY: internal_depend
internal_depend: $(ALL_PROJECTS)

test:
	@echo "$(ALL_PROJECTS)"

$(ALL_PROJECTS): .FORCE
	@if [ "$@" != "$(QS_BASE_DIR)/" ] ; then \
	   echo "*** Current Directory: $@ ***"; \
	   $(MAKE) -C $@ $$target; \
	   echo ""; \
	 fi

.PHONY: .FORCE
.FORCE:
