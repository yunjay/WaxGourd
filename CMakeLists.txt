# CMakeList.txt : CMake project for WaxGourd, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("WaxGourd")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET WaxGourd PROPERTY CXX_STANDARD 20)
#endif()

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake") # Set the vcpkg toolchain file
include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

# Define source directories - this is good practice
set(APPLICATION_DIR "${PROJECT_SOURCE_DIR}/Application")
set(RENDERER_DIR "${PROJECT_SOURCE_DIR}/RenderEngine")
set(UTILITY_DIR "${PROJECT_SOURCE_DIR}/Utility")
set(SCENE_DIR "${PROJECT_SOURCE_DIR}/Scene")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Include")

# Source files - cpps include the headers
file(GLOB_RECURSE SRC_FILES
    "${APPLICATION_DIR}/*.cpp"
    "${RENDERER_DIR}/*.cpp"
    "${UTILITY_DIR}/*.cpp"
    "${SCENE_DIR}/*.cpp"
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.cpp"
)

# Add include directories
include_directories(
    "${APPLICATION_DIR}"
    "${RENDERER_DIR}"
    "${UTILITY_DIR}"
    "${SCENE_DIR}"
    "${INCLUDE_DIR}"
    "${INCLUDE_DIR}/Imgui"
)

# vcpkg packages
find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

add_executable(WaxGourd ${SRC_FILES})

target_link_libraries(WaxGourd PRIVATE glm::glm)
target_link_libraries(WaxGourd PRIVATE SDL3::SDL3)



# TODO: Add tests and install targets if needed.

