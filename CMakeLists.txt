cmake_minimum_required(VERSION 3.5.1)

set(TARGET_NAME ptt)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_FLAGS "-g -Werror=unused-parameter")
set(CMAKE_CXX_STANDARD 17)
set(ROOT_ROOT ${CMAKE_SOURCE_DIR}/../..)


get_filename_component(3RD_INCLUDE ${CMAKE_SOURCE_DIR}/3rd/include/ ABSOLUTE)
get_filename_component(3RD_LIBRARY ${CMAKE_SOURCE_DIR}/3rd/lib ABSOLUTE)

get_filename_component(SDL_DISTRO_INCLUDE ${ROOT_ROOT}/SDL/include ABSOLUTE)
get_filename_component(SDL_DISTRO_LIBRARY ${ROOT_ROOT}/SDLMake ABSOLUTE)


#EXECUTABLES TO TEST LIBRARY SOURCES
add_executable(${TARGET_NAME} ptt.cpp)

target_link_libraries(${TARGET_NAME} 
    ${3RD_LIBRARY}/libSDL2.a 
    ${3RD_LIBRARY}/libSDL2.a
    )
target_link_libraries(${TARGET_NAME}
    "-framework Carbon" 
    "-framework ForceFeedback" 
    "-framework IOKit" 
    "-framework Cocoa" 
    "-framework CoreVideo"
    "-framework AudioToolbox"
    "-framework CoreAudio"
    "-framework Foundation"
    "-framework AppKit"
    "-framework GLUT"
    "-framework OpenGL"
    )
target_link_libraries(${TARGET_NAME} "-liconv" "-lm")


#PRE BUILD COMMANDS 
FILE(GLOB 3RD_STATIC_LIBRARIES
    "${SDL_DISTRO_LIBRARY}/libSDL2*"
    )
FILE(GLOB 3RD_HEADER_INCLUDES
    "${SDL_DISTRO_INCLUDE}/*.*"
    )
FILE(COPY 
    ${3RD_STATIC_LIBRARIES}
    DESTINATION ${3RD_LIBRARY}
)
FILE(COPY 
    ${3RD_HEADER_INCLUDES}
    DESTINATION ${3RD_INCLUDE}
)

#INCLUDE DIRECTORIES 
include_directories(
    ${3RD_INCLUDE}
    )


