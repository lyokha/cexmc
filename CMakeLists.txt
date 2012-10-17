cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
include(CMakeDependentOption)

set(name cexmc)
project(${name})
find_package(Geant4 REQUIRED gdml)
include(${Geant4_USE_FILE})


set(EXTRA_LIBRARIES )

# if CEXMC_USE_PERSISTENCY is 'yes' then run and events data can be read and
# written; requires boost::serialize headers and library
option(CEXMC_USE_PERSISTENCY
    "Build ${name} with data persistency support" ON)
if(CEXMC_USE_PERSISTENCY)
    find_package(Boost REQUIRED serialization)
    add_definitions(-DCEXMC_USE_PERSISTENCY)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    list(APPEND EXTRA_LIBRARIES ${Boost_LIBRARIES})
    message(STATUS "Library ${Boost_LIBRARIES} was added to the linkage list")
endif()

# if CEXMC_USE_CUSTOM_FILTER is 'yes' then Custom filter can be used for
# existing events data; requires boost::spirit 2.x headers. Notice: if
# CEXMC_USE_PERSISTENCY is not 'yes' then Custom Filter will not be used anyway
cmake_dependent_option(CEXMC_USE_CUSTOM_FILTER
    "Build ${name} with custom filter support" ON
    "CEXMC_USE_PERSISTENCY" OFF)
if(CEXMC_USE_CUSTOM_FILTER)
    add_definitions(-DCEXMC_USE_CUSTOM_FILTER)
endif()

# if CEXMC_DEBUG_CUSTOM_FILTER is 'yes' then AST trees will be printed out
cmake_dependent_option(CEXMC_DEBUG_CUSTOM_FILTER
    "Debug custom filter" OFF
    "CEXMC_USE_CUSTOM_FILTER" OFF)
if(CEXMC_USE_CUSTOM_FILTER)
    add_definitions(-DCEXMC_DEBUG_CF)
endif()

# if CEXMC_USE_QGSP_BIC_EMY is 'yes' then QGSP_BIC_EMY will be used as basic
# physics, otherwise - QGSP_BERT
option(CEXMC_USE_QGSP_BIC_EMY
    "Build ${name} with QGSP_BIC_EMY physics list (default is QGSP_BERT)" OFF)
if(CEXMC_USE_QGSP_BIC_EMY)
    add_definitions(-DCEXMC_USE_QGSP_BIC_EMY)
endif()

# if CEXMC_DEBUG_TP is 'yes' then additional info will be printed on track
# points data
option(CEXMC_DEBUG_TP
    "Print debug information for track points" OFF)
if(CEXMC_DEBUG_TP)
    add_definitions(-DCEXMC_DEBUG_TP)
endif()

option(CEXMC_USE_HISTOGRAMING
    "Build ${name} with histograming support (requires CERN ROOT package)" ON)
if(CEXMC_USE_HISTOGRAMING)
    find_package(ROOT)
    if(ROOT_FOUND)
        add_definitions(-DCEXMC_USE_ROOT)
        include_directories(${ROOT_INCLUDE_DIR})
        list(APPEND EXTRA_LIBRARIES ${ROOT_LIBRARIES})
        message(STATUS
            "Libraries ${ROOT_LIBRARIES} were added to the linkage list")
    else()
        message(WARNING
            "Could not find ROOT package, skip histograming support")
    endif()
endif()

cmake_dependent_option(CEXMC_USE_ROOTQT
    "Build ${name} with GUI histograming support
    (requires CERN ROOT package built with Qt support)" ON
    "CEXMC_USE_HISTOGRAMING" OFF)
if(CEXMC_USE_ROOTQT AND ROOT_FOUND)
    find_library(ROOTQT_LIBRARY GQt
        PATHS ${ROOT_LIBRARY_DIR} ${ROOT_LIBRARY_DIR}/root
        NO_DEFAULT_PATH)
    if(ROOTQT_LIBRARY)
        add_definitions(-DCEXMC_USE_ROOTQT)
        list(APPEND EXTRA_LIBRARIES -lGQt)
        message(STATUS "Library -lGQt was added to the linkage list")
    else()
        message(WARNING
            "Could not find ROOT Qt library, skip GUI histograming support")
    endif()
endif()


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR})
file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_executable(${name} EXCLUDE_FROM_ALL ${name}.cc ${sources})
target_link_libraries(${name} ${Geant4_LIBRARIES} ${EXTRA_LIBRARIES})

