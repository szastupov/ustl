include Common.mk

SUBDIRS	= bvt
LIBNAME	= ustl

SRCS	= cmemlink.cc memlink.cc memblock.cc mstream.cc ustring.cc \
       	uexception.cc sistream.cc sostream.cc fdostream.cc unew.cc
OBJS	= cmemlink.o memlink.o memblock.o mstream.o ustring.o \
        uexception.o sistream.o sostream.o fdostream.o unew.o
INCS	= cmemlink.h memblock.h memlink.h mistream.h mostream.h \
	ualgo.h ustl.h ustring.h utypes.h uutility.h uvector.h \
	ualgobase.h uctralgo.h ufunction.h upair.h uiterator.h ustack.h \
	uexception.h strmsize.h sistream.h sostream.h ulimits.h uset.h \
	umultiset.h uspecial.h uios.h fdostream.h unew.h umap.h umultimap.h \
	umemory.h uiosfunc.h utf8.h config.h
DOCT	= ustldoc.in
TOCLEAN	= install config.status config.log

include Comrulib.mk

