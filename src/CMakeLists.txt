include(FetchContent)

FetchContent_Declare(NotNull
  GIT_REPOSITORY https://github.com/bitwizeshift/not_null
  GIT_TAG c839cb1bb4cf988bd17a4c363263997275f508b8)

FetchContent_MakeAvailable(NotNull)

add_subdirectory(app)
add_subdirectory(aws)
add_subdirectory(cpp)
add_subdirectory(log)
add_subdirectory(network)
add_subdirectory(nosqldb)
add_subdirectory(restsdk)
add_subdirectory(spdlog)
add_subdirectory(sqldb)
add_subdirectory(sqlite)
add_subdirectory(stonks)