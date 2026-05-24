import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    width: 380
    color: "#0f1117"
    radius: 18

    property string currentProject: "~/Projects/NeuroClient"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 16

        // HEADER
        RowLayout {
            Layout.fillWidth: true

            Label {
                text: "Контекст"
                color: "white"
                font.pixelSize: 22
                font.bold: true
            }

            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                text: "⚙"
            }

            ToolButton {
                text: "✕"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#232633"
        }

        // PROJECT SECTION
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "Текущий проект"
                color: "#b4b8c5"
                font.pixelSize: 13
            }

            Rectangle {
                Layout.fillWidth: true
                height: 44
                radius: 12
                color: "#171a22"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12

                    Label {
                        text: root.currentProject
                        color: "white"
                        elide: Text.ElideMiddle
                        Layout.fillWidth: true
                    }

                    Label {
                        text: "📁"
                        font.pixelSize: 18
                    }
                }
            }

            RowLayout {
                spacing: 8

                Button {
                    text: "Открыть проект"
                }

                Button {
                    text: "Недавние ▼"
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#232633"
        }

        // PROJECT TREE
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            Label {
                text: "Дерево проекта"
                color: "white"
                font.pixelSize: 18
                font.bold: true
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Column {
                    width: parent.width
                    spacing: 6

                    // src
                    Column {
                        spacing: 4

                        Row {
                            spacing: 8

                            CheckBox {
                                checked: true
                            }

                            Label {
                                text: "▼ src/"
                                color: "white"
                            }
                        }

                        Column {
                            anchors.leftMargin: 34
                            x: 34
                            spacing: 4

                            Repeater {
                                model: [
                                    {
                                        name: "main.cpp",
                                        checked: true
                                    },
                                    {
                                        name: "ChatView.qml",
                                        checked: true
                                    },
                                    {
                                        name: "ContextSideBar.qml",
                                        checked: true
                                    },
                                    {
                                        name: "ScenarioBuilder.qml",
                                        checked: false
                                    }
                                ]

                                delegate: Row {
                                    spacing: 8

                                    CheckBox {
                                        checked: modelData.checked
                                    }

                                    Label {
                                        text: modelData.name
                                        color: "#d8dbe7"
                                    }
                                }
                            }
                        }
                    }

                    // core
                    Column {
                        spacing: 4

                        Row {
                            spacing: 8

                            CheckBox {
                                checked: true
                            }

                            Label {
                                text: "▼ core/"
                                color: "white"
                            }
                        }

                        Column {
                            anchors.leftMargin: 34
                            x: 34
                            spacing: 4

                            Repeater {
                                model: [
                                    {
                                        name: "ApiClient.cpp",
                                        checked: true
                                    },
                                    {
                                        name: "ProviderManager.cpp",
                                        checked: true
                                    },
                                    {
                                        name: "ContextManager.cpp",
                                        checked: false
                                    }
                                ]

                                delegate: Row {
                                    spacing: 8

                                    CheckBox {
                                        checked: modelData.checked
                                    }

                                    Label {
                                        text: modelData.name
                                        color: "#d8dbe7"
                                    }
                                }
                            }
                        }
                    }

                    // collapsed folders
                    Repeater {
                        model: [
                            "assets/",
                            "build/"
                        ]

                        delegate: Row {
                            spacing: 8

                            CheckBox {}

                            Label {
                                text: "▶ " + modelData
                                color: "white"
                            }
                        }
                    }

                    // include
                    Column {
                        spacing: 4

                        Row {
                            spacing: 8

                            CheckBox {
                                checked: true
                            }

                            Label {
                                text: "▼ include/"
                                color: "white"
                            }
                        }

                        Column {
                            anchors.leftMargin: 34
                            x: 34
                            spacing: 4

                            Repeater {
                                model: [
                                    {
                                        name: "ApiClient.hpp",
                                        checked: true
                                    },
                                    {
                                        name: "ContextManager.hpp",
                                        checked: false
                                    }
                                ]

                                delegate: Row {
                                    spacing: 8

                                    CheckBox {
                                        checked: modelData.checked
                                    }

                                    Label {
                                        text: modelData.name
                                        color: "#d8dbe7"
                                    }
                                }
                            }
                        }
                    }

                    Row {
                        spacing: 8

                        CheckBox {}

                        Label {
                            text: "README.md"
                            color: "white"
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#232633"
        }

        // QUICK ACTIONS
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Label {
                text: "Быстрые действия"
                color: "white"
                font.pixelSize: 18
                font.bold: true
            }

            RowLayout {
                spacing: 8

                Button {
                    text: "+ Добавить файл"
                }

                Button {
                    text: "+ Добавить папку"
                }
            }

            Button {
                text: "⟳ Обновить дерево"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#232633"
        }

        // AI CONTEXT
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Label {
                text: "AI Контекст"
                color: "white"
                font.pixelSize: 18
                font.bold: true
            }

            GridLayout {
                columns: 2
                columnSpacing: 20
                rowSpacing: 8

                Label {
                    text: "Выбрано файлов:"
                    color: "#9aa1b3"
                }

                Label {
                    text: "6"
                    color: "white"
                }

                Label {
                    text: "Примерно токенов:"
                    color: "#9aa1b3"
                }

                Label {
                    text: "~18.4k"
                    color: "white"
                }

                Label {
                    text: "Примерный размер:"
                    color: "#9aa1b3"
                }

                Label {
                    text: "146 KB"
                    color: "white"
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                Label {
                    text: "Использование контекста"
                    color: "#9aa1b3"
                }

                ProgressBar {
                    Layout.fillWidth: true
                    value: 0.58
                }

                Label {
                    text: "58%"
                    color: "white"
                }
            }

            Label {
                text: "Активная модель: GPT-4o"
                color: "#b4b8c5"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#232633"
        }

        // OPTIONS
        ColumnLayout {
            spacing: 4

            CheckBox {
                text: "Автообновление контекста"
                checked: true
            }

            CheckBox {
                text: "Добавлять открытые вкладки"
                checked: true
            }

            CheckBox {
                text: "Добавлять скрытые файлы"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#232633"
        }

        // FOOTER BUTTONS
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Button {
                Layout.fillWidth: true
                text: "Очистить контекст"
            }

            Button {
                Layout.fillWidth: true
                text: "Отправить в чат"
            }
        }
    }
}