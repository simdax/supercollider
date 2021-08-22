cmake_minimum_required(VERSION 3.21)

project(unity)
# add_executable(unity server.cpp main.cpp)
add_library(unity SHARED server.cpp main.cpp)

target_compile_definitions(unity PUBLIC
    BUILDING_SCLANG
)
target_include_directories(unity PUBLIC
    ../common
    ../include
    ../include/common
    ../include/server
    ../build/common
    ../include/lang
    ../lang/LangSource
    ../external_libraries/boost
    ../external_libraries/boost_sync/include
)
target_link_directories(unity PUBLIC
    ../build/lang/Debug
    ../build/server/scsynth/Debug
    ../build/external_libraries
    C:/Users/scornaz/Downloads/boost_1_74_0/stage/lib
)       
target_link_libraries(unity
    libsclang
    libscsynth
    ws2_32
)
