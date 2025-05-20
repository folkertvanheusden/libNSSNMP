cmake_minimum_required(VERSION 3.12)

project(libnssnmp VERSION 4.0.0 DESCRIPTION "a simple snmp agent library")

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_compile_options(-Wall -pedantic)

add_library(libnssnmp SHARED
	src/block.cpp
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
    FILES_MATCHING PATTERN "snmp*.h"
)

target_include_directories(libnssnmp PRIVATE include)

install(TARGETS libnssnmp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libnssnmp)

configure_file(libnssnmp.pc.in libnssnmp.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/libnssnmp.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads)

# ----

SET(INSTALL_GTEST OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

SET(LINKING_STUFF
  GTest::gtest_main
  Threads::Threads
)

add_executable(
  unittest-block
  src/test/block-unittest.cpp
  src/block.cpp
)
target_compile_options(unittest-block PRIVATE -DUNITTEST)
target_link_libraries(
  unittest-block
  ${LINKING_STUFF}
)

add_executable(
  unittest-snmp
  src/test/snmp-unittest.cpp
  src/block.cpp
  src/snmp.cpp
  src/snmp_data.cpp
  src/snmp_elem.cpp
  src/utils.cpp
)
target_compile_options(unittest-snmp PRIVATE -DUNITTEST)
target_include_directories(unittest-snmp PRIVATE include)
target_link_libraries(
  unittest-snmp
  ${LINKING_STUFF}
)
