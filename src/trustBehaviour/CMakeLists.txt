# Copyright (C) 2017 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
# Authors: Francesco Rea & Gonzalez Jonas
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)
SET(KEYWORD "trustBehaviour")
PROJECT(${KEYWORD})
set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(YARP REQUIRED)
find_package(ICUBcontrib REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${ICUBCONTRIB_MODULE_PATH})

#search for includes
INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/include
        ${YARP_INCLUDE_DIRS}
)

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${YARP_MODULE_PATH}
        ${ICUB_MODULE_PATH}
        ${ICUBCONTRIB_MODULE_PATH} # note 10/10/13 aligned to the standard Rea
        )


# Search for source code.
FILE(GLOB folder_source
        src/*.cpp
        src/*.cc
        src/*.c
)
FILE(GLOB folder_header
        include/iCub/*.h
        include/*.h
)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})


include(ICUBcontribHelpers)
list(APPEND CMAKE_MODULE_PATH ${ICUBCONTRIB_MODULE_PATH})
include(ICUBcontribHelpers)
include(ICUBcontribOptions)
icubcontrib_set_default_prefix() #set CMAKE_INSTALL_PREFIX to the ICUBcontrib one; print warning if user modifies it

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
add_definitions(${YARP_DEFINES})
include(YarpInstallationHelpers)

set(ICUB_APPLICATIONS_PREFIX "$ENV{ICUB_ROOT}" CACHE PATH "Application path prefix")

# Set up our main executable
IF (folder_source)
    ADD_EXECUTABLE(${KEYWORD}
            ${folder_source}
            ${folder_header}
            )

    TARGET_LINK_LIBRARIES( ${KEYWORD}
            ${YARP_LIBRARIES}
            )

    INSTALL_TARGETS(/bin ${KEYWORD})

    # default prefix for applications
ELSE (folder_source)
    MESSAGE( "No source code files found. Please add something")

ENDIF (folder_source)


