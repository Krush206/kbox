# Makefile.in generated by automake 1.11.1 from Makefile.am.
# awklib/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
# 2003, 2004, 2005, 2006, 2007, 2008, 2009  Free Software Foundation,
# Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



#
# awklib/Makefile.am --- automake input file for gawk
#
# Copyright (C) 1995-2006 the Free Software Foundation, Inc.
#
# This file is part of GAWK, the GNU implementation of the
# AWK Programming Language.
#
# GAWK is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# GAWK is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
#



pkgincludedir = $(includedir)/gawk
pkglibdir = $(libdir)/gawk
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = i686-pc-linux-gnulibc1
host_triplet = i686-pc-none
pkglibexec_PROGRAMS = pwcat$(EXEEXT) grcat$(EXEEXT)
subdir = awklib
DIST_COMMON = $(srcdir)/Makefile.am $(srcdir)/Makefile.in ChangeLog
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/m4/arch.m4 \
	$(top_srcdir)/m4/codeset.m4 $(top_srcdir)/m4/gettext.m4 \
	$(top_srcdir)/m4/iconv.m4 $(top_srcdir)/m4/intlmacosx.m4 \
	$(top_srcdir)/m4/intmax_t.m4 $(top_srcdir)/m4/inttypes_h.m4 \
	$(top_srcdir)/m4/isc-posix.m4 $(top_srcdir)/m4/lcmessage.m4 \
	$(top_srcdir)/m4/lib-ld.m4 $(top_srcdir)/m4/lib-link.m4 \
	$(top_srcdir)/m4/lib-prefix.m4 $(top_srcdir)/m4/libsigsegv.m4 \
	$(top_srcdir)/m4/longlong.m4 $(top_srcdir)/m4/nls.m4 \
	$(top_srcdir)/m4/po.m4 $(top_srcdir)/m4/progtest.m4 \
	$(top_srcdir)/m4/readline.m4 $(top_srcdir)/m4/socket.m4 \
	$(top_srcdir)/m4/stdint_h.m4 $(top_srcdir)/m4/uintmax_t.m4 \
	$(top_srcdir)/m4/ulonglong.m4 $(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
am__installdirs = "$(DESTDIR)$(pkglibexecdir)" "$(DESTDIR)$(bindir)"
PROGRAMS = $(pkglibexec_PROGRAMS)
nodist_grcat_OBJECTS = grcat.$(OBJEXT)
grcat_OBJECTS = $(nodist_grcat_OBJECTS)
grcat_LDADD = $(LDADD)
nodist_pwcat_OBJECTS = pwcat.$(OBJEXT)
pwcat_OBJECTS = $(nodist_pwcat_OBJECTS)
pwcat_LDADD = $(LDADD)
am__vpath_adj_setup = srcdirstrip=`echo "$(srcdir)" | sed 's|.|.|g'`;
am__vpath_adj = case $$p in \
    $(srcdir)/*) f=`echo "$$p" | sed "s|^$$srcdirstrip/||"`;; \
    *) f=$$p;; \
  esac;
am__strip_dir = f=`echo $$p | sed -e 's|^.*/||'`;
am__install_max = 40
am__nobase_strip_setup = \
  srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*|]/\\\\&/g'`
am__nobase_strip = \
  for p in $$list; do echo "$$p"; done | sed -e "s|$$srcdirstrip/||"
am__nobase_list = $(am__nobase_strip_setup); \
  for p in $$list; do echo "$$p $$p"; done | \
  sed "s| $$srcdirstrip/| |;"' / .*\//!s/ .*/ ./; s,\( .*\)/[^/]*$$,\1,' | \
  $(AWK) 'BEGIN { files["."] = "" } { files[$$2] = files[$$2] " " $$1; \
    if (++n[$$2] == $(am__install_max)) \
      { print $$2, files[$$2]; n[$$2] = 0; files[$$2] = "" } } \
    END { for (dir in files) print dir, files[dir] }'
am__base_list = \
  sed '$$!N;$$!N;$$!N;$$!N;$$!N;$$!N;$$!N;s/\n/ /g' | \
  sed '$$!N;$$!N;$$!N;$$!N;s/\n/ /g'
