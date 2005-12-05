include Config.mk

################ Source files ##########################################

SRCS	= $(wildcard *.cc)
INCS	= $(filter-out bsconf.%,$(wildcard *.h)) ustl.tbff
OBJS	= $(SRCS:.cc=.o)
DOCT	= ustldoc.in

################ Library link names ####################################

LIBA	= lib${LIBNAME}.a
LIBSO	= lib${LIBNAME}.so
ifdef MAJOR
LIBSOLNK= ${LIBSO}.${MAJOR}.${MINOR}
LIBSOBLD= ${LIBSO}.${MAJOR}.${MINOR}.${BUILD}
endif
TOCLEAN	+= ${LIBSO} ${LIBA} ${LIBSOBLD}

################ Compilation ###########################################

%.o:	%.cc
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

%.h.gch:	%.h
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -o $@ -c $<

gch:	${INCDIR}/${LIBNAME}.h.gch
${INCDIR}/${LIBNAME}.h.gch:	${INCS}
	@echo "    Creating precompiled header ..."
	@${CXX} ${CXXFLAGS} -o $@ -c ${INCDIR}/${LIBNAME}.h

${LIBA}:	${OBJS}
	@echo "Linking $@ ..."
	@${AR} r $@ $?
	@${RANLIB} $@

${LIBSOBLD}:	${OBJS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} ${SHBLDFL} -o $@ $^ ${LIBS}

html:
dox:
	@${DOXYGEN} ${DOCT}

################ Installation ##########################################

.PHONY:	all install uninstall install-incs uninstall-inst

ifdef BUILD_SHARED
ifdef BUILD_STATIC
all:		${LIBA} ${LIBSOBLD}
install:	install-static install-shared
uninstall:	uninstall-static uninstall-shared
else
all:		${LIBSOBLD}
install:	install-shared
uninstall:	uninstall-shared
endif
else
all:		${LIBA}
install:	install-static
uninstall:	uninstall-static
endif

.PHONY: install-static install-shared uninstall-static uninstall-shared
.PHONY:	gch clean depend dox html check dist distclean maintainer-clean

install-shared: ${LIBSOBLD} install-incs
	@echo "Installing ${LIBSOBLD} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBSOBLD} ${LIBDIR}
	@(cd ${LIBDIR}; \
	    rm -f ${LIBSO} ${LIBSOLNK}; \
	    ln -sf ${LIBSOBLD} ${LIBSO}; \
	    ln -sf ${LIBSOBLD} ${LIBSOLNK})

uninstall-shared: uninstall-incs
	@echo "Removing ${LIBSOBLD} from ${LIBDIR} ..."
	@(cd ${LIBDIR}; \
	    rm -f ${LIBSO} ${LIBSOLNK} ${LIBSOBLD})

install-static: ${LIBA} install-incs
	@echo "Installing ${LIBA} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBA} ${LIBDIR}

uninstall-static: uninstall-incs
	@echo "Removing ${LIBA} from ${LIBDIR} ..."
	@rm -f ${LIBDIR}/${LIBA}

install-incs: ${INCS}
	@echo "Installing headers to ${INCDIR} ..."
	@${INSTALLDIR} ${INCDIR}/${LIBNAME}
	@for i in $(filter-out ${LIBNAME}.h,${INCS}); do	\
	    ${INSTALLDATA} $$i ${INCDIR}/${LIBNAME}/$$i;	\
	done;
	@${INSTALLDATA} ${LIBNAME}.h ${INCDIR}

uninstall-incs:
	@echo "Removing headers from ${INCDIR} ..."
	@rm -rf ${INCDIR}/${LIBNAME} ${INCDIR}/${LIBNAME}.h

################ Maintenance ###########################################

clean:
	@echo "Removing generated files ..."
	@rm -f ${OBJS} ${TOCLEAN} *.rpo
	@+${MAKE} -C bvt clean

depend: ${SRCS}
	@${CXX} ${CXXFLAGS} -M ${SRCS} > .depend;
	@+${MAKE} -C bvt depend

check:	install
	@echo "Compiling and running build verification tests ..."
	@+${MAKE} -C bvt run

TMPDIR	= /tmp
DISTDIR	= ${HOME}/stored
DISTNAM	= ${LIBNAME}-${MAJOR}.${MINOR}
DISTTAR	= ${DISTNAM}-${BUILD}.tar.bz2
DDOCTAR	= ${LIBNAME}-docs-${MAJOR}.${MINOR}-${BUILD}.tar.bz2

dist:
	mkdir ${TMPDIR}/${DISTNAM}
	cp -r . ${TMPDIR}/${DISTNAM}
	+${MAKE} -C ${TMPDIR}/${DISTNAM} dox dist-clean
	(cd ${TMPDIR}; tar jcf ${DISTDIR}/${DDOCTAR} ${DISTNAM}/docs/html)
	(cd ${TMPDIR}/${DISTNAM}; rm -rf CVS; cd bvt; rm -rf CVS; cd ../docs; rm -rf CVS html)
	(cd ${TMPDIR}; tar jcf ${DISTDIR}/${DISTTAR} ${DISTNAM}; rm -rf ${DISTNAM})

distclean:	clean
	@rm -f Common.mk config.h ${LIBNAME}.spec bsconf.o bsconf .depend bvt/.depend

maintainer-clean: distclean
	@rm -rf docs/html

-include .depend
 
