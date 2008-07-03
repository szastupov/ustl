bvt/SRCS	:= $(wildcard bvt/bvt*.cc)
bvt/BVTS	:= $(bvt/SRCS:.cc=)
bvt/OBJS	:= $(addprefix $O,$(bvt/SRCS:.cc=.o))
bvt/LIBS	:= -L$(abspath $O.) -l${LIBNAME}
ifdef BUILD_SHARED
bvt/LIBS	:= -Wl,--rpath=$(abspath $O.) ${bvt/LIBS}
endif
ifdef NOLIBSTDCPP
bvt/LIBS	+= ${STAL_LIBS} -lm
endif
CXXFLAGS	+= -I.

.PHONY:	bvt/all bvt/run bvt/clean

bvt/all:	${bvt/BVTS}

bvt/run:	${bvt/BVTS}
	@echo "Running build verification tests:"
	@for i in ${bvt/BVTS}; do \
	    echo "Running $$i"; \
	    ./$$i < $$i.cc &> $$i.out; \
	    diff $$i.std $$i.out && rm -f $$i.out; \
	done

${bvt/BVTS}: bvt/bvt%: $Obvt/bvt%.o $Obvt/stdtest.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $< $Obvt/stdtest.o ${bvt/LIBS}

bvt/bench:	$Obvt/bench.o
	@echo "Linking $@ ..."
	${LD} ${LDFLAGS} -o $@ $< ${bvt/LIBS}

bvt/clean:
	@rm -f ${bvt/BVTS} ${bvt/OBJS} $(bvt/OBJS:.o=.d) bvt/bench $Obvt/bench.o $Obvt/stdtest.o $Obvt/bench.d $Obvt/stdtest.d
	@rmdir $O/bvt &> /dev/null || true

$Obvt/stdtest.o $Obvt/bench.o: bvt/Module.mk
