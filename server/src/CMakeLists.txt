# 1. List all business-logic source files, excluding main.cpp.
# List them explicitly to avoid stale CMake cache issues.
set(SERVER_CORE_SOURCES
    apiserver.cpp
    dbhandler.cpp
    employees.cpp
    iniparser.cpp
    server.cpp
)

# 2. Create the library containing the business logic.
add_library(ServerLib STATIC ${SERVER_CORE_SOURCES})

# 3. Attach headers and dependencies to the library.
target_include_directories(ServerLib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../includes
    ${SQLite3_INCLUDE_DIRS}
)
target_link_libraries(ServerLib PUBLIC ${SQLite3_LIBRARIES})

# 4. Create the final application binary.
add_executable(ServerApp main.cpp)
target_link_libraries(ServerApp PRIVATE ServerLib)

# 5. Copy the configuration file after building.
add_custom_command(
    TARGET ServerApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/config/config.ini"
            "$<TARGET_FILE_DIR:ServerApp>/config/config.ini"
            COMMENT "Copy config.ini to the output directory"
)

# 6. Copy the database after building.
add_custom_command(
    TARGET ServerApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/data/employees.db"
            "$<TARGET_FILE_DIR:ServerApp>/data/employees.db"
            COMMENT "Copy the database to the output directory"
)