add_executable(UnitTests
        ChatGPTConfig_test.cpp
        MainWindow_test.cpp
        ChatGPTClient_test.cpp

        database/ChatRepository_test.cpp
        database/DatabaseManager_test.cpp
        database/MessageRepository_test.cpp
)

target_link_libraries(UnitTests
        GTest::gtest_main
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Sql
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

# Включить тесты
include(GoogleTest)
gtest_discover_tests(UnitTests)
