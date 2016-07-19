# Put the ini files in the build directory next to the scope
# .so file so that the test tools can find them.

# Install the scope images
install(
  FILES
    "apps.png"
  DESTINATION
    ${APPS_DATA_DIR}
)

install(
  FILES
    "blacklist"
  DESTINATION
    ${SCOPE_INSTALL_DIR}
)
