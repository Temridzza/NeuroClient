import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../../theme"
import ChatGPT 1.0
import NeuroClient

ListView {
    id: chatView

    property bool sidebarAnimating: false

    spacing: 12
    clip: true

    model: App.messages

    onContentYChanged: scrollTimer.restart()
    onHeightChanged: ensureVisible()
    onCountChanged: ensureVisible()

    Timer {
        id: scrollTimer
        interval: 50
        repeat: false
        onTriggered: chatView.ensureVisible()
    }

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

                text: chatView.sidebarAnimating ? "…" : (htmlReady ? html : message)
                textFormat: htmlReady ? Text.RichText : Text.PlainText

                color: Theme.textColor
                wrapMode: Text.Wrap
                font.pixelSize: 15
                lineHeight: 1.25
            }
        }

        function checkVisibility() {
            var first = chatView.indexAt(10, chatView.contentY)
            var last = chatView.indexAt(10, chatView.contentY + chatView.height)

            if (first < 0) first = 0
            if (last < 0) last = chatView.count - 1

            for (var i = first; i <= last; i++) {
                App.messages.ensureRendered(i)
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

            var first = chatView.indexAt(10, chatView.contentY)
            var last = chatView.indexAt(10, chatView.contentY + chatView.height)

            if (first < 0) first = 0
            if (last < 0) last = chatView.count - 1

            for (var i = first; i <= last; i++) {
                App.messages.ensureRendered(i)
            }
        }
    }
}