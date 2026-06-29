cmake_minimum_required(VERSION 3.10)
project(BackupEngine C)

# ---- C11 Standard ----
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ---- Platform-specific settings ----
if(APPLE)
    set(PLATFORM_DEFINES -DPLATFORM_MACOS)
    set(PLATFORM_LIBS "")
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_DEFINES -DPLATFORM_LINUX)
    set(PLATFORM_LIBS m)
elseif(WIN32)
    set(PLATFORM_DEFINES -DPLATFORM_WINDOWS)
    set(PLATFORM_LIBS "")
endif()

# ---- Warnings ----
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ---- Sources ----
set(SQLITE_SRC   "${CMAKE_CURRENT_SOURCE_DIR}/sqlite/sqlite3.c")
set(BACKUP_SRC   "${CMAKE_CURRENT_SOURCE_DIR}/backup_db.c")
set(MAIN_SRC     "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

# ---- Executable ----
add_executable(backup_engine ${SQLITE_SRC} ${BACKUP_SRC} ${MAIN_SRC})

target_compile_definitions(backup_engine PRIVATE ${PLATFORM_DEFINES})
target_include_directories(backup_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)
target_link_libraries(backup_engine PRIVATE ${PLATFORM_LIBS})
