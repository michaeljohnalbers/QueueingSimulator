QS_BASE_DIR ?= .

include $(QS_BASE_DIR)/Common.mk

ALL_PROJECTS := $(dir $(shell /usr/bin/find $(QS_BASE_DIR) -name Makefile))

.PHONY: all
all:
	@export target=""; \
	$(MAKE) internal_all

.PHONY: internal_all
internal_all: $(ALL_PROJECTS)

.PHONY: clean
clean:
	@export target=clean; \
	$(MAKE) internal_clean

.PHONY: internal_clean
internal_clean: $(ALL_PROJECTS)

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
