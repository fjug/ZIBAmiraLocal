#! gmake

include ../GNUmakefile.setroot

TARGET_DIRS = $(shell /bin/sh $(AMIRA_ROOT)/make/findtargets.sh \
			PACKAGE LIBS $(wildcard */GNUmakefile */*/GNUmakefile */*/*/GNUmakefile */*/*/*/GNUmakefile))

include $(AMIRA_ROOT)/make/multiple.rules
