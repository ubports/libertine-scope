cmake_minimum_required(VERSION 3.0.2)
cmake_policy(SET CMP0048 NEW)

project(libertine-scope
        VERSION 1.3.2
        LANGUAGES CXX)

# We require at least g++ 4.9, to avoid ABI breakage with earlier versions.
set(cxx_version_required 4.9)
if (CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${cxx_version_required})
        message(FATAL_ERROR "g++ version must be at least ${cxx_version_required}!")
    endif()
endif()

# Set strict and naggy C++ compiler flags, and enable C++11
add_definitions(
  -fno-permissive
  -std=c++11
  -pedantic
  -Wall
  -Wextra
  -fPIC
  -DQT_NO_KEYWORDS
)

# Search for our dependencies
include(GNUInstallDirs)
find_package(PkgConfig REQUIRED)
find_package(Intltool)
find_package(Qt5Core REQUIRED)
find_program(INTLTOOL_MERGE intltool-merge)

# useful variables
set(APPS_LIB_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/unity-scopes/libertine-scope/)
set(APPS_DATA_DIR ${CMAKE_INSTALL_FULL_DATADIR}/unity/scopes/libertine-scope/)
set(SCOPE_NAME "libertine-scope")
set(SCOPE_DISPLAY_NAME "Desktop Apps")
set(PACKAGE_NAME "libertine-scope.ubuntu")
set(FULLY_QUALIFIED_NAME "${PACKAGE_NAME}_${SCOPE_NAME}")
option(CLICK_MODE "Installs to a contained location" off)

add_definitions(-DSCOPE_NAME="${SCOPE_NAME}")
add_definitions(-DGETTEXT_PACKAGE="${SCOPE_NAME}")

pkg_check_modules(SCOPE libunity-scopes>=1.0.0 REQUIRED)
pkg_check_modules(UAL ubuntu-app-launch-2>=0.9.0 REQUIRED)
pkg_check_modules(LIBERTINE libertine>=0.99 REQUIRED)

# Add our dependencies to the include paths
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${SCOPE_INCLUDE_DIRS}
  SYSTEM ${UAL_INCLUDE_DIRS}
  ${LIBERTINE_INCLUDE_DIRS}
)

set(APP ${PROJECT})

# Important project paths
if (CLICK_MODE)
  set(SCOPE_INSTALL_DIR "/libertine-scope")
else()
  set(SCOPE_INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/unity-scopes/libertine-scope/)
endif(CLICK_MODE)

set(SCOPE_NAME "libertine-scope")
set(GETTEXT_PACKAGE "${SCOPE_NAME}")

# This command figures out the target architecture and puts it into the manifest file
execute_process(COMMAND dpkg-architecture -qDEB_HOST_ARCH
                OUTPUT_VARIABLE CLICK_ARCH
                OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Configure and install the click manifest and apparmor files
configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
if (CLICK_MODE)
  install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/manifest.json
        "libertine-scope.apparmor"
        DESTINATION "/")
else()
  install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/manifest.json
        "libertine-scope.apparmor"
        DESTINATION ${SCOPE_INSTALL_DIR})
endif()

# Add our main directories
add_subdirectory(scope)
add_subdirectory(data)
add_subdirectory(po)

# Set up the tests
enable_testing()
add_subdirectory(tests)
add_custom_target(check ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure)

set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
add_custom_target(dist
  COMMAND bzr export --root=${ARCHIVE_NAME} ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
