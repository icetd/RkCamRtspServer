cmake_minimum_required(VERSION 3.14)

project(RkCamRtspServer)

option (WITH_SSL "Enable SSL support" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
include (cmake/live555.cmake)

add_subdirectory(thirdparty/libv4l2cc)
add_subdirectory(thirdparty/libjpeg-turbo)

set(INCLUDE_DIR		"${CMAKE_SOURCE_DIR}/inc")
set(SOURCE_DIR		"${CMAKE_SOURCE_DIR}/src")
set(V4L2CC_INC		"${CMAKE_SOURCE_DIR}/thirdparty/libv4l2cc/inc")
set(JPEGTURBO_INC   "${CMAKE_SOURCE_DIR}/thirdparty/libjpeg-turbo")
set(MPP_INC			"${CMAKE_SOURCE_DIR}/thirdparty/mpp/include")
set(MPP_LIB			"${CMAKE_SOURCE_DIR}/thirdparty/mpp/lib")

link_directories(
	${MPP_LIB}
)

include_directories(
	${INCLUDE_DIR}
	${V4L2CC_INC}
	${JPEGTURBO_INC}
	${MPP_INC}
	${LIVE555_INC}
) 

file(GLOB_RECURSE SRC_FILES
	"${SOURCE_DIR}/*c*"
)

file (GLOB_RECURSE INC_FILES
    "${INCLUDE_DIR}/*.h"
)


list(APPEND EXTRA_LIBS
)

# Add the executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIVE555_FILES} ${INC_FILES} ${LIVE555_INC_FILES})

# Add the target includes for MY_PROJECT 
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIVE555_INC})
target_compile_definitions(${PROJECT_NAME} PUBLIC  ${LIVE555_CFLAGS})

#===================== LINKING LIBRARIES =======================#
target_link_libraries(${PROJECT_NAME} v4l2cc pthread rockchip_vpu rockchip_mpp turbojpeg ${EXTRA_LIBS})
if (OpenSSL_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
endif ()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/configs
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
