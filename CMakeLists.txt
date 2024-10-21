cmake_minimum_required(VERSION 3.28)
project(AsioServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(./)

include(FetchContent)
FetchContent_Declare(Boost SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/boost)
FetchContent_MakeAvailable(Boost)

execute_process(
        COMMAND find "${CMAKE_SOURCE_DIR}/3rd/boost/libs" -type d -name include
        OUTPUT_VARIABLE BoostHeaders
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "\n" ";" BoostHeaders "${BoostHeaders}")

foreach(dir ${BoostHeaders})
    message(STATUS "Found include directory: ${dir}")
    include_directories(${dir})
endforeach()

file(GLOB UtilitySourceFile "Utility/*.cpp")
add_library(Utility STATIC ${UtilitySourceFile})

file(GLOB BaseFrameSourceFile "BaseFrame/*.cpp" )
add_library(BaseFrame STATIC ${BaseFrameSourceFile})
target_link_libraries(BaseFrame Utility)

file(GLOB CgiSourceFile "Cgi/*.cpp")
add_library(Cgi STATIC ${CgiSourceFile})
target_link_libraries(Cgi Boost::filesystem Utility)

add_executable(main main.cpp)
target_link_libraries(main BaseFrame Cgi)

add_executable(test_main test_main.cpp)
target_link_libraries(test_main BaseFrame Cgi)
