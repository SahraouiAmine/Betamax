include(FetchContent)

# Release tarballs pinned by hash: reproducible, and faster than git clones.

set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(FTXUI_QUIET ON CACHE BOOL "" FORCE)
FetchContent_Declare(ftxui
  URL https://github.com/ArthurSonzogni/FTXUI/archive/refs/tags/v7.0.3.tar.gz
  URL_HASH SHA256=e7c62ffe19009759821b4f0f8df7f2a6fb83784c3a9f1477d81f56d3ee723c88)

FetchContent_Declare(cli11
  URL https://github.com/CLIUtils/CLI11/archive/refs/tags/v2.7.2.tar.gz
  URL_HASH SHA256=46eef3101da70852ec7af026e09d485ccee81813331c8c6052d39344443b83da)

FetchContent_MakeAvailable(ftxui cli11)

if(ZETA_BUILD_TESTS)
  FetchContent_Declare(catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.16.0.tar.gz
    URL_HASH SHA256=0957cae5821b17ce07f0833aaa52b5137643a8382203221f363a8303c109af34)
  FetchContent_MakeAvailable(catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()

# SQLite comes from the system (macOS SDK, libsqlite3-dev on Linux).
# The imported target was renamed in CMake 4.1; accept either name.
find_package(SQLite3 REQUIRED)
if(TARGET SQLite3::SQLite3)
  set(ZETA_SQLITE_TARGET SQLite3::SQLite3)
else()
  set(ZETA_SQLITE_TARGET SQLite::SQLite3)
endif()
