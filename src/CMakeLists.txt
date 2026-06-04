# src/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Sql Qml)
set(SRC_FILES

    # =========================
    # Core
    # =========================

    core/ApplicationOrchestrator.cpp
    core/ChatOrchestrator.cpp
    core/Markdown.cpp
    core/PromptBuilder.cpp

    # =========================
    # Services
    # =========================

    service/SettingsService.cpp
    service/ModelService.cpp
    service/ProviderService.cpp
    service/MessageService.cpp
    service/ChatService.cpp
    service/ChatContextService.cpp

    # =========================
    # Network
    # =========================

    network/LLMClient.cpp

    # =========================
    # Storage / Config
    # =========================

    config/AppSettings.cpp

    # =========================
    # Database
    # =========================

    database/DatabaseManager.cpp

    database/ChatRepository.cpp
    database/MessageRepository.cpp
    database/ProviderRepository.cpp
    database/ModelRepository.cpp

    # =========================
    # UI / ViewModels
    # =========================

    ui/viewModels/ChatViewModel.cpp
    ui/viewModels/ChatListViewModel.cpp

    ui/viewModels/MessageModel.cpp

    ui/viewModels/ProviderModel.cpp
    ui/viewModels/ModelsModel.cpp
    ui/viewModels/ModelSelector.cpp
    ui/viewModels/ProviderSelector.cpp

    # =========================
    # QML
    # =========================


)

set(INCLUDE

    # =========================
    # Core
    # =========================

    ../include/core/ApplicationOrchestrator.h
    ../include/core/ChatOrchestrator.h
    ../include/core/PromptBuilder.h

    ../include/core/Markdown.h
    ../include/core/Message.h
    ../include/core/Result.h
    ../include/core/PromptContext.h
    ../include/core/Chat.h
    ../include/core/Model.h
    ../include/core/Provider.h
    ../include/core/ChatSession.h

    # =========================
    # Services
    # =========================

    ../include/service/SettingsService.h
    ../include/service/ModelService.h
    ../include/service/ProviderService.h
    ../include/service/ChatService.h
    ../include/service/MessageService.h
    ../include/service/ChatContextService.h

    # =========================
    # Network
    # =========================

    ../include/network/LLMClient.h

    # =========================
    # Storage / Config
    # =========================

    ../include/config/AppSettings.h

    # =========================
    # Database
    # =========================

    ../include/database/DatabaseManager.h

    ../include/database/ChatRepository.h
    ../include/database/MessageRepository.h
    ../include/database/ProviderRepository.h
    ../include/database/ModelRepository.h

    # =========================
    # UI / ViewModels
    # =========================

    ../include/ui/viewModels/ChatViewModel.h
    ../include/ui/viewModels/ChatListViewModel.h

    ../include/ui/viewModels/MessageModel.h

    ../include/ui/viewModels/ProviderModel.h
    ../include/ui/viewModels/ModelsModel.h
    ../include/ui/viewModels/ModelSelector.h
    ../include/ui/viewModels/ProviderSelector.h

    # =========================
    # QML
    # =========================


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
        ${CMAKE_SOURCE_DIR}/include/service
)
target_link_libraries(chatgptlib PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Sql
        Qt6::Qml
        ${CURL_TARGET}
)