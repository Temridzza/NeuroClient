# tests/CmakeLists.txt
add_executable(UnitTests
        ChatOrhestrator_test.cpp
        main.cpp
        service/ProjectContextService_test.cpp
        ProjectTreeModel_test.cpp
        # ChatGPTConfig_test.cpp
        # MainWindow_test.cpp
        # ChatGPTClient_test.cpp

        # database/ChatRepository_test.cpp
        # database/DatabaseManager_test.cpp
        # database/MessageRepository_test.cpp
)

target_link_libraries(UnitTests PRIVATE
        GTest::gtest
        GTest::gtest_main

        Qt6::Core
        Qt6::Gui
        Qt6::Sql

        ${CURL_TARGET}
        ${CMARK_GFM_LIBRARIES}

        chatgptlib
)

include(GoogleTest)
gtest_discover_tests(UnitTests)