cmake_minimum_required(VERSION 3.29.2+)
project(c-vk-triangle)

message("CMake build type: ${CMAKE_BUILD_TYPE}")

# Generate compile_commands.json only in Debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug: Exporting compile commands")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Find base dependencies (Vulkan)
find_package(Vulkan REQUIRED)

# VulkanMemoryAllocator
set(VMA_PROJ_PATH "${CMAKE_SOURCE_DIR}/thirdparty/VulkanMemoryAllocator")

# Set options for VMA building (disable all stuff not strictly necessary)
option(VMA_BUILD_DOCUMENTATION OFF)
option(VMA_BUILD_SAMPLES OFF)

add_subdirectory(${VMA_PROJ_PATH})

set(VMA_INCLUDE_DIRS ${VMA_PROJ_PATH}/include)

# GLFW
set(GLFW_PROJ_PATH "${CMAKE_SOURCE_DIR}/thirdparty/glfw")

# Set options for glfw building (build as shared library, disable all stuff not
# strictly necessary for integration)
option(BUILD_SHARED_LIBS "Build glfw as shared library" ON)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)

add_subdirectory(${GLFW_PROJ_PATH})

set(GLFW_INCLUDE_DIRS ${GLFW_PROJ_PATH}/include)

# Configure include directories for main executable
set(INCLUDE_DIRS ${INCLUDE_DIRS}
    ${VULKAN_INCLUDE_DIRS}
    ${VMA_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/thirdparty
)
message("Include directories: ${INCLUDE_DIRS}")
include_directories(${INCLUDE_DIRS})

# Source and header files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)

# Executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC
    # Config based on build mode
    "$<$<CONFIG:Debug>:BUILD_MODE_DEBUG>"
    "$<$<CONFIG:Release>:BUILD_MODE_RELEASE>"
)

# Set linked libraries
set(LIBS ${LIBS}
    vulkan
    GPUOpen::VulkanMemoryAllocator
    glfw
    m
)

# Link step (optionally with ASan)
if (USE_ASAN)
    message("Link config: Address Sanitizer")
    target_link_libraries(${PROJECT_NAME} ${LIBS} -fsanitize=address)
else ()
    message("Link config: Default")
    target_link_libraries(${PROJECT_NAME} ${LIBS})
endif()

# Copy compile_commands.json to project root for use with language servers (only in Debug mode)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Copying compile_commands.json: ${CMAKE_BINARY_DIR}/compile_commands.json -> ${CMAKE_SOURCE_DIR}/compile_commands.json")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_SOURCE_DIR}/compile_commands.json
        DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()
