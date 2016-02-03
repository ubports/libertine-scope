SET (CMAKE_AUTOMOC ON)

pkg_check_modules(URL_DISPATCHER REQUIRED url-dispatcher-1)

# Find all the sources
file(GLOB_RECURSE
  SCOPE_SOURCES
  "*.cpp"
  "*.h"
)

add_library(scope SHARED
  ${SCOPE_SOURCES}
)

include_directories(${URL_DISPATCHER_INCLUDE_DIRS})

target_link_libraries(scope
  ${SCOPE_LDFLAGS}
  Qt5::Core
  Qt5::Gui
  ${URL_DISPATCHER_LIBRARIES}
)


set_target_properties(scope
  PROPERTIES
    OUTPUT_NAME "${SCOPE_NAME}"
)

install(TARGETS scope
  LIBRARY DESTINATION ${SCOPE_INSTALL_DIR}
)

