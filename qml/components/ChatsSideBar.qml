import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Rectangle {
    id: sidebar

    width: 320
    color: "#11131a"
    radius: 22

    property color panelColor: "#1a1d26"
    property color hoverColor: "#252938"
    property color borderColor: "#2b3040"
    property color accentColor: "#7c5cff"
    property color textColor: "#ffffff"
    property color secondaryText: "#9097aa"

    layer.enabled: true

    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowOpacity: 0.35
        shadowBlur: 0.8
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14

        spacing: 12

        // HEADER
        RowLayout {
            Layout.fillWidth: true

            spacing: 10

            Text {
                text: "Чаты"

                color: textColor
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

        // NEW CHAT BUTTON
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48

            radius: 14
            color: accentColor

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked: {
                    console.log("new chat")
                }
            }

            Row {
                anchors.centerIn: parent
                spacing: 8

                Text {
                    text: "+"
                    color: "white"
                    font.pixelSize: 20
                    font.bold: true
                }

                Text {
                    text: "Новый чат"
                    color: "white"
                    font.pixelSize: 15
                    font.bold: true
                }
            }
        }

        // SEARCH
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 42

            radius: 12
            color: panelColor
            border.color: borderColor

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12

                spacing: 10

                Text {
                    text: "🔍"
                    font.pixelSize: 14
                }

                TextField {
                    Layout.fillWidth: true

                    placeholderText: "Поиск в чатах"
                    color: textColor
                    selectedTextColor: "white"

                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }
        }

        // CHAT LIST
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true

            Column {
                width: sidebar.width - 28
                spacing: 18

                // PROJECTS
                Column {
                    width: parent.width
                    spacing: 8

                    Text {
                        text: "ПРОЕКТЫ"

                        color: secondaryText
                        font.pixelSize: 11
                        font.bold: true
                    }

                    Repeater {
                        model: [
                            "NeuroClient",
                            "AI Assistant",
                            "QML UI"
                        ]

                        delegate: Rectangle {
                            width: parent.width
                            height: 44

                            radius: 12
                            color: mouse.containsMouse
                                   ? hoverColor
                                   : "transparent"

                            border.color: "#00000000"

                            MouseArea {
                                id: mouse

                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                            }

                            Row {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 12

                                spacing: 10

                                Rectangle {
                                    width: 10
                                    height: 10
                                    radius: 5
                                    color: accentColor
                                }

                                Text {
                                    text: modelData
                                    color: textColor
                                    font.pixelSize: 14
                                }
                            }
                        }
                    }
                }

                // RECENT
                Column {
                    width: parent.width
                    spacing: 8

                    Text {
                        text: "НЕДАВНЕЕ"

                        color: secondaryText
                        font.pixelSize: 11
                        font.bold: true
                    }

                    Repeater {
                        model: [
                            "Квантовая физика",
                            "Linux команды",
                            "Помощь с кодом",
                            "OpenAI API",
                            "Qt Quick Effects",
                            "Wayland clipboard"
                        ]

                        delegate: Rectangle {
                            width: parent.width
                            height: 44

                            radius: 12

                            color: mouse.containsMouse
                                   ? hoverColor
                                   : "transparent"

                            MouseArea {
                                id: mouse

                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                            }

                            Row {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 12

                                spacing: 10

                                Text {
                                    text: "○"
                                    color: secondaryText
                                    font.pixelSize: 10
                                }

                                Text {
                                    text: modelData

                                    color: textColor
                                    font.pixelSize: 14

                                    elide: Text.ElideRight
                                    width: 220
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
