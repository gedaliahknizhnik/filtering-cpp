cmake_minimum_required(VERSION 3.13)
project(filtering)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## MATPLOTLIB ####################################################################

# Find Python interpreter for Matplotlib
find_package (Python3 COMPONENTS Interpreter Development)
include_directories(include external ${Python3_INCLUDE_DIRS})

file(DOWNLOAD
  https://raw.githubusercontent.com/lava/matplotlib-cpp/master/matplotlibcpp.h
  ${CMAKE_SOURCE_DIR}/external/matplotlibcpp.h
)

## FILTERS #####################################################################

add_library(filtering INTERFACE) # Define the library target

target_include_directories(filtering INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/filteringConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT
)

install(TARGETS filtering
    EXPORT filteringTargets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/filteringConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/filteringConfig.cmake"
    INSTALL_DESTINATION lib/cmake/filtering
)

install(EXPORT filteringTargets DESTINATION lib/cmake/filtering)
install(FILES "${PROJECT_BINARY_DIR}/filteringConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/filteringConfig.cmake"
        DESTINATION lib/cmake/filtering)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION include)

## EXAMPLES ####################################################################

add_executable(example src/example_filtering.cpp)
target_link_libraries(example filtering)

add_executable(example_plot src/example_plotting.cpp)
target_link_libraries(example_plot filtering Python3::Python)

add_executable(example_multi src/example_multi.cpp)
target_link_libraries(example_multi filtering Python3::Python)

add_executable(example_highlowpass src/example_high_low_pass.cpp)
target_link_libraries(example_highlowpass filtering Python3::Python)
