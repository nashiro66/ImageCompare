cmake_minimum_required(VERSION 3.16)
# set app name
set(APP_NAME CompareImage)

# specify version of compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set project
project(${APP_NAME})
# set visual studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
# set configuration
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "Configurations" FORCE)
# make executable app
add_executable(${APP_NAME})
# add source files
add_subdirectory(src)
# suppress generation of ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION ON)

# For OpenCV
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    target_include_directories(
        ${APP_NAME}
        PUBLIC
        ${OpenCV_INCLUDE_DIRS}
    )
    target_link_libraries(
        ${APP_NAME}
        ${OpenCV_LIBS}
    )
endif()