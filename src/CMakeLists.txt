# src/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Sql Qml)
set(SRC_FILES
        network/LLMClient.cpp
        config/AppSettings.cpp

        database/ChatRepository.cpp
        database/DatabaseManager.cpp
        database/MessageRepository.cpp
        database/ProviderRepository.cpp
        database/ModelRepository.cpp

        ui/viewModels/ModelSelector.cpp
        ui/viewModels/ChatViewModel.cpp
        ui/viewModels/MessageModel.cpp
        ui/viewModels/ChatListViewModel.cpp
        ui/viewModels/ProviderModel.cpp
        ui/viewModels/ModelsModel.cpp

        ui/qml/QmlModule.cpp

        core/Markdown.cpp
        core/ChatOrchestrator.cpp
)
set(INCLUDE
        ../include/network/LLMClient.h
        ../include/config/AppSettings.h

        ../include/database/ChatRepository.h
        ../include/database/DatabaseManager.h
        ../include/database/MessageRepository.h
        ../include/database/ProviderRepository.h
        ../include/database/ModelRepository.h

        ../include/ui/viewModels/ModelSelector.h
        ../include/ui/viewModels/ChatViewModel.h
        ../include/ui/viewModels/MessageModel.h
        ../include/ui/viewModels/ChatListViewModel.h
        ../include/ui/viewModels/ProviderModel.h
        ../include/ui/viewModels/ModelsModel.h

        ../include/ui/qml/QmlModule.h

        ../include/core/Markdown.h
        ../include/core/Message.h
        ../include/core/ChatOrchestrator.h
)
add_library(chatgptlib STATIC ${SRC_FILES} ${INCLUDE})
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