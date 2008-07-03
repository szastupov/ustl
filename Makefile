-include Config.mk

################ Source files ##########################################

SRCS	:= $(wildcard *.cc)
INCS	:= $(wildcard *.h)
OBJS	:= $(addprefix $O,$(SRCS:.cc=.o))
DOCT	:= ustldoc.in

################ Compilation ###########################################

.PHONY: all clean dox html check dist distclean maintainer-clean

ALLTGTS	:= Config.mk config.h ${LIBNAME}
all:	${ALLTGTS}

${LIBNAME}:	.
	@rm -f ${LIBNAME}; ln -s . ${LIBNAME}

ifdef BUILD_SHARED
all:	${LIBSOBLD} ${LIBSOLNK} ${LIBSO}
ALLTGTS	+= ${LIBSOBLD} ${LIBSOLNK} ${LIBSO}
${LIBSOBLD}:	${OBJS}
	@echo "Linking $(notdir $@) ..."
	@${LD} ${LDFLAGS} $(call shlib_flags,$(subst $O,,${LIBSOLNK})) -o $@ $^ ${LIBS}
${LIBSOLNK} ${LIBSO}:	${LIBSOBLD}
	@(cd $(dir $@); rm -f $(notdir $@); ln -s $(notdir $<) $(notdir $@))

endif
ifdef BUILD_STATIC
all:	${LIBA}
ALLTGTS	+= ${LIBA}
${LIBA}:	${OBJS}
	@echo "Linking $@ ..."
	@${AR} r $@ $?
	@${RANLIB} $@
endif

$O%.o:	%.cc
	@echo "    Compiling $< ..."
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@${CXX} ${CXXFLAGS} -MMD -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

include bvt/Module.mk

################ Installation ##########################################

.PHONY:	install uninstall install-incs uninstall-incs

ifdef INCDIR
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
endif

ifdef BUILD_SHARED
.PHONY: install-shared uninstall-shared
LIBTI	:= ${LIBDIR}/$(notdir ${LIBSOBLD})
LIBLI	:= ${LIBDIR}/$(notdir ${LIBSOLNK})
LIBSI	:= ${LIBDIR}/$(notdir ${LIBSO})
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

clean:	bvt/clean
	@echo "Removing generated files ..."
	@rm -f ${OBJS} $(OBJS:.o=.d) ${LIBA} ${LIBSOBLD} ${LIBSO} ${LIBSOLNK}
	@rmdir $O &> /dev/null || true

check:	bvt/run

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
	@rm -f Config.mk config.h config.status ${LIBNAME}

maintainer-clean: distclean
	@rm -rf docs/html

html:
dox:
	@${DOXYGEN} ${DOCT}

${OBJS} ${bvt/OBJS}:	Makefile Config.mk config.h
${bvt/OBJS}:		bvt/Module.mk
Config.mk:		Config.mk.in
config.h:		config.h.in
Config.mk config.h:	configure
	@if [ -x config.status ]; then echo "Reconfiguring ..."; ./config.status; \
	else echo "Running configure ..."; ./configure; fi

-include ${OBJS:.o=.d}
