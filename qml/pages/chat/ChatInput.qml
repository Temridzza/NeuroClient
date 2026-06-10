import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import qs.modules.theme
import qs.modules.components
import ChatGPT 1.0
import NeuroClient
import "../../components"

StyledRect {
    id: root
    property bool loading: false
    signal sendRequested(string text)

    radius: 20
    variant: "pane" // основной фон контейнера

    implicitHeight: editorHeight + 70

    // ---------- высота редактора ----------
    property int editorHeight: Math.min(
                                   Math.max(inputField.contentHeight + 20, 40),
                                   160)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        // ---------- Текстовый ввод ----------
        ScrollView {
            id: editorScroll
            Layout.fillWidth: true
            Layout.preferredHeight: root.editorHeight
            clip: true

            TextArea {
                id: inputField
                width: editorScroll.availableWidth

                placeholderText: qsTr("Напишите сообщение...")
                color: Styling.srItem("overpane") // цвет текста по теме
                wrapMode: TextEdit.Wrap

                background: StyledRect {
                    variant: "pane"
                    radius: 16
                }

                leftPadding: 0; rightPadding: 0
                topPadding: 0; bottomPadding: 0

                // --------------------------------------------------
                //  Обработка горячих клавиш
                // --------------------------------------------------
                function isHotkey(event, binding) {
                    if (binding === "Return") {
                        return (event.key === Qt.Key_Return ||
                                event.key === Qt.Key_Enter) &&
                               !(event.modifiers & Qt.ShiftModifier)
                    }
                    if (binding === "Shift+Return") {
                        return (event.key === Qt.Key_Return ||
                                event.key === Qt.Key_Enter) &&
                               (event.modifiers & Qt.ShiftModifier)
                    }
                    if (binding === "Escape")
                        return event.key === Qt.Key_Escape
                    return false
                }

                Keys.onPressed: function(event) {
                    const sendKey    = App.settingsService.inputSendHotkey()
                    const newlineKey = App.settingsService.inputNewlineHotkey()
                    const clearKey   = App.settingsService.inputClearHotkey()

                    // ---------------- SEND ----------------
                    if (isHotkey(event, sendKey)) {
                        event.accepted = true
                        root.send()
                        return
                    }

                    // ---------------- NEWLINE ----------------
                    if (isHotkey(event, newlineKey))
                        return

                    // ---------------- CLEAR ----------------
                    if (isHotkey(event, clearKey)) {
                        inputField.clear()
                        event.accepted = true
                        return
                    }
                }
            }
        }

        // ---------- Кнопки управления ----------
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            // StyledToolButton {
            //     buttonText: "📎"
            // }

            Item { Layout.fillWidth: true }

            StyledToolButton {
                buttonText: "➤"
                onClicked: root.send()
            }
        }
    }

    // ----------------------------------------------------------------------
    //  Функция отправки сообщения
    // ----------------------------------------------------------------------
    function send() {
        const text = inputField.text.trim()
        if (text.length === 0)
            return
        root.sendRequested(text)
        inputField.clear()
    }
}