include Common.mk

SUBDIRS	= bvt
LIBNAME	= ustl

SRCS	= cmemlink.cc memlink.cc memblock.cc mstream.cc ustring.cc \
       	varsize.cc uexception.cc sistream.cc sostream.cc fdostream.cc
OBJS	= cmemlink.o memlink.o memblock.o mstream.o ustring.o \
        varsize.o uexception.o sistream.o sostream.o fdostream.o
INCS	= cmemlink.h memblock.h memlink.h mistream.h mostream.h \
	ualgo.h ustl.h ustring.h utypes.h uutility.h uvector.h \
	uctralgo.h ufunction.h upair.h uiterator.h ustack.h varsize.h \
	uexception.h strmsize.h sistream.h sostream.h ulimits.h uset.h \
	umultiset.h uspecial.h uios.h fdostream.h unew.h umap.h umultimap.h \
	uiosfunc.h
DOCT	= ustldoc.in

include Comrulib.mk

