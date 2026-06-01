# --------------------------------------------------------------
#   CMakeLists.txt  –  QtQuick / Quickshell плагин
# --------------------------------------------------------------
cmake_minimum_required(VERSION 3.31...3.31)
cmake_policy(VERSION 3.31)

project(ChatGPT LANGUAGES CXX)

# ------------------- Общие настройки -------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# При линковке будем искать библиотеки в тех же каталогах,
# где находятся их .so/.dll.
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Путь к заголовочным файлам проекта
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# ------------------- Qt ------------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Qml
    Sql
    Concurrent
)

# Стандартный набор переменных (CMAKE_RUNTIME_OUTPUT_DIRECTORY и т.п.)
qt_standard_project_setup()

# ------------------- CURL ----------------------------
include(FetchContent)

find_package(CURL QUIET)

if (CURL_FOUND)
    message(STATUS "Using system CURL")
    set(CURL_TARGET CURL::libcurl)
else()
    message(STATUS "System CURL not found – fetching from source")
    FetchContent_Declare(
        curl
        GIT_REPOSITORY https://github.com/curl/curl.git
        GIT_TAG curl-8_6_0        # актуальная ветка на момент написания
    )
    # Отключаем всё, что нам не нужно в сборке curl
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
    set(CURL_DISABLE_LDAP ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(curl)
    set(CURL_TARGET libcurl)   # цель, созданная FetchContent
endif()

# ------------------- cmark‑gfm -----------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(CMARK_GFM REQUIRED libcmark-gfm)

# ------------------- Собственная библиотека ----------
# src/ → содержит исходники и заголовки, которые формируют
# статическую/общую библиотеку chatgptlib (оставляем без изменений)
add_subdirectory(src)

# ------------------- QML‑модуль‑плагин -------------
# Цель будет называется ChatGPT_QML (можно переименовать,
# но имя должно совпадать с тем, что мы импортируем в QML)
qt_add_qml_module(ChatGPT_QML
    URI ChatGPT                # имя, которое будем писать в import
    VERSION 1.0
    PLUGIN_TARGET ChatGPT_QML            # гарантирует генерацию shared‑library

    # ---------- QML‑файлы ----------
    QML_FILES
        qml/Main.qml
        qml/theme/Theme.qml

        qml/settings/SettingsPage.qml
        qml/settings/pages/ProviderPage.qml
        qml/settings/pages/ProviderEditPage.qml

        qml/pages/chat/ChatInput.qml
        qml/pages/chat/ChatPage.qml
        qml/pages/chat/ContextSideBar.qml

        qml/pages/chat/chatsSideBar/ChatsSideBar.qml
        qml/pages/chat/chatsSideBar/SidebarHeader.qml
        qml/pages/chat/chatsSideBar/SidebarSearch.qml
        qml/pages/chat/chatsSideBar/SidebarChatList.qml
        qml/pages/chat/chatsSideBar/SidebarChatDelegate.qml
        qml/pages/chat/chatsSideBar/SidebarNewChatButton.qml
        qml/pages/chat/ChatHeader.qml
        qml/pages/chat/ChatList.qml
        qml/pages/chat/ChatStatusBar.qml
        qml/pages/chat/EmptyChatView.qml

    # ---------- C++‑исходники, которые нужны плагину ----------
    SOURCES
        src/SingletonRegistration.cpp
)

# ------------------- Include‑директории ----------
target_include_directories(ChatGPT_QML PRIVATE
    ${INCLUDE_DIR}
    ${CMARK_GFM_INCLUDE_DIRS}
)

# ------------------- Линковка --------------------
target_link_libraries(ChatGPT_QML PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Sql
    Qt6::Concurrent

    ${CURL_TARGET}
    ${CMARK_GFM_LIBRARIES}
    chatgptlib               # ваша бизнес‑логика (из src/)
)

# Флаги компиляции, которые могут понадобиться cmark‑gfm
target_compile_options(ChatGPT_QML PRIVATE
    ${CMARK_GFM_CFLAGS_OTHER}
)

# ------------------- Политики Qt (если нужны) ----------
qt_policy(SET QTP0001 NEW)   # включаем новые возможности
qt_policy(SET QTP0004 NEW)

# ------------------- Тесты -----------------------
find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(tests)

# ------------------- Установка (опционально) ----------
# Плагин будет установлен в <prefix>/lib/qtqml/ChatGPT/
install(TARGETS ChatGPT_QML
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/qtqml/ChatGPT
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin   # для Windows‑DLL
)

# ------------------- Сообщения  -------------------
message(STATUS "=== ChatGPT QML‑module ready ===")
message(STATUS "  Import URI : ChatGPT 1.0")
message(STATUS "  Plugin lib : ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}ChatGPT_QML${CMAKE_SHARED_LIBRARY_SUFFIX}")