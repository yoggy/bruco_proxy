cmake_minimum_required(VERSION 2.8)

PROJECT(bruco_proxy)

find_library(RE2_LIBRARY
	NAMES re2
	PATHS
		/usr/lib
		/usr/local/lib)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_C_FLAGS "-g -Wall")

include_directories(/usr/local/include)

set(src_utils log.cpp config.cpp lock.cpp string.cpp tcp.cpp)
set(src_tcp  tcp_server.cpp tcp_session.cpp proxy_session.cpp)
set(src_bruco_proxy main.cpp bruco_server.cpp bruco_session.cpp ${src_utils} ${src_tcp})

add_executable(bruco_proxy ${src_bruco_proxy})

target_link_libraries(bruco_proxy ${RE2_LIBRARY})
INSTALL(TARGETS bruco_proxy RUNTIME DESTINATION bin)
