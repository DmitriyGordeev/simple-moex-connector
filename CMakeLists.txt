cmake_minimum_required(VERSION 3.20)
project(socket_examples)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS system filesystem REQUIRED)
set(Boost_USE_STATIC_LIBS   ON)

add_subdirectory(boost_asio_example)

add_executable(tcp_socket_server tcp_server.cpp)
add_executable(tcp_socket_client tcp_client.cpp)

add_executable(udp_socket_server udp_server.cpp)
add_executable(udp_socket_client udp_client.cpp)

# UDP multicast listener / sender
add_executable(udp_multicast_listener udp_multicast_listener.cpp)
add_executable(udp_multicast_sender udp_multicast_sender.cpp)

add_executable(thread_udp_multicast_listener thread_udp_multicast_listener.cpp)
target_link_libraries(thread_udp_multicast_listener
        pthread)
