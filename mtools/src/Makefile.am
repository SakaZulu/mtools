INCLUDES = \
	$(DEPS_CFLAGS) \
	-I/usr/local/include \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/gtk \
	-I$(top_srcdir)/src/gtk/dialogs

AM_CPPFLAGS = -Wall -g -DMTOOLS_LOCALEDIR=\"${MTOOLS_LOCALEDIR}\"

bin_PROGRAMS = mtools

mtools_SOURCES = mtools.cpp \
	gtk/Application.h gtk/Application.cpp \
	gtk/Mtools.cpp gtk/Mtools.h \
	gtk/Callbacks.cpp gtk/Callbacks.h \
	gtk/TrailerAccessConditions.cpp gtk/TrailerAccessConditions.h \
	gtk/DataAccessConditions.cpp gtk/DataAccessConditions.h \
	Nfc.cpp Nfc.h Utils.h \
	gtk/Consts.cpp gtk/Consts.h \
	gtk/GtkUtil.h \
	gtk/dialogs/ErrorDialog.cpp gtk/dialogs/ErrorDialog.h
	

mtools_LDFLAGS = 

mtools_LDADD = $(DEPS_LIBS)



