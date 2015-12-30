project(libertine-scope CXX)
cmake_minimum_required(VERSION 2.8.10)

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
find_package(PkgConfig)
find_package(Intltool)

pkg_check_modules(SCOPE libunity-scopes>=0.6.0 REQUIRED)

# Add our dependencies to the include paths
include_directories(
  "${CMAKE_SOURCE_DIR}/src"
  ${SCOPE_INCLUDE_DIRS}
)

# Important project paths
set(SCOPE_INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/unity-scopes/libertine-scope/)
set(SCOPE_NAME "libertine-scope")
set(GETTEXT_PACKAGE "${SCOPE_NAME}")
set(PACKAGE_NAME "libertine-scope.canonical")

# If we need to refer to the scope's name or package in code, these definitions will help
add_definitions(-DPACKAGE_NAME="${PACKAGE_NAME}")
add_definitions(-DSCOPE_NAME="${SCOPE_NAME}")
add_definitions(-DGETTEXT_PACKAGE="${GETTEXT_PACKAGE}")

# This command figures out the target architecture and puts it into the manifest file
execute_process(COMMAND dpkg-architecture -qDEB_HOST_ARCH
                OUTPUT_VARIABLE CLICK_ARCH
                OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Configure and install the click manifest and apparmor files
configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json
        DESTINATION ${SCOPE_INSTALL_DIR})
install(FILES "libertine-scope.apparmor"
        DESTINATION ${SCOPE_INSTALL_DIR})

# Add our main directories
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(po)

# Set up the tests
enable_testing()
add_subdirectory(tests)
add_custom_target(check
                 ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure
)

