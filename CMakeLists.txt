cmake_minimum_required(VERSION 3.20)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(project_name marigold)
set(project_version "1.0.0")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (${project_name} VERSION ${project_version})

add_executable(${project_name})

find_package(DPP REQUIRED)
if (NOT DPP_FOUND)
    message(FATAL_ERROR "Failed to find dpp libraries")
else()
    target_link_libraries(${project_name} ${DPP_LIBRARIES})
    target_include_directories(${project_name} PRIVATE ${DPP_INCLUDE_DIRS})
endif()

add_subdirectory(src)
