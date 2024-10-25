cmake_minimum_required(VERSION 3.28)
project(AsioServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(./)

find_package(Boost REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB UtilitySourceFile "Utility/*.cpp")
add_library(Utility STATIC ${UtilitySourceFile})

file(GLOB BaseFrameSourceFile "BaseFrame/*.cpp" )
add_library(BaseFrame STATIC ${BaseFrameSourceFile})
target_link_libraries(BaseFrame Utility)

file(GLOB CgiSourceFile "Cgi/*.cpp")
add_library(Cgi STATIC ${CgiSourceFile})
target_link_libraries(Cgi Utility Boost::filesystem)

add_executable(main main.cpp)
target_link_libraries(main BaseFrame Cgi)

add_executable(test_main test_main.cpp)
target_link_libraries(test_main BaseFrame Cgi)
