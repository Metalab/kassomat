# -*- mode: Makefile; -*-
# -----------------------------------------
# project kassomat


all: all.targets

run: all.targets
	$(MAKE) -C kassomat run

all.targets :
	$(MAKE) -C lib
	$(MAKE) -C ../hiredis
	$(MAKE) -C kassomat

clean :
	cd lib && $(MAKE) clean
	cd ../hiredis && $(MAKE) clean
	cd kassomat && $(MAKE) clean

.PHONY: all clean distclean
