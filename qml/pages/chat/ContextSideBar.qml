import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../../theme"
import ChatGPT 1.0

Rectangle {
    id: root

    width: 380
    color: Theme.bgColor
    radius: 18
    border.color: Theme.borderColor

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
                color: Theme.textColor
                font.pixelSize: 22
                font.bold: true
            }

            Item { Layout.fillWidth: true }

            ToolButton { text: "⚙" }
            ToolButton { text: "✕" }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.borderColor
        }

        // PROJECT SECTION
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "Текущий проект"
                color: Theme.secondaryText
                font.pixelSize: 13
            }

            Rectangle {
                Layout.fillWidth: true
                height: 44
                radius: 12
                color: Theme.panelColor

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12

                    Label {
                        text: root.currentProject
                        color: Theme.textColor
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

                Button { text: "Открыть проект" }
                Button { text: "Недавние ▼" }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.borderColor
        }

        // PROJECT TREE
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            Label {
                text: "Дерево проекта"
                color: Theme.textColor
                font.pixelSize: 18
                font.bold: true
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Column {
                    width: parent.width
                    spacing: 6

                    // helper colors
                    property color fileColor: "#d8dbe7"

                    // src
                    Column {
                        spacing: 4

                        Row {
                            spacing: 8

                            CheckBox { checked: true }

                            Label {
                                text: "▼ src/"
                                color: Theme.textColor
                            }
                        }

                        Column {
                            x: 34
                            spacing: 4

                            Repeater {
                                model: [
                                    { name: "main.cpp", checked: true },
                                    { name: "ChatView.qml", checked: true },
                                    { name: "ContextSideBar.qml", checked: true },
                                    { name: "ScenarioBuilder.qml", checked: false }
                                ]

                                delegate: Row {
                                    spacing: 8

                                    CheckBox { checked: modelData.checked }

                                    Label {
                                        text: modelData.name
                                        color: fileColor
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

                            CheckBox { checked: true }

                            Label {
                                text: "▼ core/"
                                color: Theme.textColor
                            }
                        }

                        Column {
                            x: 34
                            spacing: 4

                            Repeater {
                                model: [
                                    { name: "ApiClient.cpp", checked: true },
                                    { name: "ProviderManager.cpp", checked: true },
                                    { name: "ContextManager.cpp", checked: false }
                                ]

                                delegate: Row {
                                    spacing: 8

                                    CheckBox { checked: modelData.checked }

                                    Label {
                                        text: modelData.name
                                        color: fileColor
                                    }
                                }
                            }
                        }
                    }

                    // collapsed folders
                    Repeater {
                        model: ["assets/", "build/"]

                        delegate: Row {
                            spacing: 8

                            CheckBox {}

                            Label {
                                text: "▶ " + modelData
                                color: Theme.textColor
                            }
                        }
                    }

                    // include
                    Column {
                        spacing: 4

                        Row {
                            spacing: 8

                            CheckBox { checked: true }

                            Label {
                                text: "▼ include/"
                                color: Theme.textColor
                            }
                        }

                        Column {
                            x: 34
                            spacing: 4

                            Repeater {
                                model: [
                                    { name: "ApiClient.hpp", checked: true },
                                    { name: "ContextManager.hpp", checked: false }
                                ]

                                delegate: Row {
                                    spacing: 8

                                    CheckBox { checked: modelData.checked }

                                    Label {
                                        text: modelData.name
                                        color: fileColor
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
                            color: Theme.textColor
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.borderColor
        }

        // QUICK ACTIONS
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Label {
                text: "Быстрые действия"
                color: Theme.textColor
                font.pixelSize: 18
                font.bold: true
            }

            RowLayout {
                spacing: 8

                Button { text: "+ Добавить файл" }
                Button { text: "+ Добавить папку" }
            }

            Button {
                text: "⟳ Обновить дерево"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.borderColor
        }

        // AI CONTEXT
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Label {
                text: "AI Контекст"
                color: Theme.textColor
                font.pixelSize: 18
                font.bold: true
            }

            GridLayout {
                columns: 2
                columnSpacing: 20
                rowSpacing: 8

                Label { text: "Выбрано файлов:"; color: Theme.secondaryText }
                Label { text: "6"; color: Theme.textColor }

                Label { text: "Примерно токенов:"; color: Theme.secondaryText }
                Label { text: "~18.4k"; color: Theme.textColor }

                Label { text: "Примерный размер:"; color: Theme.secondaryText }
                Label { text: "146 KB"; color: Theme.textColor }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                Label {
                    text: "Использование контекста"
                    color: Theme.secondaryText
                }

                ProgressBar {
                    Layout.fillWidth: true
                    value: 0.58
                }

                Label {
                    text: "58%"
                    color: Theme.textColor
                }
            }

            Label {
                text: "Активная модель: GPT-4o"
                color: Theme.secondaryText
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.borderColor
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
            color: Theme.borderColor
        }

        // FOOTER
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