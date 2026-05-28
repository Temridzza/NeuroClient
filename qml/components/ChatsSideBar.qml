import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../theme"
import ChatGPT 1.0
import NeuroClient 1.0

Rectangle {
    id: sidebar

    color: Theme.bgColor
    radius: 22

    width: root.sidebarOpened ? 320 : 0
    Behavior on width { NumberAnimation { duration: 200 } }
    visible: root.sidebarOpened
    layer.enabled: true

    property var chatModel

    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowOpacity: Theme.shadowOpacity
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
                color: Theme.textColor
                font.pixelSize: 22
                font.bold: true
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                text: "✕"
                onClicked: root.sidebarOpened = !root.sidebarOpened
            }
        }

        // NEW CHAT BUTTON
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48

            radius: 14
            color: Theme.accentColor

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked: {
                    console.log("new chat")
                    ChatModel.createChat("Новый чат")
                }
            }

            Row {
                anchors.centerIn: parent
                spacing: 8

                Text {
                    text: "+"
                    color: Theme.textColor
                    font.pixelSize: 20
                    font.bold: true
                }

                Text {
                    text: "Новый чат"
                    color: Theme.textColor
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
            color: Theme.panelColor
            border.color: Theme.borderColor

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                spacing: 10

                Text {
                    text: "🔍"
                    font.pixelSize: 14
                    color: Theme.secondaryText
                }

                TextField {
                    Layout.fillWidth: true

                    placeholderText: "Поиск в чатах"
                    color: Theme.textColor
                    selectedTextColor: "white"

                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }
        }

        // CHAT LIST
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            model: ChatModel

            delegate: Rectangle {
                width: ListView.view.width
                height: 44
                radius: 12

                property string chatTitle: title
                property string chatId: id

                color: mouse.containsMouse
                       ? Theme.hoverColor
                       : "transparent"

                MouseArea {
                    id: mouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        console.log("open chat:", chatId, chatTitle)
                        ChatModel.openChat(chatId)
                    }
                }

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    spacing: 10

                    Rectangle {
                        width: 8
                        height: 8
                        radius: 4
                        color: Theme.accentColor
                    }

                    Text {
                        text: chatTitle
                        color: Theme.textColor
                        font.pixelSize: 14
                        elide: Text.ElideRight
                        width: sidebar.width - 40
                    }
                }
            }
        }
    }
}