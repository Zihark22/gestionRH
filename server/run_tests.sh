# 0. Configure and build.
mkdir -p build

# 1. Generate build files.
cmake -B build -S .

# 2. Build the application and tests.
cmake --build build

# 3. Run the application.
# ./build/src/ServerApp

# 4. Run all unit tests.
ctest --test-dir build --output-on-failure
# Or run directly: ./build/tests/unit_tests