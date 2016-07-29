# Build with system gmock and its embedded gtest -- the horror of hardcoding
set (GMOCK_SOURCE_DIR "/usr/src/gmock" CACHE PATH "gmock source directory")
set (GTEST_INCLUDE_DIR "${GMOCK_SOURCE_DIR}/gtest/include" CACHE PATH "gtest source include directory")
add_subdirectory(${GMOCK_SOURCE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gmock")

function(create_test test_name)
  add_executable(${test_name}_exe
    fake_container.cpp
    fake_libertine.cpp
    mock_hidden_apps.h
    ${test_name}.cpp
  )

  target_link_libraries(${test_name}_exe
    apps
    Qt5::Core
    gmock
    gmock_main
  )

  add_test(${test_name} ${test_name}_exe)
endfunction(create_test)

create_test(test_scope)
create_test(test_preview)
create_test(test_query)
create_test(test_hidden_apps)
create_test(test_blacklist)
create_test(test_action)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
