qt_add_executable(ChatGPTApp main.cpp)

target_link_libraries(ChatGPTApp PRIVATE
    Qt6::Quick
    Qt6::Qml
)