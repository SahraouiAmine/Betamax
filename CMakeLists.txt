cmake_minimum_required(VERSION 3.24)

project(betamax VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(BETAMAX_BUILD_TESTS "Build the test suite" ${PROJECT_IS_TOP_LEVEL})
option(BETAMAX_WERROR "Treat warnings as errors" OFF)
option(BETAMAX_SANITIZE "Build everything with ASan + UBSan" OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CompilerOptions)   # betamax_warnings target, sanitizer flags
include(Dependencies)      # FetchContent: FTXUI, CLI11, Catch2; system SQLite
include(BetamaxModule)     # betamax_add_module()

# One library per area under src/. Sources are globbed, so adding a .cpp file
# never requires editing CMake.
betamax_add_module(core)
betamax_add_module(session betamax::core)
betamax_add_module(store   betamax::core ${BETAMAX_SQLITE_TARGET})
betamax_add_module(tui     betamax::core betamax::session ftxui::ftxui)

add_executable(betamax src/cli/main.cpp)
target_compile_definitions(betamax PRIVATE
  BETAMAX_APP_NAME="${PROJECT_NAME}"
  BETAMAX_VERSION="${PROJECT_VERSION}")
target_link_libraries(betamax PRIVATE
  betamax_warnings CLI11::CLI11
  betamax::core betamax::session betamax::store betamax::tui)

install(TARGETS betamax RUNTIME DESTINATION bin)

if(BETAMAX_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
