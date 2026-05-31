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
    property bool sidebarAnimating: false

    radius: 24
    color: Theme.chatBgColor
    border.color: Theme.borderColor

    signal requestSettings()

    Timer {
        id: animationTimer
        interval: 220
        repeat: false
        running: false

        onTriggered: {
            root.sidebarAnimating = false
        }
    }

    Connections {
        target: App.chats

        function onChatOpened(chatId) {
            App.chat.setCurrentChat(chatId)
        }
    }

    Connections {
        target: App.chat

        function onMessagesChanged() {
            chatView.positionViewAtEnd()
        }
    }

    Connections {
        target: App.chatOrchestrator

        function onChatLoadingFinished(chatId) {
            chatView.positionViewAtEnd()
            chatView.ensureVisible()

            // форсируем ререндер видимой области
            var first = chatView.indexAt(10, chatView.contentY)
            var last = chatView.indexAt(10, chatView.contentY + chatView.height)

            if (first < 0) first = 0
            if (last < 0) last = chatView.count - 1

            for (var i = first; i <= last; i++) {
                App.messages.ensureRendered(i)
            }
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
                    onClicked: {
                        root.sidebarAnimating = true
                        root.sidebarOpened = !root.sidebarOpened
                        animationTimer.running = true
                    }
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

                    model: App.providerSelector.providers
                    textRole: "display_name"

                    currentIndex: App.providerSelector.currentProviderIndex

                    onActivated: function(index) {
                        App.providerSelector.currentProvider =
                            model[index].id
                    }
                }

                ComboBox {
                    Layout.preferredWidth: 180

                    model: App.modelSelector.models
                    textRole: "display_name"

                    currentIndex: App.modelSelector.currentModelIndex

                    onActivated: function(index) {
                        App.modelSelector.currentModel =
                            model[index].id
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

            model: App.messages

            onContentYChanged: ensureVisible()
            onHeightChanged: ensureVisible()
            onCountChanged: ensureVisible()

            function ensureVisible() {
                var first = indexAt(0, contentY)
                var last = indexAt(0, contentY + height)

                if (first < 0) first = 0
                if (last < 0) last = count - 1

                for (var i = first; i <= last; i++) {
                    App.messages.ensureRendered(i)
                }
            }

            Component.onCompleted: ensureVisible()

            delegate: Item {
                id: rootItem
                width: chatView.width
                height: bubble.height + 8

                property bool isVisibleInView: false

                Component.onCompleted: checkVisibility()
                onYChanged: checkVisibility()

                function checkVisibility() {
                    var first = chatView.indexAt(10, chatView.contentY)
                    var last = chatView.indexAt(10, chatView.contentY + chatView.height)

                    if (first < 0) first = 0
                    if (last < 0) last = chatView.count - 1

                    for (var i = first; i <= last; i++) {
                        App.messages.ensureRendered(i)
                    }
                }

                Connections {
                    target: chatView
                    function onContentYChanged() {
                        rootItem.checkVisibility()
                    }
                }

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

                        // показываем placeholder только если анимация реально идет
                        text: sidebarAnimating ? "…" : (htmlReady ? html : message)
                        textFormat: htmlReady ? Text.RichText : Text.PlainText

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
                target: App.messages
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