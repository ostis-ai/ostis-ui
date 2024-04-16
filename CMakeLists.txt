cmake_minimum_required(VERSION 3.24.0)
cmake_policy(SET CMP0048 NEW)
project(ostis-ui VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
 googletest
 GIT_REPOSITORY https://github.com/google/googletest.git
 GIT_TAG        v1.14.0 # Use the latest release or a specific version you need
 OVERRIDE_FIND_PACKAGE
)

# {{{ Include sc-machine
# TODO: dynamic configuration for sc-machine path based on the ostis-web-platform's variable
set(SC_MACHINE_PATH "${CMAKE_CURRENT_LIST_DIR}/sc-machine")
include("${SC_MACHINE_PATH}/CMakeLists.txt")
# }}} Include sc-machine

# configure tests {{{
option(OSTIS_UI_BUILD_TESTS "Build ostis-ui tests" ON)
if(OSTIS_UI_BUILD_TESTS)
    find_package(GoogleTest CONFIG REQUIRED)
    include(GoogleTest)
    enable_testing()
endif()
# }}} configure tests

set(TEMPLATE_AGENT_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}/template-agent")

add_subdirectory(template-agent)
add_subdirectory(action-interpreter)
add_subdirectory(model-generators)
add_subdirectory(translators)

