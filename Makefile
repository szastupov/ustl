include Common.mk

SRCS	= cmemlink.cc memlink.cc memblock.cc mstream.cc ustring.cc \
	uexception.cc sistream.cc sostream.cc fdostream.cc unew.cc ubitset.cc \
	ustdxept.cc ulocale.cc ufacets.cc
OBJS	= cmemlink.o memlink.o memblock.o mstream.o ustring.o \
	uexception.o sistream.o sostream.o fdostream.o unew.o ubitset.o \
	ustdxept.o ulocale.o ufacets.o
INCS	= cmemlink.h memblock.h memlink.h mistream.h mostream.h \
	ualgo.h ustl.h ustring.h utypes.h uutility.h uvector.h ualgobase.h \
	uctralgo.h ufunction.h upair.h uiterator.h ustack.h uqueue.h \
	uexception.h strmsize.h sistream.h sostream.h ulimits.h uset.h \
	umultiset.h uspecial.h uios.h fdostream.h unew.h umap.h umultimap.h \
	umemory.h uiosfunc.h utf8.h config.h ubitset.h unumeric.h utuple.h \
	ulocale.h ufacets.h ulist.h upredalgo.h ustdxept.h
DOCT	= ustldoc.in
TOCLEAN	= config.status config.log

ifdef STANDALONE
LIBS	= -lsupc++ -lgcc_s -lc
endif

########################################################################

LIBA		= lib${LIBNAME}.a
LIBSO		= lib${LIBNAME}.so
ifdef MAJOR
LIBSOLNK	= ${LIBSO}.${MAJOR}
LIBSOBLD	= ${LIBSO}.${MAJOR}.${MINOR}.${BUILD}
endif
TOCLEAN		+= ${LIBSO} ${LIBA} ${LIBSOBLD}

ifdef LIBSOBLD
TARGET	= ${LIBSOBLD}
${LIBSOBLD}:	${OBJS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ -Wl,-shared,-soname=${LIBSOLNK} $^ ${LIBS}

install: ${LIBSOBLD} install-incs
	@echo "Installing ${LIBSOBLD} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBSOBLD} ${LIBDIR}
	@(cd ${LIBDIR}; ${LN} -sf ${LIBSOBLD} ${LIBSO}; ${LN} -sf ${LIBSOBLD} ${LIBSOLNK})

uninstall: uninstall-incs
	@echo "Removing ${LIBSOBLD} from ${LIBDIR} ..."
	@${RM} -f ${LIBDIR}/${LIBSO} ${LIBDIR}/${LIBSOLNK} ${LIBDIR}/${LIBSOBLD}

else
TARGET	= ${LIBA}
${LIBA}:	${OBJS}
	@echo "Linking $@ ..."
	@${AR} r $@ $?
	@${RANLIB} $@

install: ${LIBA} install-incs
	@echo "Installing ${LIBA} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBA} ${LIBDIR}

uninstall: uninstall-incs
	@echo "Removing ${LIBA} from ${LIBDIR} ..."
	@${RM} -f ${LIBDIR}/${LIBA}

endif

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
RHATDIR	= ${HOME}/rpm
DISTDIR	= ustl-${MAJOR}.${MINOR}
DISTTAR	= ${DISTDIR}-${BUILD}.tar.bz2

dist:
	mkdir ${TMPDIR}/${DISTDIR}
	cp -r . ${TMPDIR}/${DISTDIR}
	+make -C ${TMPDIR}/${DISTDIR} dox dist-clean
	(cd ${TMPDIR}/${DISTDIR}; rm -rf CVS; cd bvt; rm -rf CVS; cd ../docs; rm -rf CVS)
	(cd ${TMPDIR}; tar jcf ${RHATDIR}/SOURCES/${DISTTAR} ${DISTDIR}; rm -rf ${DISTDIR})
	cp ustl.spec ${RHATDIR}/SPECS

dist-clean:	clean
	@rm -f Common.mk config.h ustl.spec bsconf.o bsconf .depend bvt/.depend

maintainer-clean: dist-clean
	@rm -rf docs ustl.spec

-include .depend
 
