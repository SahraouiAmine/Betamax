# zeta_add_module(<name> [deps...])
#
# Defines zeta::<name> from every .cpp under src/<name>/. Headers are included
# relative to src/, e.g. #include "core/rational.hpp".
#
# While a module has no .cpp files yet (empty, or header-only) it is an
# INTERFACE library; it becomes a STATIC library as soon as one appears.
# CONFIGURE_DEPENDS makes the build re-run the glob, so new files are picked
# up by a plain `cmake --build`.
function(zeta_add_module name)
  file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/${name}/*.cpp")
  set(target zeta_${name})

  if(sources)
    add_library(${target} STATIC ${sources})
    target_link_libraries(${target} PRIVATE zeta_warnings)
    set(scope PUBLIC)
  else()
    add_library(${target} INTERFACE)
    set(scope INTERFACE)
  endif()

  target_include_directories(${target} ${scope} ${PROJECT_SOURCE_DIR}/src)
  target_compile_features(${target} ${scope} cxx_std_20)
  target_link_libraries(${target} ${scope} ${ARGN})
  add_library(zeta::${name} ALIAS ${target})
endfunction()
