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

# Set thirdparty directory
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

# VulkanMemoryAllocator
set(VMA_PROJ_PATH ${THIRDPARTY_DIR}/VulkanMemoryAllocator)
set(VMA_INCLUDE_DIRS ${VMA_PROJ_PATH}/include)

set(VMA_IMPL_FILE ${CMAKE_SOURCE_DIR}/src/lib/vma_impl.cpp)

# GLFW
set(GLFW_PROJ_PATH ${THIRDPARTY_DIR}/glfw)

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
    ${THIRDPARTY_DIR}
)
message("Include directories: ${INCLUDE_DIRS}")
include_directories(${INCLUDE_DIRS})

# Source files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c)

# Add thirdparty dependency sources (ignore specific stuff like unit tests)
file(GLOB_RECURSE RAW_C_UTILS_COLLECTION_SOURCES ${THIRDPARTY_DIR}/c_utils_collection/*.c)
set(C_UTILS_COLLECTION_SOURCES)
message(STATUS "Filtered c_utils_collection sources (excluding tests):")
foreach (SOURCE ${RAW_C_UTILS_COLLECTION_SOURCES})
    if (NOT SOURCE MATCHES "tests/")
        message("* ${SOURCE}")
        list(APPEND C_UTILS_COLLECTION_SOURCES ${SOURCE})
    endif()
endforeach()

# Add the project executable
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${VMA_IMPL_FILE}
    # Build c_utils_collection as part of the executable
    ${C_UTILS_COLLECTION_SOURCES})

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC
    # Config based on build mode
    "$<$<CONFIG:Debug>:BUILD_MODE_DEBUG>"
    "$<$<CONFIG:Release>:BUILD_MODE_RELEASE>"
)

# Set linked libraries
set(LIBS ${LIBS}
    vulkan
    glfw
    m
)

# Link step (optionally with ASan)
if (USE_ASAN)
    message("Link config: Address Sanitizer")
    target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS} -fsanitize=address)
else ()
    message("Link config: Default")
    target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})
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
