
# Put the ini files in the build directory next to the scope
# .so file so that the test tools can find them.
intltool_merge_translations(
  "libertine-scope.ini.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_${SCOPE_NAME}.ini"
  ALL
  UTF8
)
intltool_merge_translations(
  "libertine-scope-settings.ini.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_${SCOPE_NAME}-settings.ini"
  ALL
  UTF8
)

# Install the scope ini files
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_${SCOPE_NAME}.ini"
    "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_${SCOPE_NAME}-settings.ini"
  DESTINATION
    ${SCOPE_INSTALL_DIR}
)

# Install the scope images
install(
  FILES
    "icon.png"
  DESTINATION
    ${SCOPE_INSTALL_DIR}
)


