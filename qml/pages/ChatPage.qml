import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../components"
import "../theme"
import ChatGPT 1.0
import NeuroClient

Rectangle {
    id: root

    property bool sidebarOpened: true
    property var chatVM

    radius: 24
    color: Theme.chatBgColor
    border.color: Theme.borderColor

    signal requestSettings()

    Connections {
        target: App.chats

        function onChatOpened(chatId) {
            console.log("OPEN CHAT:", chatId)

            App.chat.setCurrentChat(chatId)
        }
    }

    Connections {
        target: App.chat

        function onMessagesChanged() {
            chatView.positionViewAtEnd()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        // HEADER
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            radius: 14
            color: Theme.panelColor

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 14
                anchors.rightMargin: 14
                spacing: 10

                ToolButton {
                    text: "☰"
                    onClicked: root.sidebarOpened = !root.sidebarOpened
                }

                Rectangle {
                    width: 28
                    height: 28
                    radius: 14
                    color: Theme.accentColor

                    Text {
                        anchors.centerIn: parent
                        text: "N"
                        color: "white"
                        font.bold: true
                    }
                }

                ComboBox {
                    Layout.preferredWidth: 180
                    model: App.modelSelector.models
                    currentIndex: model.indexOf(App.modelSelector.currentModel)

                    onCurrentTextChanged: {
                        App.modelSelector.currentModel = currentText
                    }
                }

                Item { Layout.fillWidth: true }

                ToolButton {
                    text: "⚙"
                    onClicked: root.requestSettings()
                }
            }
        }

        // CHAT
        ListView {
            id: chatView

            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 12
            clip: true

            model: App.chat.messages

            delegate: Item {
                width: chatView.width
                height: bubble.height + 8

                Rectangle {
                    id: bubble

                    width: Math.min(parent.width * 0.82, textItem.implicitWidth + 32)
                    height: textItem.implicitHeight + 28

                    radius: 18

                    anchors {
                        right: role === "user" ? parent.right : undefined
                        left: role === "assistant" ? parent.left : undefined
                    }

                    color: role === "user"
                        ? Theme.accentColor
                        : Theme.panelColor

                    border.color: Theme.borderColor

                    Text {
                        id: textItem
                        anchors.fill: parent
                        anchors.margins: 14

                        text: html.length > 0 ? html : message
                        textFormat: html.length > 0 ? Text.RichText : Text.PlainText

                        color: Theme.textColor
                        wrapMode: Text.Wrap
                        font.pixelSize: 15
                        lineHeight: 1.25
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }

            Connections {
                target: App.chat.messages
                function onRowsInserted() {
                    chatView.positionViewAtEnd()
                }
            }
        }

        Text {
            visible: App.chat.loading
            text: "Assistant is typing..."
            color: Theme.secondaryText
            font.pixelSize: 13
        }

        ChatInput {
            Layout.fillWidth: true
            loading: App.chat.loading

            onSendRequested: function(text) {
                App.chat.sendMessage(text)
            }
        }
    }
}
