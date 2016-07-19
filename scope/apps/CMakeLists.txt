set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

pkg_check_modules(URL_DISPATCHER REQUIRED url-dispatcher-1)

# Find all the sources
file(GLOB_RECURSE SCOPE_SOURCES
  "*.cpp"
  "*.h"
)

add_library(apps SHARED
  ${SCOPE_SOURCES}
)

include_directories(${URL_DISPATCHER_INCLUDE_DIRS})

target_link_libraries(apps
  ${SCOPE_LDFLAGS}
  ${UAL_LDFLAGS}
  ${LIBERTINE_LDFLAGS}
  Qt5::Core
  ${URL_DISPATCHER_LIBRARIES}
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h"
)

set_target_properties(apps
  PROPERTIES
    OUTPUT_NAME ${PACKAGE_NAME}
)

install(TARGETS apps
  LIBRARY DESTINATION "${APPS_LIB_DIR}"
)

set(APPS_INI_TARGET ${PACKAGE_NAME}.ini)
configure_file(
  ${APPS_INI_TARGET}.in.in
  ${APPS_INI_TARGET}.in
)

add_custom_target(${APPS_INI_TARGET} ALL
  COMMENT "Merging translations into ${APPS_INI_TARGET}"
  COMMAND LC_ALL=C ${INTLTOOL_MERGE} -d -u ${CMAKE_SOURCE_DIR}/po ${APPS_INI_TARGET}.in ${APPS_INI_TARGET} >/dev/null
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${APPS_INI_TARGET}"
  DESTINATION "${APPS_LIB_DIR}"
)
