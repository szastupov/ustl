TOCLEAN	+= ${EXE}

all:	${EXE}

${EXE}:	${OBJS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $^ ${LIBS}

install: ${EXE} subdirs-install
	@echo "Installing ${EXE} to ${BINDIR} ..."
	@${INSTALLDIR} ${BINDIR}
	@${INSTALLEXE} ${EXE} ${BINDIR}

uninstall: subdirs-uninstall
	@echo "Removing ${EXE} from ${BINDIR} ..."
	@${RM} -f ${BINDIR}/${EXE}

include Comrudir.mk
-include .depend
 
