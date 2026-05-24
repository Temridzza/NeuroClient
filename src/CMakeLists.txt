find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

set(SRC_FILES
        ChatGPTClient.cpp
        MainWindow.cpp
        ChatGPTConfig.cpp
        SettingsController.cpp
        SettingsWindow.cpp
        database/ChatRepository.cpp
        database/DatabaseManager.cpp
        database/MessageRepository.cpp
        MessageWidget.cpp
        SidePanel.cpp
)

set(INCLUDE
        ../include/ChatGPTClient.h
        ../include/MainWindow.h
        "../include/ChatGPTConfig.h"
        ../include/SettingsController.h
        ../include/SettingsWindow.h
        ../include/database/ChatRepository.h
        ../include/database/DatabaseManager.h
        ../include/database/MessageRepository.h
        ../include/MessageWidget.h
        ../include/SidePanel.h
)

add_library(chatgptlib STATIC ${SRC_FILES} ${INCLUDE} MainWindow.ui)

target_include_directories(chatgptlib PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/database)

target_link_libraries(chatgptlib PUBLIC
        Qt6::Core Qt6::Gui Qt6::Widgets
        Qt6::Sql
        CURL::libcurl
        nlohmann_json::nlohmann_json
)