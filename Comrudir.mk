%.o:	%.cc
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -o $@ -c $<

%.o:	%.c
	@echo "    Compiling $< ..."
	@${CC} ${CFLAGS} -o $@ -c $<

clean:	subdirs-clean
	@echo "Removing generated files ..."
	@${RM} -f ${OBJS} ${TOCLEAN} *.rpo .depend

depend: ${SRCS} subdirs-depend
	@if [ "${SRCS}" != "" ]; then			\
	    ${CXX} ${CXXFLAGS} -M ${SRCS} > .depend;	\
	fi

dox:
	@${DOXYGEN} ${DOCT}

autoconf-clean:
	rm -f Common.mk config.h config.log config.status config.cache

maintainer-clean: clean autoconf-clean
	rm -f configure
	rm -rf docs

tounix:	subdirs-tounix
	@for i in ${SRCS}; do \
		mv $${i%%.cc}.cpp $$i; \
	done
	@for i in ${SRCS} ${INCS}; do \
		dos2unix -q -k $$i; \
	done

towin:	${SRCS}	subdirs-towin
	@for i in ${SRCS} ${INCS}; do \
		unix2dos -q -k $$i; \
	done
	@for i in ${SRCS}; do \
		mv $$i $${i%%.cc}.cpp; \
	done

subdirs-all:
	@for i in ${SUBDIRS}; do \
		(echo Running all in $$i; ${MAKE} -s -C $$i all; echo Leaving $$i) || exit; \
	done

subdirs-depend:
	@for i in ${SUBDIRS}; do \
		(echo Running depend in $$i; ${MAKE} -s -C $$i depend; echo Leaving $$i) || exit; \
	done

subdirs-clean:
	@for i in ${SUBDIRS}; do \
		(echo Running clean in $$i; ${MAKE} -s -C $$i clean; echo Leaving $$i) || exit; \
	done

subdirs-install:
	@for i in ${SUBDIRS}; do \
		(echo Running install in $$i; ${MAKE} -s -C $$i install; echo Leaving $$i) || exit; \
	done

subdirs-uninstall:
	@for i in ${SUBDIRS}; do \
		(echo Running uninstall in $$i; ${MAKE} -s -C $$i uninstall; echo Leaving $$i) || exit; \
	done

subdirs-tounix:
	@for i in ${SUBDIRS}; do \
		(echo Running tounix in $$i; ${MAKE} -s -C $$i tounix; echo Leaving $$i) || exit; \
	done


subdirs-towin:
	@for i in ${SUBDIRS}; do \
		(echo Running towin in $$i; ${MAKE} -s -C $$i towin; echo Leaving $$i) || exit; \
	done


