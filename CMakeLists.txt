#libsgl master cmake-file
PROJECT(libsgl)
INCLUDE(FindOpenGL)

SET(TARGET_VERSION_MAJOR 0)
SET(TARGET_VERSION_MINOR 9)


FIND_LIBRARY(BOOST_SIGNALS_LIB boost_signals)
FIND_LIBRARY(ECLASSES2_LIB eclasses2)

SET(SYS_DATADIR "${CMAKE_INSTALL_PREFIX}/share/libsgl" CACHE PATH "systems dir for data")
#SET(GL_EXT_INCLUDES "${OPENGL_INCLUDE_DIR}/GL" CACHE PATH "directory with glext.h and glxext.h")

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

#helper 
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
  ${UTIL_DIR}sglimagefile.cpp
  ${UTIL_DIR}memconsumer.cpp
)

SET(TEXT_DIR text/)
SET(TEXT_OBJFILES ${TEXT_DIR}sgltext.cpp ${TEXT_DIR}sglconsole.cpp ${TEXT_DIR}sgl3dtext.cpp ${TEXT_DIR}sgltextbackend.cpp)

SET(TEXT_BACKEND_GLF_DIR ${TEXT_DIR}backend_glf/)
SET(TEXT_BACKEND_GLF_OBJFILES ${TEXT_BACKEND_GLF_DIR}glf.c ${TEXT_BACKEND_GLF_DIR}sgltextbackend_glf.cpp)


ADD_DEFINITIONS(-DDATA_DIR=\\"${SYS_DATADIR}\\" -DGL_GLEXT_PROTOTYPES -DBOOST_HAS_PTHREADS -ffast-math -pthread -Wall)
INCLUDE_DIRECTORIES(${GL_EXT_INCLUDES} ${OPENGL_INCLUDE_DIR})

ADD_LIBRARY(sgl SHARED 
  ${CORE_OBJFILES} 
  ${HELPER_OBJFILES} 
  ${PRIMITIVES_OBJFILES} 
  ${UTIL_OBJFILES} 
  ${TEXT_OBJFILES} 
  ${TEXT_BACKEND_GLF_OBJFILES} 
)
#The library target "sgl" already has a default OUTPUT_NAME of "sgl", so we don't need to change it.
#ADD_LIBRARY(ssgl STATIC   ${CORE_OBJFILES} 
#  ${HELPER_OBJFILES} 
#  ${PRIMITIVES_OBJFILES} 
#  ${UTIL_OBJFILES} 
#  ${TEXT_OBJFILES} 
#  ${TEXT_BACKEND_GLF_OBJFILES} 
#)
#The library target "ssgl" has a default OUTPUT_NAME of "ssgl", so change it.
#SET_TARGET_PROPERTIES(ssgl PROPERTIES OUTPUT_NAME "sgl")

TARGET_LINK_LIBRARIES(sgl ${OPENGL_LIBRARIES} ${BOOST_SIGNALS_LIB} ${ECLASSES2_LIB} m)

#INSTALL(TARGETS sgl DESTINATION libs)


