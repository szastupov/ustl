include Common.mk

SRCS	= cmemlink.cc fdostream.cc memblock.cc memlink.cc mstream.cc \
	sistream.cc sostream.cc ubitset.cc uexception.cc ustdxept.cc ustring.cc
OBJS	= cmemlink.o fdostream.o memblock.o memlink.o mstream.o \
	sistream.o sostream.o ubitset.o uexception.o ustdxept.o ustring.o
INCS	= cmemlink.h config.h fdostream.h memblock.h memlink.h mistream.h \
	mostream.h simd.h sistream.h sostream.h strmsize.h ualgo.h \
	ualgobase.h ubitset.h uctralgo.h uexception.h ufunction.h uios.h \
	uiosfunc.h uiterator.h ulaalgo.h ulimits.h ulist.h umap.h umatrix.h \
	umemory.h umultimap.h umultiset.h unumeric.h upair.h upredalgo.h \
	uqueue.h uset.h uspecial.h ustack.h ustdxept.h ustl.h ustl.tbff \
	ustring.h utf8.h utuple.h utypes.h uutility.h uvector.h
DOCT	= ustldoc.in

ifdef STANDALONE
SRCS	+= unew.cc
OBJS	+= unew.o
INCS	+= unew.h
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

gch:	${INCDIR}/${LIBNAME}.h.gch
${INCDIR}/${LIBNAME}.h.gch:	${INCS}
	@echo "    Creating precompiled header ..."
	@${CXX} ${CXXFLAGS} -o $@ -c ${INCDIR}/${LIBNAME}.h

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
DISTNAM	= ${LIBNAME}-${MAJOR}.${MINOR}
DISTTAR	= ${DISTNAM}-${BUILD}.tar.bz2

dist:
	mkdir ${TMPDIR}/${DISTNAM}
	cp -r . ${TMPDIR}/${DISTNAM}
	+make -C ${TMPDIR}/${DISTNAM} dox dist-clean
	(cd ${TMPDIR}/${DISTNAM}; rm -rf CVS; cd bvt; rm -rf CVS; cd ../docs; rm -rf CVS)
	(cd ${TMPDIR}; tar jcf ${DISTDIR}/${DISTTAR} ${DISTNAM}; rm -rf ${DISTNAM})

dist-clean:	clean
	@rm -f Common.mk config.h ${LIBNAME}.spec bsconf.o bsconf .depend bvt/.depend

maintainer-clean: dist-clean
	@rm -rf docs/html

-include .depend
 
