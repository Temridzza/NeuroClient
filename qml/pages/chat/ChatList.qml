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

    maximumFlickVelocity: 12000
    flickDeceleration: 8000

    WheelHandler {
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad

        onWheel: function(event) {
            const factor = 2   // ← скорость прокрутки (главное число)

            chatView.contentY -= event.angleDelta.y * factor
            event.accepted = true
        }
    }

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
        height: bubble.implicitHeight

        Rectangle {
            id: bubble

            property int maxWidth: chatView.width * 0.82
            property int padding: 14

            // ключ: bubble сам вычисляет ширину от текста
            width: Math.min(maxWidth, textItem.implicitWidth + padding * 2)

            anchors {
                right: role === "user" ? parent.right : undefined
                left: role === "assistant" ? parent.left : undefined
            }

            radius: 18
            color: role === "user" ? Theme.accentColor : Theme.panelColor
            border.color: Theme.borderColor

            clip: true

            TextArea {
                id: textItem

                width: bubble.width - bubble.padding * 2
                readOnly: true
                wrapMode: TextArea.Wrap

                textFormat: htmlReady ? TextEdit.RichText : TextEdit.PlainText
                text: message

                background: null
                color: Theme.textColor

                padding: 0
            }

            implicitHeight: textItem.implicitHeight + padding * 2
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