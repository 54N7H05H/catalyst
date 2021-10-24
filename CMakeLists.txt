##
##   ▄████▄   ▄▄▄     ▄▄▄█████▓ ▄▄▄       ██▓   ▓██   ██▓  ██████ ▄▄▄█████▓
##  ▒██▀ ▀█  ▒████▄   ▓  ██▒ ▓▒▒████▄    ▓██▒    ▒██  ██▒▒██    ▒ ▓  ██▒ ▓▒
##  ▒▓█    ▄ ▒██  ▀█▄ ▒ ▓██░ ▒░▒██  ▀█▄  ▒██░     ▒██ ██░░ ▓██▄   ▒ ▓██░ ▒░
##  ▒▓▓▄ ▄██▒░██▄▄▄▄██░ ▓██▓ ░ ░██▄▄▄▄██ ▒██░     ░ ▐██▓░  ▒   ██▒░ ▓██▓ ░ 
##  ▒ ▓███▀ ░ ▓█   ▓██▒ ▒██▒ ░  ▓█   ▓██▒░██████▒ ░ ██▒▓░▒██████▒▒  ▒██▒ ░ 
##  ░ ░▒ ▒  ░ ▒▒   ▓▒█░ ▒ ░░    ▒▒   ▓▒█░░ ▒░▓  ░  ██▒▒▒ ▒ ▒▓▒ ▒ ░  ▒ ░░   
##   ░  ▒     ▒   ▒▒ ░   ░      ▒   ▒▒ ░░ ░ ▒  ░▓██ ░▒░ ░ ░▒  ░ ░    ░    
##   ░          ░   ▒    ░        ░   ▒     ░ ░   ▒ ▒ ░░  ░  ░  ░    ░      
##    ░ ░            ░  ░              ░  ░    ░  ░░ ░           ░           
##    ░                                            ░ ░                       
##
##  Copyright (c) 2020 - present, Santhosh K (0xCAFECODE)
##
##  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
##  files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
##  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
##  Software is furnished to do so, subject to the following conditions:
##
##  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
##  Software.
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
##  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
##  OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
##  OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##
##  This file is part of the "Catalyst Game Engine".
##
cmake_minimum_required(VERSION 3.0.0)

##
##  Project initialization, source tree management and version control
##
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR
        "In-source builds are not allowed by the catalyst project. It is done so as to vaoid polluting the source tree with
        build files. Please create a dedicated build folder such as 'build' and run cmake from there. This process should
        have created CMakeSources.txt and CMakeConfig directory. Please remove them accordingly."    
    )
endif()

if(NOT DEFINED CATALYST_VERSION)
    set(CATALYST_VERSION_MAJOR  0)
    set(CATALYST_VERSION_MINOR  1)
    set(CATALYST_VERSION_PATCH  0)

    set(CATALST_VERSION "${CATALYST_VERSION_MAJOR}.${CATALYST_VERSION_MINOR}.${CATALYST_VERSION_PATCH}")
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(
        catalyst
        VERSION ${CATALYST_VERSION_MAJOR}.${CATALYST_VERSION_MINOR}.${CATALYST_VERSION_PATCH}
        LANGUAGES C
    )
    set(CATALYST_STANDALONE_BUILD   TRUE)
else()
    set(CATALYST_STANDALONE_BUILD   FALSE)
endif()

##
##  Project output configuration
##
##  Configuration for different types of output produced by the buildsystem. This includes documentation, unit-tests and
##  example programs. These outputs are, by default, enabled in standalone builds and disabled in dependency builds
##
option(CATALYST_BUILD_DOCUMENTAION  "Build the catalyst documentation"      ${CATALYST_STANDALONE_BUILD})
option(CATALYST_BUILD_TEST          "Build unit-tests for catalyst"         ${CATALYST_STANDALONE_BUILD})
option(CATALYST_BUILD_EXAMPLES      "Build example programs for catalyst"   ${CATALYTS_STANDALONE_BUILD})

##
##  Catalyst graphicsbackend configuration
##
##  Catalyst supports several rendering backends such as OpenGL, DirectX (9, 10, 11, 12), Vulkan, Metal and WebGPU
##  (currently OpenGL backend is implemented). The backends that should be included in the build can be configured using the
##  following options
##
##option(CATALYST_BUILD_OPENGL_BACKEND        "Include the OpenGL backend for catalyst rendering library"         ON)
##option(CATALYST_BUILD_DIRECTX_9 BACKEND     "Include the DirectX 9 backend for catalyst rendering library"      OFF)
##option(CATALYST_BUILD_DIRECTX_10_BACKEND    "Include the DirectX 10 backend for catalyst rendering library"     OFF)
##option(CATALYST_BUILD_DIRECTX_11_BACKEND    "Include the DirectX 11 backend for catalyst rendering library"     OFF)
##option(CATALYST_BUILD_DIRECTX_12_BACKEND    "Include the DirectX 12 backend for catalyst rendering library"     OFF)
##option(CAATLYST_BUILD_VULKAN_BACKEND        "Include the Vulkan backend for catalyst rendering library"         OFF)
##option(CATALYST_BUILD_METAL_BACKEND         "Include the Metal backend for catalyst rendering library"          OFF)

##
##  Project source management
##
##  Inorder to keep the CMakeLists.txt file clean, the sources for all the specific library are included in the respective
##  source folders. These can be directly included while creating the build depending upon the configuration
##

include ( CMakeSources.txt )

add_library(catalyst)
add_library(Catalyst::API ALIAS catalyst)

foreach(HEADER IN LISTS CATALYST_HEADERS)
    target_sources(
            catalyst PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${HEADER}>
        )
endforeach()

foreach(SOURCE IN LISTS CATALYST_SOURCES)
    target_sources(
            catalyst PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}>
        )
endforeach()

target_include_directories(catalyst PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(catalyst PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(catalyst-editor)
add_executable(Catalyst::Editor ALIAS catalyst-editor)

target_sources(catalyst-editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/catalyst-editor/editor/main.c)
target_include_directories(catalyst-editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(catalyst-editor catalyst)
