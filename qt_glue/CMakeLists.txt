set(SRCS sglqtmultispacemgr.cpp sglqtspace.cpp)
set(HDRS sglqtmultispacemgr.h sglqtspace.h)

SET(BUILD_QT_SAMPLES on CACHE BOOL "build qt-based examples")

if(BUILD_QT4_GLUE)
	add_subdirectory(qt4_glue)
endif(BUILD_QT4_GLUE)

if(BUILD_QT5_GLUE)
	add_subdirectory(qt5_glue)
endif(BUILD_QT5_GLUE)

if(BUILD_QT4_GLUE AND BUILD_QT5_GLUE)
	message(WARNING "Buildung glue for Qt4 AND Qt5 will very likely lots of trouble, be sure you know waht you're doing")
endif(BUILD_QT4_GLUE AND BUILD_QT5_GLUE)

INSTALL(FILES ${HDRS} DESTINATION include/libsgl/qt_glue)