SCRIPTS = $(bin_SCRIPTS)
DEFAULT_INCLUDES = -I. -I$(top_builddir)
depcomp = $(SHELL) $(top_srcdir)/depcomp
am__depfiles_maybe = depfiles
am__mv = mv -f
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
SOURCES = $(nodist_grcat_SOURCES) $(nodist_pwcat_SOURCES)
DIST_SOURCES =
ETAGS = etags
CTAGS = ctags
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
pkgdatadir = $(datadir)/awk
pkglibexecdir = $(libexecdir)/awk
ACLOCAL = ${SHELL} /home/kevin/source/kbox_term/source/gawk-4.0.1/missing --run aclocal-1.11
AMTAR = ${SHELL} /home/kevin/source/kbox_term/source/gawk-4.0.1/missing --run tar
AUTOCONF = ${SHELL} /home/kevin/source/kbox_term/source/gawk-4.0.1/missing --run autoconf
AUTOHEADER = ${SHELL} /home/kevin/source/kbox_term/source/gawk-4.0.1/missing --run autoheader
AUTOMAKE = ${SHELL} /home/kevin/source/kbox_term/source/gawk-4.0.1/missing --run automake-1.11
AWK = gawk
CC = /home/kevin/lib/android-9-toolchain/bin/arm-linux-androideabi-gcc
CCDEPMODE = depmode=gcc3
CFLAGS = -g -O2
CPP = /home/kevin/lib/android-9-toolchain/bin/arm-linux-androideabi-gcc -E
CPPFLAGS = 
CYGPATH_W = echo
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /bin/grep -E
EXEEXT = 
GETTEXT_MACRO_VERSION = 0.18
GMSGFMT = /usr/bin/msgfmt
GMSGFMT_015 = /usr/bin/msgfmt
GREP = /bin/grep
HAVE_LIBSIGSEGV = no
INSTALL = .././install-sh -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
INTLLIBS = 
INTL_MACOSX_LIBS = 
LDFLAGS = -L../ncurses-5.9/lib/ -export-dynamic
LIBICONV = 
LIBINTL = 
LIBOBJS =  ${LIBOBJDIR}mktime$U.o
LIBREADLINE = -lreadline -lncurses
LIBS = -lm 
LIBSIGSEGV = 
LIBSIGSEGV_PREFIX = 
LN_S = ln -s
LTLIBICONV = 
LTLIBINTL = 
LTLIBOBJS =  ${LIBOBJDIR}mktime$U.lo
LTLIBSIGSEGV = 
MAKEINFO = ${SHELL} /home/kevin/source/kbox_term/source/gawk-4.0.1/missing --run makeinfo
MKDIR_P = /bin/mkdir -p
MSGFMT = /usr/bin/msgfmt
MSGFMT_015 = /usr/bin/msgfmt
MSGMERGE = /usr/bin/msgmerge
OBJEXT = o
PACKAGE = gawk
PACKAGE_BUGREPORT = bug-gawk@gnu.org
PACKAGE_NAME = GNU Awk
PACKAGE_STRING = GNU Awk 4.0.1
PACKAGE_TARNAME = gawk
PACKAGE_URL = http://www.gnu.org/software/gawk/
PACKAGE_VERSION = 4.0.1
PATH_SEPARATOR = :
POSUB = 
SET_MAKE = 
SHELL = /bin/sh
SOCKET_LIBS = 
STRIP = strip
USE_NLS = no
VERSION = 4.0.1
XGETTEXT = /usr/bin/xgettext
XGETTEXT_015 = /usr/bin/xgettext
XGETTEXT_EXTRA_OPTIONS = 
YACC = bison -y
YFLAGS = 
abs_builddir = /home/kevin/source/kbox_term/source/gawk-4.0.1/awklib
abs_srcdir = /home/kevin/source/kbox_term/source/gawk-4.0.1/awklib
abs_top_builddir = /home/kevin/source/kbox_term/source/gawk-4.0.1
abs_top_srcdir = /home/kevin/source/kbox_term/source/gawk-4.0.1
ac_ct_CC = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = ${AMTAR} chof - "$$tardir"
am__untar = ${AMTAR} xf -
bindir = ${exec_prefix}/bin
build = i686-pc-linux-gnulibc1
build_alias = 
build_cpu = i686
build_os = linux-gnulibc1
build_vendor = pc
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = ${prefix}
host = i686-pc-none
host_alias = i686
host_cpu = i686
host_os = none
host_vendor = pc
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /home/kevin/source/kbox_term/source/gawk-4.0.1/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
mkdir_p = /bin/mkdir -p
oldincludedir = /usr/include
pdfdir = ${docdir}
prefix = /usr/local
program_transform_name = s,x,x,
psdir = ${docdir}
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target_alias = arm
top_build_prefix = ../
top_builddir = ..
top_srcdir = ..
EXTRA_DIST = ChangeLog ChangeLog.0 extract.awk eg $(srcdir)/stamp-eg
# With some locales, the script extract.awk fails.
# So we fix the locale to some sensible value.
AWKPROG = LC_ALL=C LANG=C $(abs_top_builddir)/gawk$(EXEEXT)

