cmake_minimum_required(VERSION 2.6)

#this will find qt4 and set the include-dirs for the modules QtOpenGL QtGui QtCore
find_package(Qt4 REQUIRED )
set(QT_USE_QTOPENGL TRUE)
INCLUDE(${QT_USE_FILE})

foreach(LOOP ${HDRS})
	list(APPEND Q4HDRS "../${LOOP}")
endforeach(LOOP)

foreach(LOOP ${SRCS})
	list(APPEND Q4SRCS "../${LOOP}")
endforeach(LOOP)


QT4_WRAP_CPP(MOCS ${Q4HDRS} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED) #-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED is a woraround for https://bugreports.qt.nokia.com/browse/QTBUG-22829

if(SHARED_LIB)
	add_library(sgl_qt4 SHARED ${Q4SRCS} ${MOCS})
	TARGET_LINK_LIBRARIES(sgl_qt4 sgl ${QT_LIBRARIES})
	set_target_properties( sgl_qt4 PROPERTIES SOVERSION ${soversion} VERSION ${version} OUTPUT_NAME sgl_qt4 INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	INSTALL(TARGETS sgl_qt4  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
	#The library target "sgl" already has a default OUTPUT_NAME of "sgl", so we don't need to change it.
endif(SHARED_LIB)

if(STATIC_LIB)
	add_library(sgl_qt4_static STATIC ${Q4SRCS} ${MOCS})
	#The library target "sgl_static" has a default OUTPUT_NAME of "sgl_static", can be changed with
	set_target_properties( sgl_qt4_static PROPERTIES SOVERSION ${soversion} VERSION ${version} OUTPUT_NAME sgl_qt4 INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	INSTALL(TARGETS sgl_qt4_static  LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif(STATIC_LIB)

if(BUILD_QT_SAMPLES)
	include_directories(${CMAKE_SOURCE_DIR})
	if(STATIC_LIB)
		set(LIB sgl_qt4_static sgl_static)
	else(STATIC_LIB)
		set(LIB sgl_qt4 sgl ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
	endif(STATIC_LIB)
	message(STATUS "linking samples against ${LIB}")

	add_executable(qt4_sample1 qt4_sample1.cpp)
	add_executable(qt4_sample2 qt4_sample2.cpp)
	add_executable(qt4_sample3 qt4_sample3.cpp)
	add_executable(qt4_stlsample ../stlsample.cpp)

	
	target_link_libraries(qt4_sample1 ${QT_LIBRARIES} ${LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
	target_link_libraries(qt4_sample2 ${QT_LIBRARIES} ${LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
	target_link_libraries(qt4_sample3 ${QT_LIBRARIES} ${LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
	target_link_libraries(qt4_stlsample ${QT_LIBRARIES} ${LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
endif(BUILD_QT_SAMPLES)


