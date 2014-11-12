cmake_minimum_required(VERSION 2.6)

PROJECT(libsgl)
INCLUDE(CPack)

SET(TARGET_VERSION_MAJOR 0)
SET(TARGET_VERSION_MINOR 11)
SET(CPACK_PACKAGE_CONTACT "reimer@cbs.mpg.de")

set(version ${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR} CACHE INTERNAL "The current Version of the sgl")
set(soversion ${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR} CACHE INTERNAL "The current Version of the sgl")


FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Freetype REQUIRED)
FIND_PACKAGE(Boost 1.39 REQUIRED)


find_path(FTGL_INCLUDE_DIR "FTGL/ftgl.h")
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${FTGL_INCLUDE_DIR})

find_library(LIBFTGL ftgl)

find_file(DEFAULT_TTF NAMES FreeMonoBold.ttf FreeSansBold.ttf CACHE PATH_SUFFIXES "share/fonts/truetype/freefont" DOC "path to default truetype font")

SET(CORE_DIR ./)
SET(CORE_OBJFILES  
  ${CORE_DIR}sglmisc.c
  ${CORE_DIR}sglobj.cpp 
  ${CORE_DIR}sglobjbase.cpp 
  ${CORE_DIR}sglspace.cpp 
  ${CORE_DIR}sglvektor.cpp 
  ${CORE_DIR}sglmetaobj.cpp 
  ${CORE_DIR}sglmatrixobj.cpp
)

SET(HELPER_DIR helper/)
SET(HELPER_OBJFILES 
  ${HELPER_DIR}sglcamera.cpp 
  ${HELPER_DIR}sglclipplane.cpp 
  ${HELPER_DIR}sglgrid.cpp 
  ${HELPER_DIR}sglhelper.cpp 
  ${HELPER_DIR}sgllight.cpp 
  ${HELPER_DIR}sgldisplist.cpp
)

SET(PRIMITIVES_DIR primitives/)
SET(PRIMITIVES_OBJFILES 
  ${PRIMITIVES_DIR}sglflobj.cpp
  ${PRIMITIVES_DIR}sglgeosphere.cpp
  ${PRIMITIVES_DIR}sglicosahedron.cpp
  ${PRIMITIVES_DIR}sglloft.cpp
  ${PRIMITIVES_DIR}sglneck.cpp
  ${PRIMITIVES_DIR}sglpolygonobj.cpp
  ${PRIMITIVES_DIR}sglquader.cpp 
  ${PRIMITIVES_DIR}sglvieleck.cpp 
  ${PRIMITIVES_DIR}sglquadricobj.cpp
)

SET(UTIL_DIR util/)
SET(UTIL_OBJFILES 
  ${UTIL_DIR}sglmaterial.cpp
  ${UTIL_DIR}sgltextur.cpp
#  ${UTIL_DIR}sglshader.cpp #GLSL not fully supported by all current GL implementations - so skip it till we find clean solution
  ${UTIL_DIR}memconsumer.cpp
)

SET(USE_DEVIL off CACHE BOOL "use DevIL (aka OpenIL) for texture loading")
SET(STATIC_LIB off CACHE BOOL "create static libs as well")
SET(SHARED_LIB on CACHE BOOL "create sahred libs as well")

IF(USE_DEVIL)
ADD_DEFINITIONS(-DUSE_DEVIL)
	find_path(DEVIL_INCLUDE_DIR "IL/ilut.h")
	find_library(IL_LIBRARY NAMES "IL")
	find_library(ILU_LIBRARY NAMES "ILU")
	find_library(ILUT_LIBRARY NAMES "ILUT")
	set(DEVIL_LIBS ${IL_LIBRARY} ${ILU_LIBRARY} ${ILUT_LIBRARY})
	set(DEVIL_INCS ${DEVIL_INCLUDE_DIR})
ENDIF(USE_DEVIL)

FILE(GLOB_RECURSE includes RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

SET(TEXT_DIR text/)
SET(TEXT_OBJFILES ${TEXT_DIR}sgltext.cpp ${TEXT_DIR}sglconsole.cpp ${TEXT_DIR}sgl3dtext.cpp)

ADD_DEFINITIONS(-DDEFAULT_TTF="${DEFAULT_TTF}" -DILUT_USE_OPENGL -ffast-math -Wall)
INCLUDE_DIRECTORIES(${GL_EXT_INCLUDES} ${OPENGL_INCLUDE_DIR} ${DEVIL_INCS})

if(SHARED_LIB)
	ADD_LIBRARY(sgl SHARED
		${CORE_OBJFILES}
		${HELPER_OBJFILES}
		${PRIMITIVES_OBJFILES}
		${UTIL_OBJFILES}
		${TEXT_OBJFILES}
	)
	#The library target "sgl" already has a default OUTPUT_NAME of "sgl", so we don't need to change it.

	#${OPENGL_LIBRARIES} #DONT Link libGL here - confuses QT's OpenGL-Interface
	TARGET_LINK_LIBRARIES(sgl ${DEVIL_LIBS} ${OPENGL_LIBRARIES} ${LIBFTGL} ${FREETYPE_LIBRARIES} m)
	set_target_properties( sgl PROPERTIES SOVERSION ${soversion} VERSION ${version} OUTPUT_NAME sgl INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	list(APPEND LIBS sgl)
endif(SHARED_LIB)

if(STATIC_LIB)
	find_library(LIBFTGL_STATIC "${CMAKE_STATIC_LIBRARY_PREFIX}ftgl${CMAKE_STATIC_LIBRARY_SUFFIX}")
	ADD_LIBRARY(sgl_static STATIC   ${CORE_OBJFILES}
		${HELPER_OBJFILES}
		${PRIMITIVES_OBJFILES}
		${UTIL_OBJFILES}
		${TEXT_OBJFILES}
	)
	#The library target "sgl_static" has a default OUTPUT_NAME of "sgl_static", can be changed with
	set_target_properties( sgl_static PROPERTIES SOVERSION ${soversion} VERSION ${version} OUTPUT_NAME sgl INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	list(APPEND LIBS sgl_static)
	TARGET_LINK_LIBRARIES(sgl_static ${DEVIL_LIBS} ${LIBFTGL_STATIC} ${FREETYPE_LIBRARIES} m)
endif(STATIC_LIB)
	
INSTALL(TARGETS ${LIBS}  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

FOREACH(inc ${includes})
  GET_FILENAME_COMPONENT(inc_path ${inc} PATH)
  INSTALL(FILES ${inc} DESTINATION include/libsgl/${inc_path})
ENDFOREACH(inc)

SET(BUILD_QT4_GLUE off CACHE BOOL "build qt4_glue (needs qt4)")

if(BUILD_QT4_GLUE)
	add_subdirectory(qt4_glue)
endif(BUILD_QT4_GLUE)
