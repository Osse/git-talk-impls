cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_CXX_STANDARD 17)

project(gittalk CXX)

find_package(fmt REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

foreach(prog printobject writeblob writecommit writetree checkouttree)
    add_executable(${prog} ${prog}.cpp writeobject.cpp)
    target_link_libraries(${prog} stdc++fs fmt::fmt ZLIB::ZLIB OpenSSL::Crypto)
    target_include_directories(${prog} PUBLIC SYSTEM 3rdparty)
endforeach()
