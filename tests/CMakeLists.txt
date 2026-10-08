# Catch2 tests: every .cpp under tests/ goes into one binary. The target only
# exists once there is at least one test file.
file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

if(test_sources)
  include(Catch)
  add_executable(zeta_tests ${test_sources})
  target_link_libraries(zeta_tests PRIVATE
    zeta_warnings Catch2::Catch2WithMain
    zeta::core zeta::session zeta::store)
  catch_discover_tests(zeta_tests)
endif()

# Smoke test: the binary starts and reports its version.
add_test(NAME cli.version COMMAND zeta --version)
set_tests_properties(cli.version PROPERTIES
  PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")
