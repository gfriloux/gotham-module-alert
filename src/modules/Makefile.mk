MAINTAINERCLEANFILES += \
src/lib/modules/alert/*.gc{no,da}

moddir = $(libdir)/gotham/$(MODULE_ARCH)
mod_LTLIBRARIES = \
   src/modules/alert.la

src_modules_alert_la_SOURCES = \
   src/modules/alert/alert.c \
   src/modules/alert/alert.h \
   src/modules/alert/alfred.c \
   src/modules/alert/botman.c \
   src/modules/alert/command.c \
   src/modules/alert/event.c \
   src/modules/alert/module.c \
   src/modules/alert/conf.c
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
