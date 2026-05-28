cmake_minimum_required(VERSION 3.31...3.31)
cmake_policy(VERSION 3.31)

project(ChatGPT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# ==========================================
# Qt
# ==========================================
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Qml
    Sql
    Concurrent
)

qt_standard_project_setup()

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
# cmark-gfm
# ==========================================
find_package(PkgConfig REQUIRED)
pkg_check_modules(CMARK_GFM REQUIRED libcmark-gfm)

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

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_qml_module(chatgpt_app
    URI ChatGPT
    VERSION 1.0

    QML_FILES
        qml/Main.qml
        qml/components/ChatsSideBar.qml
        qml/components/ContextSideBar.qml
        qml/theme/qmldir

    SOURCES
        include/ui/qml/QmlModule.h
        QML_FILES qml/pages/SettingsPage.qml
        QML_FILES qml/components/ChatInput.qml
        QML_FILES qml/theme/Theme.qml
        QML_FILES qml/pages/ChatPage.qml
)

# ==========================================
# Includes
# ==========================================
target_include_directories(chatgpt_app PRIVATE
    ${INCLUDE_DIR}
    ${CMARK_GFM_INCLUDE_DIRS}
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
    Qt6::Concurrent

    ${CURL_TARGET}
    ${CMARK_GFM_LIBRARIES}
    chatgptlib
)

target_compile_options(chatgpt_app PRIVATE
    ${CMARK_GFM_CFLAGS_OTHER}
)

# ==========================================
# Tests
# ==========================================
find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(tests)