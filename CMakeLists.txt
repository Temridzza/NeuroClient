cmake_minimum_required(VERSION 3.31)
project(ChatGPT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(SRC ${CMAKE_SOURCE_DIR}/src)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

find_package(Qt6 COMPONENTS Core Gui Widgets Sql REQUIRED)

include_directories(${INCLUDE})

add_subdirectory(src)

add_executable(ChatGPT main.cpp)

target_link_libraries(ChatGPT PRIVATE
        Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Sql
        CURL::libcurl
        nlohmann_json::nlohmann_json
        chatgptlib
)

target_include_directories(ChatGPT PRIVATE ${INCLUDE})

# connect test
enable_testing()
add_subdirectory(tests)
