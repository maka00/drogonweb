cmake_minimum_required (VERSION 3.9)
project(websocket-project)

if(WIN32)
	set(CPACK_GENERATOR "ZIP")
else(WIN32)
	set(CPACK_GENERATOR "TGZ")
endif(WIN32)
set (${PROJECT_NAME}_MAJOR_VERSION 1)
set (${PROJECT_NAME}_MINOR_VERSION 1)
set (${PROJECT_NAME}_PATCH_VERSION 1)
SET(CPACK_PACKAGE_VERSION_MAJOR 1)
SET(CPACK_PACKAGE_VERSION_MINOR 1)
SET(CPACK_PACKAGE_VERSION_PATCH 1)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include(CPack)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
endforeach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# if we need a logger
add_subdirectory(${PROJECT_SOURCE_DIR}/external)
add_subdirectory(${PROJECT_SOURCE_DIR}/server)
