include ../Common.mk

BVTS	= bvt00 bvt01 bvt02 bvt03 bvt04 bvt05 bvt06 bvt07 bvt08 bvt09 bvt10 \
	bvt11 bvt12 bvt13 bvt14 bvt15 bvt16 bvt17 bvt18 bvt19 bvt20 bvt21 \
	bvt22
OBJS	= bvt00.o bvt01.o bvt02.o bvt03.o bvt04.o bvt05.o bvt06.o bvt07.o \
	bvt08.o bvt09.o bvt10.o bvt11.o bvt12.o bvt13.o bvt14.o bvt15.o \
	bvt16.o bvt17.o bvt18.o bvt19.o bvt20.o bvt21.o bvt22.o
SRCS	= bvt00.cc bvt01.cc bvt02.cc bvt03.cc bvt04.cc bvt05.cc bvt06.cc \
	bvt07.cc bvt08.cc bvt09.cc bvt10.cc bvt11.cc bvt12.cc bvt13.cc \
	bvt14.cc bvt15.cc bvt16.cc bvt17.cc bvt18.cc bvt19.cc bvt20.cc \
	bvt21.cc bvt22.cc bench.cc
LIBS	+= -l${LIBNAME}

CXXFLAGS	+= -I ${INCDIR}
LDFLAGS		+= -L ${LIBDIR}
TOCLEAN		+= ${BVTS}

all:	${BVTS} bench

run:	${BVTS}
	@for i in ${BVTS}; do echo "Running $$i"; ./$$i < $$i.cc | diff $$i.std -; done

bvt%:	bvt%.o
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $^ ${LIBS}

bench:	bench.o
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $^ ${LIBS}

%.o:	%.cc
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

clean:
	@echo "Removing generated files ..."
	@${RM} -f ${OBJS} ${TOCLEAN} *.rpo

depend: ${SRCS}
	@${CXX} ${CXXFLAGS} -M ${SRCS} > .depend

-include .depend
 
