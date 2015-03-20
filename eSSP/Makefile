# -*- mode: Makefile; -*-
# -----------------------------------------
# project kassomat


all: all.targets

all.targets :
	$(MAKE) -C lib
	$(MAKE) -C kassomat

clean :
	cd lib && $(MAKE) clean
	cd kassomat && $(MAKE) clean

.PHONY: all clean distclean
