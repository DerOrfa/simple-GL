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

QT4_WRAP_CPP(MOCS ${HDRS} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED) #-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED is a woraround for https://bugreports.qt.nokia.com/browse/QTBUG-22829

if(SHARED_LIB)
	add_library(qt4_glue SHARED ${SRCS} ${MOCS})
	TARGET_LINK_LIBRARIES(qt4_glue sgl ${QT_LIBRARIES})
	set_target_properties( qt4_glue PROPERTIES SOVERSION ${soversion} VERSION ${version} OUTPUT_NAME qt4_glue INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	INSTALL(TARGETS qt4_glue  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
	#The library target "sgl" already has a default OUTPUT_NAME of "sgl", so we don't need to change it.
endif(SHARED_LIB)

if(STATIC_LIB)
	add_library(qt4_glue_static STATIC ${SRCS} ${MOCS})
	#The library target "sgl_static" has a default OUTPUT_NAME of "sgl_static", can be changed with
	set_target_properties( qt4_glue_static PROPERTIES SOVERSION ${soversion} VERSION ${version} OUTPUT_NAME qt4_glue INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	INSTALL(TARGETS qt4_glue_static  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif(STATIC_LIB)

SET(BUILD_SAMPLES on CACHE BOOL "build qt-based examples")

if(STATIC_LIB)
	set(LIB qt4_glue_static sgl_static)
else(STATIC_LIB)
	set(LIB qt4_glue sgl ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
endif(STATIC_LIB)

if(BUILD_SAMPLES)
	include_directories(${CMAKE_SOURCE_DIR})
	add_executable(qt4_sample1 qt4_sample1.cpp)
	add_executable(qt4_sample2 qt4_sample2.cpp)
	add_executable(qt4_sample3 qt4_sample3.cpp)
	
	target_link_libraries(qt4_sample1 ${QT_LIBRARIES} ${LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
	target_link_libraries(qt4_sample2 ${QT_LIBRARIES} ${LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
	target_link_libraries(qt4_sample3 ${QT_LIBRARIES} ${LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
endif(BUILD_SAMPLES)

INSTALL(FILES ${HDRS} DESTINATION include/libsgl/qt4_glue)
