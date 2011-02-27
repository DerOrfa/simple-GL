cmake_minimum_required(VERSION 2.6)

#this will find qt4 and set the include-dirs for the modules QtOpenGL QtGui QtCore
find_package(Qt4 REQUIRED )
set(QT_USE_QTOPENGL TRUE)
INCLUDE(${QT_USE_FILE})

set(QT_USE_OPENGL TRUE)
set(DEFINES -DGL_GLEXT_PROTOTYPES -DBOOST_HAS_PTHREADS)
add_definitions(-DGL_GLEXT_PROTOTYPES -DBOOST_HAS_PTHREADS -DQT_NO_KEYWORDS)

set(SRCS sglqtmultispacemgr.cpp sglqtspace.cpp)
set(HDRS sglqtmultispacemgr.h sglqtspace.h)

QT4_WRAP_CPP(MOCS ${HDRS})

add_library(qt4_glue SHARED ${SRCS} ${MOCS})
INSTALL(TARGETS qt4_glue  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
#The library target "sgl" already has a default OUTPUT_NAME of "sgl", so we don't need to change it.

if(STATIC_LIB)
	add_library(qt4_glue_static STATIC ${SRCS} ${MOCS})
	#The library target "sgl_static" has a default OUTPUT_NAME of "sgl_static", can be changed with
	SET_TARGET_PROPERTIES(qt4_glue_static PROPERTIES OUTPUT_NAME "qt4_glue")
	INSTALL(TARGETS qt4_glue_static  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif(STATIC_LIB)

INSTALL(FILES ${HDRS} DESTINATION include/libsgl/${inc_path})
