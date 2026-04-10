add_executable(server
    main.cpp
    database.cpp
    inventory.cpp
    server.cpp
    sqlite3.c
)

target_include_directories(server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)