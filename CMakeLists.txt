cmake_minimum_required(VERSION 3.16)

project(Lingo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lingo
    src/main.cpp
    src/achievements/AchievementsManager.cpp
    src/core/MistakeManager.cpp
    src/core/WordDatabase.cpp
    src/levels/LevelManager.cpp
    src/progress/ProgressManager.cpp
    src/quiz/QuizGame.cpp
    src/utils/AppPaths.cpp
    src/utils/Random.cpp
)

target_include_directories(lingo PRIVATE src)

if(APPLE)
    configure_file(
        macos/Lingo.command.in
        ${CMAKE_BINARY_DIR}/Lingo.command
        @ONLY
    )

    add_custom_target(
        macos_shortcut ALL
        COMMAND ${CMAKE_COMMAND} -E chmod +x ${CMAKE_BINARY_DIR}/Lingo.command
        DEPENDS lingo
    )
endif()
