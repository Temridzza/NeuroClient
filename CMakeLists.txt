# CMakeLists.txt
cmake_minimum_required(VERSION 3.31)

project(ChatGPT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)

# ==========================================
# Qt
# ==========================================
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Quick
        Qml
        Sql
)

qt_standard_project_setup()
#set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# ==========================================
# CURL
# ==========================================
include(FetchContent)

find_package(CURL QUIET)

if (CURL_FOUND)

    message(STATUS "Using system CURL")
    set(CURL_TARGET CURL::libcurl)

else()

    message(STATUS "System CURL not found, fetching...")

    FetchContent_Declare(
            curl
            GIT_REPOSITORY https://github.com/curl/curl.git
            GIT_TAG curl-8_6_0
    )

    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
    set(CURL_DISABLE_LDAP ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(curl)

    set(CURL_TARGET libcurl)

endif()

# ==========================================
# Includes
# ==========================================
include_directories(${INCLUDE})

# ==========================================
# Library
# ==========================================
add_subdirectory(src)

# ==========================================
# Main executable
# ==========================================
qt_add_executable(chatgpt_app
        main.cpp
)

# ==========================================
# QML
# ==========================================
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)
qt_add_qml_module(chatgpt_app
        URI ChatGPT
        VERSION 1.0

        QML_FILES
        qml/Main.qml
        qml/components/ChatsSideBar.qml
        qml/components/ContextSideBar.qml
)

# ==========================================
# Linking
# ==========================================
target_link_libraries(chatgpt_app PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        Qt6::Sql

        ${CURL_TARGET}

        chatgptlib
)

target_include_directories(chatgpt_app PRIVATE
        ${INCLUDE}
)

# ==========================================
# Tests
# ==========================================
find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(tests)

