
CFLAGS += -fPIC -DDATA_DIR="\"/usr/share/libsgl\"" -DGL_GLEXT_PROTOTYPES -pthread 
CXXFLAGS = $(CFLAGS)


CORE_DIR=
HELPER_DIR=helper/
PRIMITIVES_DIR=primitives/
UTIL_DIR=util/
TEXT_DIR=text/
TEXT_BACKEND_GLF_DIR=$(TEXT_DIR)backend_glf/

CORE_OBJFILES = \
$(CORE_DIR)sglmisc.o \
$(CORE_DIR)sglobj.o \
$(CORE_DIR)sglobjbase.o \
$(CORE_DIR)sglspace.o \
$(CORE_DIR)sglvektor.o \
$(CORE_DIR)sglmetaobj.o \
$(CORE_DIR)sglmatrixobj.o

HELPER_OBJFILES = \
$(HELPER_DIR)sglcamera.o \
$(HELPER_DIR)sglclipplane.o \
$(HELPER_DIR)sglgrid.o \
$(HELPER_DIR)sglhelper.o \
$(HELPER_DIR)sgllight.o \
$(HELPER_DIR)sgldisplist.o \
$(HELPER_DIR)sgllensflare.o 

PRIMITIVES_OBJFILES = \
$(PRIMITIVES_DIR)sglflobj.o \
$(PRIMITIVES_DIR)sglgeosphere.o \
$(PRIMITIVES_DIR)sglicosahedron.o \
$(PRIMITIVES_DIR)sglloft.o \
$(PRIMITIVES_DIR)sglneck.o \
$(PRIMITIVES_DIR)sglpolygonobj.o \
$(PRIMITIVES_DIR)sglquader.o \
$(PRIMITIVES_DIR)sglvieleck.o \
$(PRIMITIVES_DIR)sglquadricobj.o

UTIL_OBJFILES = \
$(UTIL_DIR)readtex.o  \
$(UTIL_DIR)sglmaterial.o  \
$(UTIL_DIR)sgltextur.o \
$(UTIL_DIR)sglimagefile.o \
$(UTIL_DIR)memconsumer.o

TEXT_OBJFILES = \
$(TEXT_DIR)sgltext.o \
$(TEXT_DIR)sglconsole.o \
$(TEXT_DIR)sgl3dtext.o \
$(TEXT_DIR)sgltextbackend.o

TEXT_BACKEND_GLF_OBJFILES = \
$(TEXT_BACKEND_GLF_DIR)glf.o \
$(TEXT_BACKEND_GLF_DIR)sgltextbackend_glf.o

ALL_OBJ_FILES = \
$(CORE_OBJFILES) \
$(HELPER_OBJFILES) \
$(PRIMITIVES_OBJFILES) \
$(UTIL_OBJFILES) \
$(TEXT_OBJFILES) \
$(TEXT_BACKEND_GLF_OBJFILES) \
$(SAMPLE_CLASSES_OBJFILES)


core:	$(CORE_OBJFILES)
	@echo "===========Core done========"

helper: $(HELPER_OBJFILES)
	@echo "===========Helper done========"

primitives: $(PRIMITIVES_OBJFILES)
	@echo "===========Primitives done========"

util:	$(UTIL_OBJFILES)
	@echo "===========Util done========"

sample_classes:	$(SAMPLE_CLASSES_OBJFILES)
	@echo "===========sample Classes done========"

text:	$(TEXT_OBJFILES) $(TEXT_BACKEND_GLF_OBJFILES)
	@echo "===========Text done========"

libsgl.a:	core helper primitives util text sample_classes
	-rm -f libsgl.a
	@echo "===========Linking libsgl.a========"
	$(AR) cqs libsgl.a $(ALL_OBJ_FILES) 

libsgl.so:	core helper primitives util text sample_classes
	-rm -f libsgl.so
	@echo "===========Linking libsgl.so========"
	g++ -shared -o libsgl.so -Wl,-soname,libsgl.so $(ALL_OBJ_FILES) 

libs:	libsgl.a libsgl.so

clean:
	-rm -f $(ALL_OBJ_FILES)
	-rm -f libsgl.*
	-rm -f test test.o

depsclean:
	-rm -f .*depend

install: libs
	mkdir -p $(DESTDIR)/usr/lib
	mkdir -p $(DESTDIR)/usr/share/libsgl
	mkdir -p $(DESTDIR)/usr/include/libsgl/text
	mkdir -p $(DESTDIR)/usr/include/libsgl/primitives
	mkdir -p $(DESTDIR)/usr/include/libsgl/helper
	mkdir -p $(DESTDIR)/usr/include/libsgl/util
	mkdir -p $(DESTDIR)/usr/include/libsgl/text/backend_glf
	mkdir -p $(DESTDIR)/usr/share/libsgl/pics
	mkdir -p $(DESTDIR)/usr/share/libsgl/fonts
	install -m 644 libsgl.a  $(DESTDIR)/usr/lib
	install -m 755 libsgl.so $(DESTDIR)/usr/lib
	install -m 644 data/pics/*   $(DESTDIR)/usr/share/libsgl/pics/
	install -m 644 data/fonts/*   $(DESTDIR)/usr/share/libsgl/fonts/
	for i in `find . -name "*.h"`; do cp $$i $(DESTDIR)/usr/include/libsgl/$$i; done

deps:	depsclean .core_depend .helper_depend .primitives_depend .util_depend .text_depend .text_backend_depend
	@echo "===========Depends done=========="

.core_depend :
	$(CXX) $(CXXFLAGS) -MM $(CORE_DIR)*.c* > $@

.helper_depend :
	$(CXX) $(CXXFLAGS) -MM $(HELPER_DIR)*.c* > $@

.primitives_depend :
	$(CXX) $(CXXFLAGS) -MM $(PRIMITIVES_DIR)*.c* > $@

.util_depend :
	$(CXX) $(CXXFLAGS) -MM $(UTIL_DIR)*.c* > $@

.text_depend :
	$(CXX) $(CXXFLAGS) -MM $(TEXT_DIR)*.c* > $@

.text_backend_depend :
	$(CXX) $(CXXFLAGS) -MM $(TEXT_BACKEND_GLF_DIR)*.c* > $@

.sample_classes_depend :
	$(CXX) $(CXXFLAGS) -MM $(SAMPLE_CLASSES_DIR)*.c* > $@

include .core_depend .helper_depend .primitives_depend .util_depend .text_depend .text_backend_depend .sample_classes_depend
