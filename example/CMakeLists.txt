project(snmp_example)
cmake_minimum_required(VERSION 3.9)

add_compile_options(-Wall -pedantic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
	snmp_example
	example.cpp
)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads)
target_link_libraries(snmp_example Threads::Threads)

include(FindPkgConfig)

pkg_check_modules(LIBNSSNMP REQUIRED libnssnmp)
target_link_libraries(snmp_example ${LIBNSSNMP_LIBRARIES})
target_include_directories(snmp_example PUBLIC ${LIBNSSNMP_INCLUDE_DIRS})
target_compile_options(snmp_example PUBLIC ${LIBNSSNMP_CFLAGS_OTHER})
