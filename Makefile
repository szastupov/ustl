include Common.mk

SRCS	= cmemlink.cc memlink.cc memblock.cc mstream.cc ustring.cc ubitset.cc \
	sistream.cc sostream.cc fdostream.cc uexception.cc ustdxept.cc
OBJS	= cmemlink.o memlink.o memblock.o mstream.o ustring.o sistream.o \
	sostream.o fdostream.o ubitset.o uexception.o ustdxept.o
INCS	= cmemlink.h memblock.h memlink.h mistream.h mostream.h ustdxept.h \
	ualgo.h ustl.h ustring.h utypes.h uutility.h uvector.h ualgobase.h \
	uctralgo.h ufunction.h upair.h uiterator.h ustack.h uqueue.h \
	uexception.h strmsize.h sistream.h sostream.h ulimits.h uset.h \
	umultiset.h uspecial.h uios.h fdostream.h umap.h umultimap.h \
	umemory.h uiosfunc.h utf8.h config.h ubitset.h unumeric.h utuple.h \
	umatrix.h ulaalgo.h ulocale.h ufacets.h ulist.h upredalgo.h ustl.tbff
DOCT	= ustldoc.in

ifdef STANDALONE
SRCS	+= unew.cc ulocale.cc ufacets.cc
OBJS	+= unew.o ulocale.o ufacets.o
INCS	+= unew.h ulocale.h ufacets.h
endif
TOCLEAN	+= config.status config.log

########################################################################

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

${LIBA}:	${OBJS}
	@echo "Linking $@ ..."
	@${AR} r $@ $?
	@${RANLIB} $@

${LIBSOBLD}:	${OBJS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} ${SHBLDFL} -o $@ $^ ${LIBS}

.PHONY: install-static install-shared uninstall-static uninstall-shared

install-shared: ${LIBSOBLD} install-incs
	@echo "Installing ${LIBSOBLD} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBSOBLD} ${LIBDIR}
	@(cd ${LIBDIR}; ${LN} -sf ${LIBSOBLD} ${LIBSO}; ${LN} -sf ${LIBSOBLD} ${LIBSOLNK})

uninstall-shared: uninstall-incs
	@echo "Removing ${LIBSOBLD} from ${LIBDIR} ..."
	@${RM} -f ${LIBDIR}/${LIBSO} ${LIBDIR}/${LIBSOLNK} ${LIBDIR}/${LIBSOBLD}

install-static: ${LIBA} install-incs
	@echo "Installing ${LIBA} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBA} ${LIBDIR}

uninstall-static: uninstall-incs
	@echo "Removing ${LIBA} from ${LIBDIR} ..."
	@${RM} -f ${LIBDIR}/${LIBA}

install-incs: ${INCS}
	@echo "Installing headers to ${INCDIR} ..."
	@${INSTALLDIR} ${INCDIR}/${LIBNAME}
	@${INSTALLDIR} ${INCDIR}/${LIBNAME}/locale
	@for i in ${INCS}; do					\
	    ${INSTALLDATA} $$i ${INCDIR}/${LIBNAME}/$$i;	\
	done;
	@${INSTALLDATA} ${LIBNAME}.h ${INCDIR}

uninstall-incs:
	@echo "Removing headers from ${INCDIR} ..."
	@${RM} -rf ${INCDIR}/${LIBNAME} ${INCDIR}/${LIBNAME}.h


%.o:	%.cc
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

.PHONY:	gch clean depend dox dist dist-clean maintainer-clean

gch:	${INCDIR}/ustl.h.gch
${INCDIR}/ustl.h.gch:	${INCS}
	@echo "    Creating precompiled header ..."
	@${CXX} ${CXXFLAGS} -o $@ -c ${INCDIR}/ustl.h

clean:
	@echo "Removing generated files ..."
	@${RM} -f ${OBJS} ${TOCLEAN} *.rpo
	@+make -C bvt clean

depend: ${SRCS}
	@${CXX} ${CXXFLAGS} -M ${SRCS} > .depend;
	@+make -C bvt depend

dox:
	@${DOXYGEN} ${DOCT}

TMPDIR	= /tmp
DISTDIR	= ${HOME}/stored
DISTNAM	= ustl-${MAJOR}.${MINOR}
DISTTAR	= ${DISTNAM}-${BUILD}.tar.bz2

dist:
	mkdir ${TMPDIR}/${DISTNAM}
	cp -r . ${TMPDIR}/${DISTNAM}
	+make -C ${TMPDIR}/${DISTNAM} dox dist-clean
	(cd ${TMPDIR}/${DISTNAM}; rm -rf CVS; cd bvt; rm -rf CVS; cd ../docs; rm -rf CVS)
	(cd ${TMPDIR}; tar jcf ${DISTDIR}/${DISTTAR} ${DISTNAM}; rm -rf ${DISTNAM})

dist-clean:	clean
	@rm -f Common.mk config.h ustl.spec bsconf.o bsconf .depend bvt/.depend

maintainer-clean: dist-clean
	@rm -rf docs ustl.spec

-include .depend
 
