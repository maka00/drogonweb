cmake_minimum_required(VERSION 3.10)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

if(MSVC)
    add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
	# for the hardcore type of guy...
	#add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
	add_compile_options("$<$<CONFIG:RELEASE>:-O3>")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	    # add_compile_options("-stdlib=libc++")
	    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -v")
	    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
    else()
        # nothing special for gcc at the moment
    endif()
endif()

set(SOURCES
        main.cpp       
)
if (WIN32)
	SET (Boost_USE_STATIC_LIBS ON)
endif(WIN32)
find_package(Threads)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Drogon CONFIG REQUIRED)
find_package(Trantor CONFIG REQUIRED)
add_definitions(-DOM_STATIC_BUILD=1)


if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG=1)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

if(UNIX)
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        DEBUG_POSTFIX "d"
        CXX_STANDARD 17
        # CXX_EXTENSIONS OFF
        CMAKE_CXX_FLAGS_RELEASE "-O3"
        #SOVERSION 1
        #POSITION_INDEPENDENT_CODE ON
        )
endif(UNIX)
if(WIN32)
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        DEBUG_POSTFIX "d"
        CXX_STANDARD 17
        # CXX_EXTENSIONS OFF
        CMAKE_CXX_FLAGS_RELEASE "-O3"
        SOVERSION 1
        #POSITION_INDEPENDENT_CODE ON
        )
endif(WIN32)
# include_directories(${DROGON_INCLUDE_DIRS})
# link_libraries(${DROGON_LIBRARIES})
message(${DROGON_INCLUDE_DIRS})
message(${DROGON_LIBRARIES})
message(${TRANTOR_LIBRARIES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}
        PRIVATE
        ${EXTERNAL_SPDLOG_INCLUDE_DIR}
        ${EXTERNAL_STB_INCLUDE_DIR}
       	${EXTERNAL_CLARA_INCLUDE_DIR}
        ${BOOST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${DROGON_INCLUDE_DIRS}
        )


if(UNIX)
target_link_libraries(${PROJECT_NAME}
        PUBLIC
	Threads::Threads
		PRIVATE
        Boost::boost
		OpenSSL::SSL
		OpenSSL::Crypto
		${DROGON_LIBRARIES}
		${TRANTOR_LIBRARIES}
        )
endif(UNIX)
if(WIN32)
target_link_libraries(${PROJECT_NAME}
        PUBLIC
	Threads::Threads
        Boost::boost
		OpenSSL::SSL
		OpenSSL::Crypto
		)
endif(WIN32)


target_compile_features(${PROJECT_NAME}
        PUBLIC
        cxx_std_17
        )

target_compile_options(${PROJECT_NAME}
        PUBLIC
        )
if(WIN32)
	target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	_USE_MATH_DEFINES
	)
endif(WIN32)
add_subdirectory(unittest)
install(TARGETS ${PROJECT_NAME} DESTINATION ${ProjectId})