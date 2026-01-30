include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(Framebuffer
  Framebuffer.cpp
)
target_link_libraries(Framebuffer cs4212-util)
target_link_libraries(Framebuffer ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(Framebuffer PNG::PNG)
target_link_libraries(Framebuffer ZLIB::ZLIB)

add_library(renderLib
    vec3.cpp
    vec3.h
)

target_include_directories(renderLib PUBLIC
    ${CMAKE_SOURCE_DIR}
)
