cmake_minimum_required(VERSION 3.0.0)

project(test1 VERSION 0.1.0 LANGUAGES C CXX OBJCXX)

set(FFMPEG ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg)
set(GLFW ${CMAKE_CURRENT_SOURCE_DIR}/glfw)

add_compile_definitions("GL_SILENCE_DEPRECATION")

include_directories(${FFMPEG}/include ${GLFW}/include)
link_directories(${FFMPEG}/lib ${GLFW}/lib-x86_64)
list(APPEND SOURCES
    src/main.cpp
    src/tools/display_texture.cpp
    src/tools/load_ffmpeg.cpp
    # src/tools/display_texture.hpp
    # src/tools/load_ffmpeg.hpp
)

set(CMAKE_CXX_STANDARD 17)
add_executable(test1 ${SOURCES})
list(APPEND EXTRA_LIBS 
    "-framework Foundation"
    "-framework CoreGraphics"
    "-framework IOKIT"
    "-framework AppKit"
    "-framework OpenGl")
list(APPEND ALL_LINK
    test1 
    avcodec 
    avdevice 
    avfilter 
    avformat 
    avutil 
    glfw3 
    ${EXTRA_LIBS}
)
target_link_libraries(${ALL_LINK})
