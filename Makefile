-include Config.mk

################ Source files ##########################################

SRCS	:= $(wildcard *.cc)
OBJS	:= $(addprefix $O,$(SRCS:.cc=.o))

################ Compilation ###########################################

.PHONY: all clean html check dist distclean maintainer-clean

all:	Config.mk config.h
ALLTGTS	:= Config.mk config.h

$O%.o:	%.cc
	@echo "    Compiling $< ..."
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@${CXX} ${CXXFLAGS} -MMD -MT "$(<:.cc=.s) $@" -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

include bvt/Module.mk

################ Installation ##########################################

.PHONY:	install uninstall install-incs uninstall-incs

################ Maintenance ###########################################

clean:	bvt/clean
	@rm -f ${OBJS} $(OBJS:.o=.d)
	@rmdir $O &> /dev/null || true

check:	bvt/run

html:
	@${DOXYGEN} ${NAME}doc.in

ifdef MAJOR
DISTVER	:= ${MAJOR}.${MINOR}
DISTNAM	:= ${NAME}-${DISTVER}
DISTLSM	:= ${DISTNAM}.lsm
DISTTAR	:= ${DISTNAM}.tar.bz2

dist:
	@echo "Generating ${DISTTAR} and ${DISTLSM} ..."
	@mkdir .${DISTNAM}
	@rm -f ${DISTTAR}
	@cp -r * .${DISTNAM} && mv .${DISTNAM} ${DISTNAM}
	@+${MAKE} -sC ${DISTNAM} maintainer-clean
	@tar jcf ${DISTTAR} ${DISTNAM} && rm -rf ${DISTNAM}
	@echo "s/@version@/${DISTVER}/" > ${DISTLSM}.sed
	@echo "s/@date@/`date +%F`/" >> ${DISTLSM}.sed
	@echo -n "s/@disttar@/`du -h --apparent-size ${DISTTAR}`/" >> ${DISTLSM}.sed;
	@sed -f ${DISTLSM}.sed docs/${NAME}.lsm > ${DISTLSM} && rm -f ${DISTLSM}.sed
endif

distclean:	clean
	@rm -f Config.mk config.h config.status

maintainer-clean: distclean
	@if [ -d docs/html ]; then rm -f docs/html/*; rmdir docs/html; fi

${OBJS} ${bvt/OBJS}:	Makefile Config.mk config.h
${bvt/OBJS}:		bvt/Module.mk
Config.mk:		Config.mk.in
config.h:		config.h.in
Config.mk config.h:	configure
	@if [ -x config.status ]; then echo "Reconfiguring ..."; ./config.status; \
	else echo "Running configure ..."; ./configure; fi

-include ${OBJS:.o=.d} ${bvt/OBJS:.o=.d}
