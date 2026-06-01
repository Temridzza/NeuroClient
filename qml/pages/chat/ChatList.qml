import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Qt.labs.platform 1.1

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
        clip: true

        Rectangle {
            id: bubble

            property int pad: 14
            property int maxWidth: chatView.width * 0.82

            property bool hovered: false

            width: Math.min(maxWidth, textItem.implicitWidth + pad * 2)
            implicitHeight: textItem.implicitHeight + pad * 2

            radius: 18
            color: role === "user" ? Theme.accentColor : Theme.panelColor
            border.color: Theme.borderColor

            anchors {
                right: role === "user" ? parent.right : undefined
                left: role !== "user" ? parent.left : undefined
            }

            anchors.rightMargin: role === "user" ? 10 : 0
            anchors.leftMargin: role !== "user" ? 10 : 0

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: bubble.hovered = true
                onExited: bubble.hovered = false
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton

                onClicked: function(mouse) {
                    if (mouse.button === Qt.RightButton) {
                        contextMenu.popup()
                    }
                }
            }

            Menu {
                id: contextMenu

                MenuItem {
                    text: "Копировть"
                    onTriggered: Qt.callLater(() => Qt.application.clipboard.setText(message))
                }
            }

            Item {
                id: copyButton
                width: 28
                height: 28

                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 6

                opacity: bubble.hovered ? 1.0 : 0.0
                scale: bubble.hovered ? 1.0 : 0.85

                Behavior on opacity { NumberAnimation { duration: 120 } }
                Behavior on scale { NumberAnimation { duration: 120 } }

                Text {
                    text: Theme.iconCopy
                    font.pixelSize: 25
                    color: Theme.textColor
                }

                MouseArea {
                    id: hoverArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onContainsMouseChanged: {
                        bubble.hovered = containsMouse
                    }
                }
            }

            Text {
                id: textItem

                anchors.fill: parent
                anchors.margins: bubble.pad

                text: chatView.sidebarAnimating ? "…" : (htmlReady ? html : message)
                textFormat: htmlReady ? Text.RichText : Text.PlainText

                wrapMode: Text.Wrap
                color: Theme.textColor
                font.pixelSize: 15
                lineHeight: 1.25
                renderType: Text.NativeRendering
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