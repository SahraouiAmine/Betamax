cmake_minimum_required(VERSION 3.24)

# Working name. Renaming the project = change ZETA_APP_NAME (and the Homebrew formula).
project(zeta VERSION 0.1.0 LANGUAGES CXX)
set(ZETA_APP_NAME zeta)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(ZETA_BUILD_TESTS "Build the test suite" ${PROJECT_IS_TOP_LEVEL})
option(ZETA_WERROR "Treat warnings as errors" OFF)
option(ZETA_SANITIZE "Build everything with ASan + UBSan" OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CompilerOptions)   # zeta_warnings target, sanitizer flags
include(Dependencies)      # FetchContent: FTXUI, CLI11, Catch2; system SQLite
include(ZetaModule)        # zeta_add_module()

# One library per area under src/. Sources are globbed, so adding a .cpp file
# never requires editing CMake.
zeta_add_module(core)
zeta_add_module(session zeta::core)
zeta_add_module(store   zeta::core ${ZETA_SQLITE_TARGET})
zeta_add_module(tui     zeta::core zeta::session ftxui::ftxui)

add_executable(zeta src/cli/main.cpp)
set_target_properties(zeta PROPERTIES OUTPUT_NAME ${ZETA_APP_NAME})
target_compile_definitions(zeta PRIVATE
  ZETA_APP_NAME="${ZETA_APP_NAME}"
  ZETA_VERSION="${PROJECT_VERSION}")
target_link_libraries(zeta PRIVATE
  zeta_warnings CLI11::CLI11
  zeta::core zeta::session zeta::store zeta::tui)

install(TARGETS zeta RUNTIME DESTINATION bin)

if(ZETA_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
