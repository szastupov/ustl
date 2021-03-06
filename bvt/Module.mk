################ Source files ##########################################

bvt/SRCS	:= $(wildcard bvt/bvt*.cc)
bvt/BVTS	:= $(bvt/SRCS:.cc=)
bvt/OBJS	:= $(addprefix $O,$(bvt/SRCS:.cc=.o))
bvt/LIBS	:= -L$(abspath $O.) -l${NAME}
ifdef BUILD_SHARED
bvt/LIBS	:= -Wl,--rpath=$(abspath $O.) ${bvt/LIBS}
endif
ifdef NOLIBSTDCPP
bvt/LIBS	+= ${STAL_LIBS} -lm
endif
CXXFLAGS	+= -I.

################ Compilation ###########################################

.PHONY:	bvt/all bvt/run bvt/clean bvt/check

bvt/all:	${bvt/BVTS}

# The correct output of a bvt is stored in bvtXX.std
# When the bvt runs, its output is compared to .std
#
bvt/run:	${bvt/BVTS}
	@echo "Running build verification tests:"
	@for i in ${bvt/BVTS}; do \
	    echo "Running $$i"; \
	    ./$$i < $$i.cc &> $$i.out; \
	    diff $$i.std $$i.out && rm -f $$i.out; \
	done

${bvt/BVTS}: bvt/%: $Obvt/%.o $Obvt/stdtest.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $< $Obvt/stdtest.o ${bvt/LIBS}

bvt/bench:	$Obvt/bench.o $Obvt/stdtest.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $< $Obvt/stdtest.o ${bvt/LIBS}

################ Maintenance ###########################################

clean:	bvt/clean
bvt/clean:
	@rm -f ${bvt/BVTS} ${bvt/OBJS} $(bvt/OBJS:.o=.d) bvt/bench $Obvt/bench.o $Obvt/stdtest.o $Obvt/bench.d $Obvt/stdtest.d
	@rmdir $O/bvt &> /dev/null || true

check:		bvt/run
bvt/check:	check

${bvt/OBJS} $Obvt/stdtest.o $Obvt/bench.o: Makefile bvt/Module.mk Config.mk ${NAME}/config.h

-include ${bvt/OBJS:.o=.d} $Obvt/stdtest.d
