cmake_minimum_required(VERSION 2.6)

PROJECT(libsgl)
cmake_policy(SET CMP0005 OLD) #make it not complain about escaped Preprocessor definition (can be removed when we switch to cmake 2.6)
INCLUDE(CPack)

SET(TARGET_VERSION_MAJOR 0)
SET(TARGET_VERSION_MINOR 9)
SET(CPACK_PACKAGE_CONTACT "reimer@cbs.mpg.de")

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Boost REQUIRED COMPONENTS signals)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

SET(SYS_DATADIR "share" CACHE PATH "systems dir for data (excl. prefix)")

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
  ${HELPER_DIR}sgllensflare.cpp 
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
  ${UTIL_DIR}readtex.c
  ${UTIL_DIR}sglmaterial.cpp
  ${UTIL_DIR}sgltextur.cpp
#  ${UTIL_DIR}sglshader.cpp #GLSL not fully supported by all current GL implementations - so skip it till we find clean solution
  ${UTIL_DIR}sglimagefile.cpp
  ${UTIL_DIR}memconsumer.cpp
)

SET(USE_DEVIL off CACHE BOOL "use DevIL (aka OpenIL) for texture loading")
SET(STATIC_LIB off CACHE BOOL "create static libs as well")
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
SET(TEXT_OBJFILES ${TEXT_DIR}sgltext.cpp ${TEXT_DIR}sglconsole.cpp ${TEXT_DIR}sgl3dtext.cpp ${TEXT_DIR}sgltextbackend.cpp)

SET(TEXT_BACKEND_GLF_DIR ${TEXT_DIR}backend_glf/)
SET(TEXT_BACKEND_GLF_OBJFILES ${TEXT_BACKEND_GLF_DIR}glf.c ${TEXT_BACKEND_GLF_DIR}sgltextbackend_glf.cpp)


ADD_DEFINITIONS(-DDATA_DIR="\\\"${CMAKE_INSTALL_PREFIX}/${SYS_DATADIR}/libsgl\\\"" -DILUT_USE_OPENGL -DGL_GLEXT_PROTOTYPES -DBOOST_HAS_PTHREADS -ffast-math -Wall)
INCLUDE_DIRECTORIES(${GL_EXT_INCLUDES} ${OPENGL_INCLUDE_DIR} ${DEVIL_INCS})

ADD_LIBRARY(sgl SHARED 
  ${CORE_OBJFILES} 
  ${HELPER_OBJFILES} 
  ${PRIMITIVES_OBJFILES} 
  ${UTIL_OBJFILES} 
  ${TEXT_OBJFILES} 
  ${TEXT_BACKEND_GLF_OBJFILES} 
)
#The library target "sgl" already has a default OUTPUT_NAME of "sgl", so we don't need to change it.

#${OPENGL_LIBRARIES} #DONT Link libGL here - confuses QT's OpenGL-Interface
TARGET_LINK_LIBRARIES(sgl ${Boost_LIBRARIES} ${DEVIL_LIBS} ${OPENGL_LIBRARIES} m)
SET(LIBS sgl)

if(STATIC_LIB)
	ADD_LIBRARY(sgl_static STATIC   ${CORE_OBJFILES}
		${HELPER_OBJFILES}
		${PRIMITIVES_OBJFILES}
		${UTIL_OBJFILES}
		${TEXT_OBJFILES}
		${TEXT_BACKEND_GLF_OBJFILES}
	)
	#The library target "sgl_static" has a default OUTPUT_NAME of "sgl_static", can be changed with
	SET_TARGET_PROPERTIES(sgl_static PROPERTIES OUTPUT_NAME "sgl")
	SET(LIBS ${LIBS} sgl_static)
endif(STATIC_LIB)
	
INSTALL(TARGETS ${LIBS}  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

FOREACH(inc ${includes})
  GET_FILENAME_COMPONENT(inc_path ${inc} PATH)
  INSTALL(FILES ${inc} DESTINATION include/libsgl/${inc_path})
ENDFOREACH(inc)

INSTALL(DIRECTORY data/ DESTINATION ${CMAKE_INSTALL_PREFIX}/${SYS_DATADIR}/libsgl PATTERN ".svn" EXCLUDE)

SET(BUILD_QT4_GLUE off CACHE BOOL "build qt4_glue (needs qt4)")

if(BUILD_QT4_GLUE)
	add_subdirectory(qt4_glue)
endif(BUILD_QT4_GLUE)

#README
#rm -rf `svn status|grep ^?|colrm 1 7`
#CC=distcc CXX="distcc g++" cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=release
#DISTCC_HOSTS="localhost timmy" make -j3
