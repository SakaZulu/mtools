INCLUDES = \
	$(DEPS_CFLAGS) \
	-I/usr/local/include \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/gtk \
	-I$(top_srcdir)/src/gtk/dialogs

AM_CPPFLAGS = -Wall -g -O0 -DMTOOLS_LOCALEDIR=\"${MTOOLS_LOCALEDIR}\"

bin_PROGRAMS = mtools

mtools_SOURCES = mtools.cpp \
	gtk/Application.h gtk/Application.cpp \
	gtk/Mtools.cpp gtk/Mtools.h \
	gtk/Callbacks.cpp gtk/Callbacks.h \
	gtk/TrailerAccessConditions.cpp gtk/TrailerAccessConditions.h \
	gtk/DataAccessConditions.cpp gtk/DataAccessConditions.h \
	gtk/ComboBoxText.cpp gtk/ComboBoxText.h \
	Nfc.cpp Nfc.h Utils.h \
	gtk/Consts.cpp gtk/Consts.h \
	gtk/GtkUtil.h gtk/GtkUtil.cpp \
	gtk/dialogs/ErrorDialog.cpp gtk/dialogs/ErrorDialog.h \
	gtk/dialogs/KeysChooserDialog.cpp gtk/dialogs/KeysChooserDialog.h \
	gtk/dialogs/KeysStoreDialog.cpp gtk/dialogs/KeysStoreDialog.h \
	gtk/dialogs/InputDialog.cpp gtk/dialogs/InputDialog.h
	

mtools_LDFLAGS = -lboost_regex-mt -lboost_thread-mt

mtools_LDADD = $(DEPS_LIBS)



