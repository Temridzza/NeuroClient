# src/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Sql Qml)
set(SRC_FILES
        network/LLMClient.cpp
        config/AppSettings.cpp

        database/ChatRepository.cpp
        database/DatabaseManager.cpp
        database/MessageRepository.cpp

        ui/viewModels/ModelSelectorViewModel.cpp
        ui/viewModels/ChatViewModel.cpp
        ui/viewModels/MessageModel.cpp
        ui/qml/QmlModule.cpp

        core/Markdown.cpp
)
set(INCLUDE
        ../include/network/LLMClient.h
        ../include/config/AppSettings.h

        ../include/database/ChatRepository.h
        ../include/database/DatabaseManager.h
        ../include/database/MessageRepository.h

        ../include/ui/viewModels/ModelSelectorViewModel.h
        ../include/ui/viewModels/ChatViewModel.h
        ../include/ui/viewModels/MessageModel.h
        ../include/ui/qml/QmlModule.h

        ../include/core/Markdown.h
)
add_library(chatgptlib STATIC ${SRC_FILES} ${INCLUDE}
)
target_include_directories(chatgptlib PUBLIC
        ${CMAKE_SOURCE_DIR}/include

        ${CMAKE_SOURCE_DIR}/include/database

        ${CMAKE_SOURCE_DIR}/include/ui
        ${CMAKE_SOURCE_DIR}/include/ui/viewModels
        ${CMAKE_SOURCE_DIR}/include/ui/qml

        ${CMAKE_SOURCE_DIR}/include/core

        ${CMAKE_SOURCE_DIR}/include/network

        ${CMAKE_SOURCE_DIR}/include/config
)
target_link_libraries(chatgptlib PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Sql
        Qt6::Qml
        ${CURL_TARGET}
)