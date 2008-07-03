bvt/SRCS	:= $(wildcard bvt/bvt*.cc)
bvt/BVTS	:= $(bvt/SRCS:.cc=)
bvt/OBJS	:= $(bvt/SRCS:.cc=.o)
bvt/LIBS	:= -L. -l${LIBNAME}
ifdef BUILD_SHARED
bvt/LIBS	+= -Wl,--rpath=$(abspath .)
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

${bvt/BVTS}: bvt/bvt%: bvt/bvt%.o bvt/stdtest.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $< bvt/stdtest.o ${bvt/LIBS}

bvt/bench:	bvt/bench.o
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $^ ${bvt/LIBS}

bvt/clean:
	@rm -f ${bvt/BVTS} ${bvt/OBJS} bvt/bench bvt/bench.o bvt/stdtest.o
