cmake_minimum_required(VERSION 3.23)
project(GCmL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -flto  -lstdc++")

# Boost
set(Boost_NO_WARN_NEW_VERSIONS 1)
set(Boost_USE_STATIC_LIBS  ON)
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})


add_executable(GCmL src/Traversal.cpp src/Traversal.h src/Generator.cpp src/Generator.h src/main.cpp)
target_link_libraries(GCmL Boost::program_options)
