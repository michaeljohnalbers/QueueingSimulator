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
	$(MAKE) internal_clean; \
	$(MAKE) doc_clean

.PHONY: depend
depend:
	@export target=depend; \
	$(MAKE) internal_depend

.PHONY: doc
doc:
	@echo "Generating documentation..."
	@cd $(QS_BASE_DIR)/doc; \
	doxygen ./Doxyfile; \
	cd latex; \
	$(MAKE) all; \
	$(CP) refman.pdf $(QS_BASE_DIR)/doc/QueueingSimulator.pdf

.PHONY: doc_clean
doc_clean:
	@cd $(QS_BASE_DIR)/doc; \
	$(RM) -rf html latex QueueingSimulator.pdf

.PHONY: help
help:
	@echo "Help Information for building QueueingSimulator"
	@echo "-----------------------------------------------"
	@echo "Targets: "
	@echo "  all - Builds the entire package"
	@echo "  clean - Cleans the entire package"
	@echo "  depend - Builds depend lists for the package"
	@echo "  doc - Builds Doxygen documentation for package"
	@echo "        (Doxygen, TeX and dot need to be installed.)"
	@echo "  doc_clean - Cleans up generated documentation."

.PHONY: internal_all
internal_all: $(ALL_PROJECTS)

.PHONY: internal_clean
internal_clean: $(ALL_PROJECTS)

.PHONY: internal_depend
internal_depend: $(ALL_PROJECTS)

$(ALL_PROJECTS): .FORCE
	@if [ "$@" != "$(QS_BASE_DIR)/" ] ; then \
	   echo "*** Current Directory: $@ ***"; \
	   $(MAKE) -C $@ $$target; \
	   echo ""; \
	 fi

.PHONY: .FORCE
.FORCE:
