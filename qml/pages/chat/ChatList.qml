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
    onHeightChanged: tickVisibleRange()
    onCountChanged: tickVisibleRange()

    Timer {
        id: scrollTimer
        interval: 50
        repeat: false
        onTriggered: chatView.tickVisibleRange()
    }

    function tickVisibleRange() {
        var first = indexAt(10, contentY)
        var last = indexAt(10, contentY + height)

        if (first < 0) first = 0
        if (last < 0) last = count - 1

        for (var i = first; i <= last; i++) {
            App.messages.ensureRendered(i)
        }
    }

    Component.onCompleted: tickVisibleRange()

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
    }

    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AsNeeded
    }

    Connections {
        target: App.messages
        function onRowsInserted() {
            chatView.positionViewAtEnd()
            chatView.tickVisibleRange()
        }
    }

    Connections {
        target: App.chat
        function onMessagesChanged() {
            chatView.positionViewAtEnd()
            chatView.tickVisibleRange()
        }
    }

    Connections {
        target: App.chatOrchestrator
        function onChatLoadingFinished(chatId) {
            chatView.positionViewAtEnd()
            chatView.tickVisibleRange()
        }
    }
}