# Get config.h from the build directory and custom.h from the source directory.
AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)
bin_SCRIPTS = igawk
AUXAWK = passwd.awk group.awk
nodist_grcat_SOURCES = grcat.c
nodist_pwcat_SOURCES = pwcat.c
all: all-am

.SUFFIXES:
.SUFFIXES: .c .o .obj
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu awklib/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu awklib/Makefile
.PRECIOUS: Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure:  $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):
install-pkglibexecPROGRAMS: $(pkglibexec_PROGRAMS)
	@$(NORMAL_INSTALL)
	test -z "$(pkglibexecdir)" || $(MKDIR_P) "$(DESTDIR)$(pkglibexecdir)"
	@list='$(pkglibexec_PROGRAMS)'; test -n "$(pkglibexecdir)" || list=; \
	for p in $$list; do echo "$$p $$p"; done | \
	sed 's/$(EXEEXT)$$//' | \
	while read p p1; do if test -f $$p; \
	  then echo "$$p"; echo "$$p"; else :; fi; \
	done | \
	sed -e 'p;s,.*/,,;n;h' -e 's|.*|.|' \
	    -e 'p;x;s,.*/,,;s/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/' | \
	sed 'N;N;N;s,\n, ,g' | \
	$(AWK) 'BEGIN { files["."] = ""; dirs["."] = 1 } \
	  { d=$$3; if (dirs[d] != 1) { print "d", d; dirs[d] = 1 } \
	    if ($$2 == $$4) files[d] = files[d] " " $$1; \
	    else { print "f", $$3 "/" $$4, $$1; } } \
	  END { for (d in files) print "f", d, files[d] }' | \
	while read type dir files; do \
	    if test "$$dir" = .; then dir=; else dir=/$$dir; fi; \
	    test -z "$$files" || { \
	      echo " $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$files '$(DESTDIR)$(pkglibexecdir)$$dir'"; \
	      $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$files "$(DESTDIR)$(pkglibexecdir)$$dir" || exit $$?; \
	    } \
	; done

