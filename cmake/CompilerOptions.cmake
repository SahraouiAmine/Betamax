# Warnings apply only to our own targets (linked privately), never to dependencies.
add_library(betamax_warnings INTERFACE)
target_compile_options(betamax_warnings INTERFACE -Wall -Wextra -Wpedantic)
if(BETAMAX_WERROR)
  target_compile_options(betamax_warnings INTERFACE -Werror)
endif()

# Sanitizers apply globally (set before dependencies are fetched) so that
# third-party code is instrumented too; mixing instrumented and
# uninstrumented code can produce false positives.
if(BETAMAX_SANITIZE)
  add_compile_options(-fsanitize=address,undefined
                      -fno-sanitize-recover=undefined
                      -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

# Apple's linker warns when a static library appears twice on the link line,
# which is normal with transitive module dependencies (core via session, etc.).
if(APPLE)
  add_link_options(LINKER:-no_warn_duplicate_libraries)
endif()
