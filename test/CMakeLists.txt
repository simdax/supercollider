cmake_minimum_required(VERSION 3.21)

project(unity)
add_library(unity SHARED server.cpp)

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
    ../external_libraries/yaml-cpp/include
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

add_executable(unity_bin main.cpp)
target_link_libraries(unity_bin unity libsclang)

add_custom_command(TARGET unity_bin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/../build/lang/Debug/libsclang.dll
    ${CMAKE_SOURCE_DIR}/../build/server/scsynth/Debug/libscsynth.dll
    ${CMAKE_SOURCE_DIR}/../build/server/scsynth/Debug/libsndfile-1.dll
    ${CMAKE_BINARY_DIR}/Debug
)
# add_custom_command(TARGET unity_bin POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/../SCClassLibrary
#     ${CMAKE_BINARY_DIR}/Debug/SCClassLibrary
# )
# add_custom_command(TARGET unity_bin POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/../build/server/scsynth/Debug/plugins
#     ${CMAKE_BINARY_DIR}/Debug/plugins
# )
