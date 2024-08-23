cmake_minimum_required(VERSION 3.9)

project(libnssnmp VERSION 1.1.0 DESCRIPTION "a simple snmp agent library")

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_compile_options(-Wall -pedantic)

add_library(libnssnmp SHARED
	src/snmp.cpp
	src/snmp_data.cpp
	src/snmp_elem.cpp
	src/utils.cpp
)

include(GNUInstallDirs)
set_target_properties(libnssnmp PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(libnssnmp PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libnssnmp
    FILES_MATCHING PATTERN "*.h"
)

target_include_directories(libnssnmp PRIVATE include)

install(TARGETS libnssnmp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libnssnmp)

configure_file(libnssnmp.pc.in libnssnmp.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/libnssnmp.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
