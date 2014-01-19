EXTRA_DIST = 

indicatordir = $(PIDGIN_LIBDIR)/pidgin

indicator_la_LDFLAGS = -module -avoid-version

if PLUGINS

indicator_LTLIBRARIES = indicator.la

indicator_la_SOURCES = \
	pidgin-indicator.c \
	gtkdocklet.c

indicator_la_LIBADD = \
	$(BINRELOC_LIBS) \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(PIDGIN_LIBS) \
	$(PURPLE_LIBS) \
	$(INDICATOR_LIBS)

endif

AM_CPPFLAGS = \
	-DVERSION=\"$(VERSION)\" \
	-DDATADIR=\"$(PIDGIN_DATADIR)\" \
	$(BINRELOC_CFLAGS) \
	$(DEBUG_CFLAGS) \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(PIDGIN_CFLAGS) \
	$(INDICATOR_CFLAGS)
