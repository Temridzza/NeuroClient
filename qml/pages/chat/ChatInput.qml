// --------------------------------------------------------------------------
//  InputBox.qml – поле ввода сообщения
//  Все цвета берутся из синглтона Colors (colors.qml)
// --------------------------------------------------------------------------
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// Убираем импорт Theme → вместо него импортируем Colors
import qs.modules.theme
import Theme 1.0
import ChatGPT 1.0
import NeuroClient

Rectangle {
    id: root
    property bool loading: false
    signal sendRequested(string text)

    radius: 20
    color: Colors.surface
    border.color: Colors.primary

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
                color: Colors.overSurface
                wrapMode: TextEdit.Wrap

                background: Rectangle { color: "transparent" }

                leftPadding: 0; rightPadding: 0
                topPadding: 0;  bottomPadding: 0

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

                    // ---------------- NEWLINE (always allowed) ----------------
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

            ToolButton {
                Layout.preferredWidth: 36
                Layout.preferredHeight: 36

                background: Rectangle {
                    radius: 18
                    color: Qt.alpha(Colors.primary, 0.12)

                    border.width: 1
                    border.color: Qt.alpha(Colors.primary, 0.25)
                }

                contentItem: Text {
                    text: "📎"
                    color: Colors.primary
                    font.pixelSize: 16

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: root.settings()
            }
            // ToolButton { text: "🌐" }
            // ToolButton { text: "🧠" }

            Item { Layout.fillWidth: true }

            // ---------- Кнопка отправки ----------
            Rectangle {
                width: 42
                height: 42
                radius: 21

                color: (App.chat && loading) ? Colors.surfaceDim
                                            : Colors.primary

                MouseArea {
                    anchors.fill: parent
                    enabled: !(App.chat && loading)
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.send()
                }

                Text {
                    anchors.centerIn: parent
                    text: "➤"
                    color: Colors.background
                    font.pixelSize: 18
                    font.bold: true
                }
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