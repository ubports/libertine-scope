SET (CMAKE_INCLUDE_CURRENT_DIR ON)
SET (CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)

# Find all the sources
file(GLOB_RECURSE
  SCOPE_SOURCES
  "*.cpp"
  "*.h"
)

# Build a shared library containing our scope code.
add_library(
  scope SHARED
  ${SCOPE_SOURCES}
)

# Link against the object library and our external library dependencies
target_link_libraries(
  scope
  ${SCOPE_LDFLAGS}
  Qt5::Core
)


# Set the correct library output name to conform to the securiry policy 
set_target_properties(
  scope
  PROPERTIES
    OUTPUT_NAME "${SCOPE_NAME}"
)

# Install the scope shared library
install(
  TARGETS scope
  LIBRARY DESTINATION ${SCOPE_INSTALL_DIR}
)

