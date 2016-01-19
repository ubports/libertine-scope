# Build with system gmock and its embedded gtest -- the horror of hardcoding
set (GMOCK_SOURCE_DIR "/usr/src/gmock" CACHE PATH "gmock source directory")
set (GTEST_INCLUDE_DIR "${GMOCK_SOURCE_DIR}/gtest/include" CACHE PATH "gtest source include directory")
add_subdirectory(${GMOCK_SOURCE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gmock")

add_executable(libertine_scope_tests
  fake_container.cpp
  fake_libertine.cpp
  test_scope.cpp
)

target_link_libraries(libertine_scope_tests
  scope
  Qt5::Core
  gmock
  gmock_main
)

add_test(test_scope
  libertine_scope_tests
)
