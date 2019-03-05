
cmake_minimum_required (VERSION 3.10)
set(name "XiCameraSync")
project ( ${name} )

# c++ version
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# opencv 
if ( WIN32 )
    set(OpenCV_DIR "E:/libs/OpenCV/x64/vc14/lib")
    set(OpenCV_CONFIG_PATH "E:/libs/OpenCV")
    set(WIN_HEADER_PATH "${PROJECT_SOURCE_DIR}/winheadfile")
    include_directories(${WIN_HEADER_PATH})
endif()
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# XIMEA camera
if ( WIN32 )
    set(XIMEA_SDK_INCLUDE_DIR "C:/XIMEA/API")
    set(XIMEA_SDK_LIBRARY_DEBUG debug "C:/XIMEA/API/x64/xiapi64.lib")
    set(XIMEA_SDK_LIBRARY_RELEASE optimized "C:/XIMEA/API/x64/xiapi64.lib")
else ()
    set(XIMEA_SDK_INCLUDE_DIR "/opt/XIMEA/include")
    set(XIMEA_SDK_LIBRARY_DEBUG debug "/usr/lib/libm3api.so")
    set(XIMEA_SDK_LIBRARY_RELEASE optimized "/usr/lib/libm3api.so")
endif()
include_directories(${XIMEA_SDK_INCLUDE_DIR})


# NvEncoder
set(MY_HEADER_FILES
    ${MY_HEADER_FILES}
    SKCommon.hpp
)
set(MY_SOURCE_FILES 
    ${MY_SOURCE_FILES}
    main.cpp
	)

include_directories(
	${PROJECT_SOURCE_DIR}
)

add_executable(${name}
    ${MY_HEADER_FILES}
    ${MY_SOURCE_FILES}
)

target_link_libraries(${name}
	${OpenCV_LIBS}
    debug  ${XIMEA_SDK_LIBRARY_DEBUG}
    optimized  ${XIMEA_SDK_LIBRARY_RELEASE}
)