uninstall-pkglibexecPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(pkglibexec_PROGRAMS)'; test -n "$(pkglibexecdir)" || list=; \
	files=`for p in $$list; do echo "$$p"; done | \
	  sed -e 'h;s,^.*/,,;s/$(EXEEXT)$$//;$(transform)' \
	      -e 's/$$/$(EXEEXT)/' `; \
	test -n "$$list" || exit 0; \
	echo " ( cd '$(DESTDIR)$(pkglibexecdir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(pkglibexecdir)" && rm -f $$files

clean-pkglibexecPROGRAMS:
	-test -z "$(pkglibexec_PROGRAMS)" || rm -f $(pkglibexec_PROGRAMS)
install-binSCRIPTS: $(bin_SCRIPTS)
	@$(NORMAL_INSTALL)
	test -z "$(bindir)" || $(MKDIR_P) "$(DESTDIR)$(bindir)"
	@list='$(bin_SCRIPTS)'; test -n "$(bindir)" || list=; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  if test -f "$$d$$p"; then echo "$$d$$p"; echo "$$p"; else :; fi; \
	done | \
	sed -e 'p;s,.*/,,;n' \
	    -e 'h;s|.*|.|' \
	    -e 'p;x;s,.*/,,;$(transform)' | sed 'N;N;N;s,\n, ,g' | \
	$(AWK) 'BEGIN { files["."] = ""; dirs["."] = 1; } \
	  { d=$$3; if (dirs[d] != 1) { print "d", d; dirs[d] = 1 } \
	    if ($$2 == $$4) { files[d] = files[d] " " $$1; \
	      if (++n[d] == $(am__install_max)) { \
		print "f", d, files[d]; n[d] = 0; files[d] = "" } } \
	    else { print "f", d "/" $$4, $$1 } } \
	  END { for (d in files) print "f", d, files[d] }' | \
	while read type dir files; do \
	     if test "$$dir" = .; then dir=; else dir=/$$dir; fi; \
	     test -z "$$files" || { \
	       echo " $(INSTALL_SCRIPT) $$files '$(DESTDIR)$(bindir)$$dir'"; \
	       $(INSTALL_SCRIPT) $$files "$(DESTDIR)$(bindir)$$dir" || exit $$?; \
	     } \
	; done

uninstall-binSCRIPTS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_SCRIPTS)'; test -n "$(bindir)" || exit 0; \
	files=`for p in $$list; do echo "$$p"; done | \
	       sed -e 's,.*/,,;$(transform)'`; \
	test -n "$$list" || exit 0; \
	echo " ( cd '$(DESTDIR)$(bindir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(bindir)" && rm -f $$files

mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c

include ./$(DEPDIR)/grcat.Po
include ./$(DEPDIR)/pwcat.Po

.c.o:
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(COMPILE) -c $<

.c.obj:
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
	$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(COMPILE) -c `$(CYGPATH_W) '$<'`

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	mkid -fID $$unique
tags: TAGS

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	set x; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)  $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: CTAGS
CTAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	list='$(SOURCES) $(HEADERS)  $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags

distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(PROGRAMS) $(SCRIPTS)
installdirs:
	for dir in "$(DESTDIR)$(pkglibexecdir)" "$(DESTDIR)$(bindir)"; do \
	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
	done
install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-local clean-pkglibexecPROGRAMS \
	mostlyclean-am

distclean: distclean-am
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
distclean-am: clean-am distclean-compile distclean-generic \
	distclean-tags

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am:

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am: install-binSCRIPTS install-pkglibexecPROGRAMS
	@$(NORMAL_INSTALL)
	$(MAKE) $(AM_MAKEFLAGS) install-exec-hook
install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man:

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am: uninstall-binSCRIPTS uninstall-local \
	uninstall-pkglibexecPROGRAMS

.MAKE: install-am install-exec-am install-strip

.PHONY: CTAGS GTAGS all all-am check check-am clean clean-generic \
	clean-local clean-pkglibexecPROGRAMS ctags distclean \
	distclean-compile distclean-generic distclean-tags distdir dvi \
	dvi-am html html-am info info-am install install-am \
	install-binSCRIPTS install-data install-data-am install-dvi \
	install-dvi-am install-exec install-exec-am install-exec-hook \
	install-html install-html-am install-info install-info-am \
	install-man install-pdf install-pdf-am \
	install-pkglibexecPROGRAMS install-ps install-ps-am \
	install-strip installcheck installcheck-am installdirs \
	maintainer-clean maintainer-clean-generic mostlyclean \
	mostlyclean-compile mostlyclean-generic pdf pdf-am ps ps-am \
	tags uninstall uninstall-am uninstall-binSCRIPTS \
	uninstall-local uninstall-pkglibexecPROGRAMS


all: $(srcdir)/stamp-eg $(AUXPROGS) igawk $(AUXAWK)

install-exec-hook: $(AUXAWK)
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)
	for i in $(AUXAWK) $(srcdir)/eg/lib/*.awk ; do \
		progname=`echo $$i | sed 's;.*/;;'` ; \
		$(INSTALL_DATA) $$i $(DESTDIR)$(pkgdatadir)/$$progname ; \
	done

# pkglibexecdir and pkgdatadir are removed in the top level Makefile's uninstall
uninstall-local:
	rm -fr $(DESTDIR)$(pkglibexecdir)/* $(DESTDIR)$(pkgdatadir)/*
	rm -f $(DESTDIR)$(bindir)/igawk

clean-local:
	rm -f $(AUXAWK) igawk *.exe
	rm -fr eg.old

$(srcdir)/stamp-eg: $(srcdir)/../doc/gawk.texi $(srcdir)/../doc/gawkinet.texi
	cd $(srcdir) && \
	rm -fr eg && \
	rm -fr stamp-eg && \
	$(AWKPROG) -f extract.awk ../doc/gawk.texi ../doc/gawkinet.texi
	@echo 'some makes are stupid and will not check a directory' > $(srcdir)/stamp-eg
	@echo 'against a file, so this file is a place holder. gack.' >> $(srcdir)/stamp-eg

$(srcdir)/eg/lib/pwcat.c $(srcdir)/eg/lib/grcat.c $(srcdir)/eg/prog/igawk.sh \
$(srcdir)/eg/lib/passwdawk.in $(srcdir)/eg/lib/groupawk.in: stamp-eg; @:

pwcat$(EXEEXT): $(srcdir)/eg/lib/pwcat.c
	$(COMPILE) $(srcdir)/eg/lib/pwcat.c $(LDFLAGS) -o $@

grcat$(EXEEXT): $(srcdir)/eg/lib/grcat.c
	$(COMPILE) $(srcdir)/eg/lib/grcat.c $(LDFLAGS) -o $@

igawk: $(srcdir)/eg/prog/igawk.sh
	cp $(srcdir)/eg/prog/igawk.sh $@ ; chmod 755 $@

passwd.awk: $(srcdir)/eg/lib/passwdawk.in
	sed 's;/usr/local/libexec/awk;$(pkglibexecdir);' < $(srcdir)/eg/lib/passwdawk.in > passwd.awk

group.awk: $(srcdir)/eg/lib/groupawk.in
	sed 's;/usr/local/libexec/awk;$(pkglibexecdir);' < $(srcdir)/eg/lib/groupawk.in > group.awk

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
