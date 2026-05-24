# src/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Sql Qml)
set(SRC_FILES
        ChatGPTClient.cpp
        ChatGPTConfig.cpp

        database/ChatRepository.cpp
        database/DatabaseManager.cpp
        database/MessageRepository.cpp

        ui/viewModels/ModelSelectorViewModel.cpp
        ui/qml/QmlModule.cpp
)
set(INCLUDE
        ../include/ChatGPTClient.h
        ../include/ChatGPTConfig.h

        ../include/database/ChatRepository.h
        ../include/database/DatabaseManager.h
        ../include/database/MessageRepository.h

        ../include/ui/viewModels/ModelSelectorViewModel.h
        ../include/ui/qml/QmlModule.h
)
add_library(chatgptlib STATIC ${SRC_FILES}
        ui/qml/QmlModule.cpp)
target_include_directories(chatgptlib PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/database
        ${CMAKE_SOURCE_DIR}/include/ui
        ${CMAKE_SOURCE_DIR}/include/ui/viewModels
        ${CMAKE_SOURCE_DIR}/include/ui/qml
)
target_link_libraries(chatgptlib PUBLIC
        Qt6::Core Qt6::Gui Qt6::Sql Qt6::Qml ${CURL_TARGET}
)