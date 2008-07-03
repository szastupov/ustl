include Config.mk

################ Source files ##########################################

SRCS	:= $(wildcard *.cc)
INCS	:= $(wildcard *.h)
OBJS	:= $(SRCS:.cc=.o)
DOCT	:= ustldoc.in

################ Compilation ###########################################

.PHONY: all clean depend dox html check dist distclean maintainer-clean
ifdef BUILD_SHARED
all:	${LIBSOBLD}

${LIBSOBLD}:	${OBJS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} ${SHBLDFL} -o $@ $^ ${LIBS}
endif
ifdef BUILD_STATIC
all:	${LIBA}

${LIBA}:	${OBJS}
	@echo "Linking $@ ..."
	@${AR} r $@ $?
	@${RANLIB} $@
endif

%.o:	%.cc
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

html:
dox:
	@${DOXYGEN} ${DOCT}

################ Installation ##########################################

.PHONY:	install uninstall install-incs uninstall-incs

LIDIR	:= ${INCDIR}/${LIBNAME}
INCSI	:= $(addprefix ${LIDIR}/,$(filter-out ${LIBNAME}.h,${INCS}))
RINCI	:= ${LIDIR}.h

install:	install-incs
install-incs: ${INCSI} ${RINCI}
${INCSI}: ${LIDIR}/%.h: %.h
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
${RINCI}: ${LIBNAME}.h
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
uninstall-incs:
	@echo "Removing ${LIDIR}/ and ${LIDIR}.h ..."
	@(cd ${INCDIR}; rm -rf ${LIBNAME} ${LIBNAME}.h)

ifdef BUILD_SHARED
.PHONY: install-shared uninstall-shared
LIBTI	:= ${LIBDIR}/${LIBSOBLD}
LIBLI	:= ${LIBDIR}/${LIBSOLNK}
LIBSI	:= ${LIBDIR}/${LIBSO}
install:	${LIBTI} ${LIBLI} ${LIBSI}
${LIBTI}:	${LIBSOBLD}
	@echo "Installing $@ ..."
	@${INSTALLLIB} $< $@
${LIBLI} ${LIBSI}: ${LIBTI}
	@(cd ${LIBDIR}; rm -f $@; ln -s $(notdir $<) $(notdir $@))
endif

ifdef BUILD_STATIC
.PHONY: install-static uninstall-static
LIBAI	:= ${LIBDIR}/${LIBA}
install:	${LIBAI}
${LIBAI}:	${LIBA}
	@echo "Installing $@ ..."
	@${INSTALLLIB} $< $@
endif

uninstall:	uninstall-incs
	@echo "Removing library from ${LIBDIR} ..."
	@rm -f ${LIBTI} ${LIBLI} ${LIBSI} ${LIBAI}

################ Maintenance ###########################################

clean:
	@echo "Removing generated files ..."
	@rm -f ${OBJS} ${LIBSO} ${LIBA} ${LIBSOBLD}
	@+${MAKE} -C bvt clean

depend: ${SRCS}
	@${CXX} ${CXXFLAGS} -MM ${SRCS} > .depend;
	@+${MAKE} -C bvt depend

check:	install
	@echo "Compiling and running build verification tests ..."
	@+${MAKE} -C bvt run

TMPDIR	:= /tmp
DISTDIR	:= ${HOME}/stored
DISTVER	:= ${MAJOR}.${MINOR}
DISTNAM	:= ${LIBNAME}-${DISTVER}
DISTLSM	:= ${DISTNAM}.lsm
DISTTAR	:= ${DISTNAM}.tar.bz2
DDOCTAR	:= ${DISTNAM}-docs.tar.bz2

dist:
	mkdir ${TMPDIR}/${DISTNAM}
	cp -r . ${TMPDIR}/${DISTNAM}
	+${MAKE} -C ${TMPDIR}/${DISTNAM} dox distclean
	(cd ${TMPDIR};							\
	    tar jcf ${DISTDIR}/${DDOCTAR} ${DISTNAM}/docs/html;		\
	    rm -f ${DISTNAM}/.git; rm -rf ${DISTNAM}/docs/html;		\
	    cp ${DISTNAM}/docs/${LIBNAME}.lsm ${DISTDIR}/${DISTLSM};	\
	    tar --numeric-owner --same-owner -jcf ${DISTDIR}/${DISTTAR} ${DISTNAM}; rm -rf ${DISTNAM})

distclean:	clean
	@rm -f Config.mk config.h .depend bvt/.depend

maintainer-clean: distclean
	@rm -rf docs/html

-include .depend
