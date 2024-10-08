cmake_minimum_required(VERSION 3.5)
project(PortalEditor)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Source files
file(GLOB_RECURSE APP_SOURCES "src/*.cpp")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/../Dimensional-Core/src)
include_directories(${PROJECT_SOURCE_DIR}/../Dimensional-Core/lib/GLM)
include_directories(${PROJECT_SOURCE_DIR}/../Dimensional-Core/lib/ENTT/single_include)
include_directories(${PROJECT_SOURCE_DIR}/../Dimensional-Core/lib/YAML/include)
include_directories(src)

# Executable
add_executable(Portal ${APP_SOURCES})
target_link_libraries(Portal Dimensional-Core)

if(WIN32)
    # Copying the Assets directory on Windows
    add_custom_command(TARGET Portal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Assets" ${CMAKE_BINARY_DIR}/Assets
        COMMENT "Copying ${PROJECT_SOURCE_DIR}/Assets to ${CMAKE_BINARY_DIR}/Assets"
    )
    # Copying imgui.ini on Windows
    add_custom_command(TARGET Portal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/imgui.ini" ${CMAKE_BINARY_DIR}/imgui.ini
        COMMENT "Copying ${PROJECT_SOURCE_DIR}/imgui.ini to ${CMAKE_BINARY_DIR}/imgui.ini"
    )
else()
    # Creating a symlink for the Assets directory on other platforms
    add_custom_command(TARGET Portal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_SOURCE_DIR}/Assets" ${CMAKE_BINARY_DIR}/Assets
        COMMENT "Creating symlink from ${PROJECT_SOURCE_DIR}/Assets to ${CMAKE_BINARY_DIR}/Assets"
    )
    # Creating another symlink for the Assets directory
    add_custom_command(TARGET Portal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_SOURCE_DIR}/Assets" ${PROJECT_SOURCE_DIR}/../Assets
    )
    add_custom_command(TARGET Portal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_SOURCE_DIR}/imgui.ini" ${PROJECT_SOURCE_DIR}/../imgui.ini
    )

    # Creating a symlink for imgui.ini on other platforms
    add_custom_command(TARGET Portal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_SOURCE_DIR}/imgui.ini" ${CMAKE_BINARY_DIR}/imgui.ini
        COMMENT "Creating symlink from ${PROJECT_SOURCE_DIR}/imgui.ini to ${CMAKE_BINARY_DIR}/imgui.ini"
    )
endif()
