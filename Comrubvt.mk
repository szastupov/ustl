# Targets from building build verification tests.
#
# Define BVTS variable containing names of all tests (no extensions)
# LIBS variable should list required libraries.
#

CXXFLAGS	+= -I..
LDFLAGS		+= -L..
TOCLEAN		+= ${BVTS}

all:	${BVTS}

run:	${BVTS}
	@for i in ${BVTS}; do echo "Running $$i"; ./$$i | diff $$i.std -; done

bvt%:	bvt%.o
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $^ ${LIBS}

include Comrudir.mk
-include .depend
 
