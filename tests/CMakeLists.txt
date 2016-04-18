# Build with system gmock and its embedded gtest -- the horror of hardcoding
set (GMOCK_SOURCE_DIR "/usr/src/gmock" CACHE PATH "gmock source directory")
set (GTEST_INCLUDE_DIR "${GMOCK_SOURCE_DIR}/gtest/include" CACHE PATH "gtest source include directory")
add_subdirectory(${GMOCK_SOURCE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gmock")

add_executable(libertine_scope_tests
  fake_container.cpp
  fake_libertine.cpp

  # tests
  test_scope.cpp
  test_app_description.cpp
  test_preview.cpp
  test_query.cpp
)

target_link_libraries(libertine_scope_tests
  scope
  Qt5::Core
  gmock
  gmock_main
)

add_test(test_scope libertine_scope_tests)
add_test(test_app_description libertine_scope_tests)
add_test(test_preview libertine_scope_tests)
add_test(test_query libertine_scope_tests)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/tests/data/ ${CMAKE_CURRENT_BINARY_DIR}/data)
