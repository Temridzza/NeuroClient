import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import Theme 1.0
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
        id: row

        width: chatView.width
        implicitHeight: bubble.implicitHeight + 8

        readonly property bool isUser: role === "user"

        Rectangle {
            id: bubble

            property int pad: 14
            property int maxBubbleWidth: Math.round(chatView.width * 0.82)
            property bool hovered: false

            anchors {
                top: parent.top
                right: isUser ? parent.right : undefined
                left: !isUser ? parent.left : undefined
                rightMargin: isUser ? 10 : 0
                leftMargin: !isUser ? 10 : 0
            }

            width: Math.min(
                maxBubbleWidth,
                Math.max(120, metrics.boundingRect.width + pad * 2)
            )

            implicitHeight: textItem.implicitHeight + pad * 2

            radius: 18
            color: isUser ? Theme.accentColor : Theme.panelColor
            border.color: Theme.borderColor

            HoverHandler {
                onHoveredChanged: bubble.hovered = hovered
            }

            TapHandler {
                acceptedButtons: Qt.RightButton
                onTapped: contextMenu.popup()
            }

            // ======================
            // COPY BUTTON
            // ======================
            ToolButton {
                id: copyButton

                width: 28
                height: 28

                anchors {
                    right: parent.right
                    top: parent.top
                    margins: 6
                }

                visible: bubble.hovered || textItem.selectedText.length > 0
                opacity: visible ? 1.0 : 0.0
                scale: visible ? 1.0 : 0.9

                Behavior on opacity { NumberAnimation { duration: 120 } }
                Behavior on scale   { NumberAnimation { duration: 120 } }

                background: Rectangle {
                    radius: 14
                    color: Qt.rgba(1, 1, 1, 0.08)
                }

                Text {
                    text: Theme.iconCopy
                    font.pixelSize: 25
                }

                // ✅ MAIN CHANGE HERE
                onClicked: App.messages.copyAt(index, MessageModel.RawMarkdown)
            }

            // ======================
            // TEXT
            // ======================
            TextEdit {
                id: textItem

                anchors.fill: parent
                anchors.margins: bubble.pad

                text: sidebarAnimating ? "…" : (htmlReady ? html : message)
                textFormat: htmlReady ? TextEdit.RichText : TextEdit.PlainText

                readOnly: true
                selectByMouse: true
                persistentSelection: true
                cursorVisible: false
                wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                renderType: Text.NativeRendering

                color: Theme.textColor
                font.pixelSize: 15

                selectionColor: Theme.selectionColor
                selectedTextColor: Theme.textColor

                onLinkActivated: Qt.openUrlExternally(link)
            }

            // ======================
            // CONTEXT MENU
            // ======================
            Menu {
                id: contextMenu

                MenuItem {
                    text: "Копировать"
                    onTriggered: App.messages.copyAt(index, App.RawMarkdown)
                }

                MenuItem {
                    text: "Копировать как текст"
                    onTriggered: App.messages.copyAt(index, App.PlainText)
                }

                MenuItem {
                    text: "Копировать как видно"
                    onTriggered: App.messages.copyAt(index, App.VisibleText)
                }

                MenuSeparator {}

                MenuItem {
                    text: "Копировать выделенное"
                    enabled: textItem.selectedText.length > 0
                    onTriggered: App.messages.copyAt(index, App.PlainText)
                }

                MenuItem {
                    text: "Выделить всё"
                    onTriggered: textItem.selectAll()
                }
            }

            TextMetrics {
                id: metrics
                font: textItem.font
                text: sidebarAnimating ? "…" : message
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