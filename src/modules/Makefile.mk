MAINTAINERCLEANFILES += \
src/lib/modules/alert/*.gc{no,da}

moddir = $(libdir)/gotham/$(MODULE_ARCH)
mod_LTLIBRARIES = \
   src/modules/alert.la

BUILT_SOURCES = .sources_alert
src_modules_alert_la_SOURCES = \
   src/modules/alert/alert.c \
   src/modules/alert/alert.h \
   src/modules/alert/command.c \
   src/modules/alert/Module_Common_Azy.c \
   src/modules/alert/Module_Common_Azy.h \
   src/modules/alert/Module_Common.c \
   src/modules/alert/Module_Common.h
src_modules_alert_la_CFLAGS = \
   $(ALERT_CFLAGS) -DDATA_DIR=\"$(datadir)\" \
   -DSYSCONF_DIR=\"$(sysconfdir)\"
src_modules_alert_la_LDFLAGS = \
   `pkg-config --libs $(ALERT_REQUIRES)` \
   -no-undefined -module -avoid-version \
   -Wl,-z,defs -lm
src_modules_alert_la_LIBADD = \
   src/lib/libcjson.la
src_modules_alert_la_LIBTOOLFLAGS = --tag=disable-static
.sources_alert: src/modules/alert/alert.azy
	azy_parser -H -p -o $(top_srcdir)/src/modules/alert \
	                    $(top_srcdir)/src/modules/alert/alert.azy
