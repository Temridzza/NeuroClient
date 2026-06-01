import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Theme 1.0
import ChatGPT 1.0
import NeuroClient

Rectangle {
    id: root

    property bool loading: false
    signal sendRequested(string text)

    radius: 20
    color: Theme.panelColor
    border.color: Theme.borderColor

    implicitHeight: editorHeight + 70

    property int editorHeight: Math.min(Math.max(inputField.contentHeight + 20, 40), 160)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        ScrollView {
            id: editorScroll
            Layout.fillWidth: true
            Layout.preferredHeight: root.editorHeight
            clip: true

            TextArea {
                id: inputField
                width: editorScroll.availableWidth

                placeholderText: "Напишите сообщение..."
                color: Theme.textColor
                wrapMode: TextEdit.Wrap

                background: Rectangle {
                    color: "transparent"
                }

                leftPadding: 0
                rightPadding: 0
                topPadding: 0
                bottomPadding: 0

                function isHotkey(event, binding) {
                    if (binding === "Return") {
                        return (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            && !(event.modifiers & Qt.ShiftModifier)
                    }

                    if (binding === "Shift+Return") {
                        return (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            && (event.modifiers & Qt.ShiftModifier)
                    }

                    if (binding === "Escape") {
                        return event.key === Qt.Key_Escape
                    }

                    return false
                }

                Keys.onPressed: function(event) {

                    const sendKey = App.settingsService.inputSendHotkey()
                    const newlineKey = App.settingsService.inputNewlineHotkey()
                    const clearKey = App.settingsService.inputClearHotkey()

                    // SEND
                    if (isHotkey(event, sendKey)) {
                        event.accepted = true
                        root.send()
                        return
                    }

                    // NEWLINE (разрешаем всегда)
                    if (isHotkey(event, newlineKey)) {
                        return
                    }

                    // CLEAR
                    if (isHotkey(event, clearKey)) {
                        inputField.clear()
                        event.accepted = true
                        return
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            ToolButton { text: "📎" }
            // ToolButton { text: "🌐" }
            // ToolButton { text: "🧠" }

            Item { Layout.fillWidth: true }

            Rectangle {
                width: 42
                height: 42
                radius: 21
                color: (App.chat && loading) ? Theme.disabledColor : Theme.accentColor

                MouseArea {
                    anchors.fill: parent
                    enabled: !(App.chat && loading)
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.send()
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