# Find GoogleTest.
# Option A: install it with `apt install libgtest-dev`.
find_package(GTest REQUIRED)

# Alternative: replace find_package with FetchContent if preferred.

# Test executable.
add_executable(unit_tests
    EmployeeTest.cpp
    # DatabaseTest.cpp
)

# Link the application library and GoogleTest.
target_link_libraries(unit_tests PRIVATE 
    ServerLib
    GTest::gtest_main
)

# Register tests with CTest.
include(GoogleTest)
gtest_discover_tests(unit_tests)