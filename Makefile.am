INCLUDES = 
METASOURCES = AUTO

SUBDIRS = util text primitives helper sample_classes

lib_LTLIBRARIES =  libsgl.la
libsgl_la_SOURCES =  sglmetaobj.cpp sglmetaobj.h sglmisc.c sglmisc.h sglobjbase.cpp sglobjbase.h sglobj.cpp sglobj.h sglspace.cpp sglspace.h sglvektor.cpp sglvektor.h
libsgl_la_LIBADD = $(top_builddir)/libsgl/sample_classes/libsample_classes.la $(top_builddir)/libsgl/util/libutil.la $(top_builddir)/libsgl/text/libtext.la $(top_builddir)/libsgl/primitives/libprimitives.la $(top_builddir)/libsgl/helper/libhelper.la
