# Common targets that are used by most projects
#

LIBA		= lib${LIBNAME}.a
LIBSO		= lib${LIBNAME}.so
ifdef MAJOR
LIBSOLNK	= ${LIBSO}.${MAJOR}
LIBSOBLD	= ${LIBSO}.${MAJOR}.${MINOR}.${BUILD}
endif
TOCLEAN		+= ${LIBSO} ${LIBA} ${LIBSOBLD}

ifdef LIBSOBLD

all:	${LIBSOBLD}

${LIBSOBLD}:	${OBJS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ -Wl,-shared,-soname=${LIBSOLNK} $^ ${LIBS}

install:	install-shared install-incs
uninstall:	uninstall-shared uninstall-incs

else

all:	${LIBA}

install:	install-static install-incs
uninstall:	uninstall-static uninstall-incs

endif

${LIBA}:	${OBJS}
	@echo "Linking $@ ..."
	@${AR} r $@ $?
	@${RANLIB} $@

ifdef INCS

install-incs: ${INCS}
	@echo "Installing headers to ${INCDIR} ..."
	@if [ ! "${INCS}" = "${LIBNAME}.h" ]; then		\
	    ${INSTALLDIR} ${INCDIR}/${LIBNAME};			\
	    for i in ${INCS}; do				\
	    	if [ ! "$$i" = "." ]; then			\
		    ${INSTALLDIR} ${INCDIR}/${LIBNAME}/`dirname $$i`;		\
		fi;						\
		${INSTALLDATA} $$i ${INCDIR}/${LIBNAME}/$$i;	\
	    done;						\
	fi
	@if [ -f "${LIBNAME}.h" ]; then				\
	    ${INSTALLDATA} ${LIBNAME}.h ${INCDIR};		\
	fi

uninstall-incs:
	@echo "Removing headers from ${INCDIR} ..."
	@if [ ! "${LIBNAME}" = "" ]; then	\
	    ${RM} -rf ${INCDIR}/${LIBNAME};	\
	fi
	@${RM} -f ${INCDIR}/${LIBNAME}.h

else

install-incs:
uninstall-incs:

endif

install-static: ${LIBA}
	@echo "Installing ${LIBA} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBA} ${LIBDIR}

uninstall-static:
	@echo "Removing ${LIBA} from ${LIBDIR} ..."
	@${RM} -f ${LIBDIR}/${LIBA}

install-shared: ${LIBSOBLD}
	@echo "Installing ${LIBSOBLD} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBSOBLD} ${LIBDIR}
	@(cd ${LIBDIR}; ${LN} -sf ${LIBSOBLD} ${LIBSO}; ${LN} -sf ${LIBSOBLD} ${LIBSOLNK})

uninstall-shared:
	@echo "Removing ${LIBSOBLD} from ${LIBDIR} ..."
	@${RM} -f ${LIBDIR}/${LIBSO} ${LIBDIR}/${LIBSOLNK} ${LIBDIR}/${LIBSOBLD}

include Comrudir.mk
-include .depend
 
