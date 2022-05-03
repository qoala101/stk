find_package(SQLite3 REQUIRED)

add_library(libdb
  db.cc
  db_types.cc
  sqlite/sqlite_db.cc
)
set_target_properties(libdb PROPERTIES OUTPUT_NAME db)
target_include_directories(libdb PUBLIC ${PROJECT_SOURCE_DIR}/include/db)
target_include_directories(libdb PUBLIC ${PROJECT_SOURCE_DIR}/include/db/sqlite)
target_link_libraries(libdb PRIVATE SQLite::SQLite3)