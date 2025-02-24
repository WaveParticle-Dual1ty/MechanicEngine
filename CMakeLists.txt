cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(MechanicEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJ_SRC_DIR ${PROJ_DIR}/Source)
set(PROJ_THIRDPARTY_DIR ${PROJ_DIR}/Source/ThirdParty)
set(PROJ_RESOURCE_DIR ${PROJ_DIR}/Resources)
set(PROJ_DEPENDS_DIR ${PROJ_DIR}/Depends)
set(PROJ_OUTPUT_DIR ${PROJ_DIR}/Out)
set(PROJ_OUTPUT_DIR_DEBUG ${PROJ_OUTPUT_DIR}/Debug)
set(PROJ_OUTPUT_DIR_RELEASE ${PROJ_OUTPUT_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJ_OUTPUT_DIR_DEBUG})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJ_OUTPUT_DIR_DEBUG})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJ_OUTPUT_DIR_DEBUG})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJ_OUTPUT_DIR_RELEASE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJ_OUTPUT_DIR_RELEASE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJ_OUTPUT_DIR_RELEASE})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(${PROJ_SRC_DIR})

add_subdirectory(Source)
add_subdirectory(Resources)

