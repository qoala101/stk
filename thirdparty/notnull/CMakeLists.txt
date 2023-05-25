include(FetchContent)

FetchContent_Declare(NotNull SYSTEM
  GIT_REPOSITORY https://github.com/bitwizeshift/not_null
  GIT_TAG c839cb1bb4cf988bd17a4c363263997275f508b8
)

FetchContent_MakeAvailable(NotNull)

add_library(thirdparty::NotNull ALIAS NotNull)

mark_as_system_target(NotNull)