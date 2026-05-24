# src/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Sql Qml)
set(SRC_FILES
        ChatGPTClient.cpp
        ChatGPTConfig.cpp
        database/ChatRepository.cpp
        database/DatabaseManager.cpp
        database/MessageRepository.cpp
)
set(INCLUDE
        ../include/ChatGPTClient.h
        ../include/ChatGPTConfig.h
        ../include/database/ChatRepository.h
        ../include/database/DatabaseManager.h
        ../include/database/MessageRepository.h
)
add_library(chatgptlib STATIC ${SRC_FILES})
target_include_directories(chatgptlib PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/database
)
target_link_libraries(chatgptlib PUBLIC
        Qt6::Core Qt6::Gui Qt6::Sql Qt6::Qml ${CURL_TARGET}
)