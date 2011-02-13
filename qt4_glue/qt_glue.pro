# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: .
# Das Target ist eine Bibliothek:  

INSTALLS += headers target 
target.path = /tmp/lib
headers.files += sglqtmultispacemgr.h sglqtspace.h 
headers.path = /tmp/libsgl/qt4_glue

INCLUDEPATH += /home/enni/simpleGL
DEFINES = GL_GLEXT_PROTOTYPES BOOST_HAS_PTHREADS
CONFIG += debug warn_on qt thread dll no_keywords
VERSION = 0.7
TEMPLATE = lib 
HEADERS += sglqtmultispacemgr.h sglqtspace.h 
SOURCES += sglqtmultispacemgr.cpp sglqtspace.cpp 
CONFIG -= release

QT += opengl

