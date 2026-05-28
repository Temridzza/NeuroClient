import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../theme"
import ChatGPT 1.0

Rectangle {
    id: root

    property bool loading: false

    signal sendRequested(string text)

    radius: 20
    color: Theme.panelColor
    border.color: Theme.borderColor

    height: 110

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        TextArea {
            id: inputField

            Layout.fillWidth: true
            Layout.fillHeight: true

            placeholderText: "Напишите сообщение..."

            color: Theme.textColor
            wrapMode: TextEdit.Wrap

            background: Rectangle {
                color: "transparent"
            }

            Keys.onReturnPressed: function(event) {
                if (!(event.modifiers & Qt.ShiftModifier)) {
                    event.accepted = true
                    root.send()
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            ToolButton { text: "📎" }
            ToolButton { text: "🌐" }
            ToolButton { text: "🧠" }

            Item { Layout.fillWidth: true }

            Rectangle {
                width: 42
                height: 42
                radius: 21

                color: (chatVM && loading) ? Theme.disabledColor : Theme.accentColor

                MouseArea {
                    anchors.fill: parent
                    enabled: !(chatVM && loading)
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        console.log("ChatInput_onClicked_send")
                        root.send()
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: "➤"
                    color: "white"
                    font.pixelSize: 18
                    font.bold: true
                }
            }
        }
    }

    function send() {
        const text = inputField.text.trim()

        if (text.length === 0)
            return

        root.sendRequested(text)
        inputField.clear()
    }
}