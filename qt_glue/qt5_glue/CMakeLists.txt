cmake_minimum_required(VERSION 2.8.6)

#this will find qt4 and set the include-dirs for the modules QtOpenGL QtGui QtCore
find_package(Qt5 COMPONENTS OpenGL)
set(CMAKE_AUTOMOC TRUE)

foreach(LOOP ${HDRS})
	list(APPEND Q5HDRS "../${LOOP}")
endforeach(LOOP)

foreach(LOOP ${SRCS})
	list(APPEND Q5SRCS "../${LOOP}")
endforeach(LOOP)

include_directories(${Qt5OpenGL_INCLUDE_DIRS})

if(SHARED_LIB)
	add_library(sgl_qt5 SHARED ${Q5SRCS})
	TARGET_LINK_LIBRARIES(sgl_qt5 sgl  ${Qt5OpenGL_LIBRARIES})
	set_target_properties( sgl_qt5 PROPERTIES SOVERSION ${soversion} VERSION ${version} OUTPUT_NAME sgl_qt5 INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	INSTALL(TARGETS sgl_qt5  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
	#The library target "sgl" already has a default OUTPUT_NAME of "sgl", so we don't need to change it.
endif(SHARED_LIB)
 
if(STATIC_LIB)
	add_library(sgl_qt5_static STATIC ${Q5SRCS})
	#The library target "sgl_static" has a default OUTPUT_NAME of "sgl_static", can be changed with
	set_target_properties( sgl_qt5_static PROPERTIES SOVERSION ${soversion} VERSION ${version} OUTPUT_NAME sgl_qt5 INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	INSTALL(TARGETS sgl_qt5_static  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif(STATIC_LIB)
 
if(BUILD_QT_SAMPLES)
	include_directories(${CMAKE_SOURCE_DIR})
	if(STATIC_LIB)
		set(LIB sgl_qt5_static sgl_static ${Qt5OpenGL_LIBRARIES})
	else(STATIC_LIB)
		set(LIB sgl_qt5 sgl)
	endif(STATIC_LIB)
	message(STATUS "linking samples against ${LIB}")
	
	list(APPEND CMAKE_CXX_FLAGS ${Qt5OpenGL_EXECUTABLE_COMPILE_FLAGS})
	add_executable(qt5_sample1 ../sample1.cpp)
	add_executable(qt5_sample2 ../sample2.cpp)
	add_executable(qt5_sample3 ../sample3.cpp)
	add_executable(qt5_stlsample ../stlsample.cpp)
	
	target_link_libraries(qt5_sample1 ${LIB} ${OPENGL_LIBRARIES})
	target_link_libraries(qt5_sample2 ${LIB} ${OPENGL_LIBRARIES})
	target_link_libraries(qt5_sample3 ${LIB} ${OPENGL_LIBRARIES})
	target_link_libraries(qt5_stlsample ${LIB} ${OPENGL_LIBRARIES})
endif(BUILD_QT_SAMPLES)


