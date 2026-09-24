# 1. Minimum version and project name.
cmake_minimum_required(VERSION 3.16)
project(ServerApp VERSION 1.0.0 LANGUAGES CXX)

# 2. C++ standard (C++17).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 3. Strict warnings.
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 4. Find external dependencies.
find_package(SQLite3 REQUIRED)

# Include the project headers and SQLite headers.
include_directories(includes ${SQLite3_INCLUDE_DIRS})

# 5. Application source directory.
add_subdirectory(src)

# 6. Unit test configuration.
include(CTest) # Declare BUILD_TESTING, enabled by default.
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()