import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import qs.modules.theme
import qs.modules.components
import ChatGPT 1.0
import NeuroClient 1.0

StyledRect {
    Layout.fillWidth: true
    Layout.preferredHeight: 48

    radius: 14
    variant: "primary"   // основной фон кнопки

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onEntered: parent.variant = "focus"      // hover
        onExited: parent.variant = "primary"     // обычное
        onPressed: parent.variant = "primary"    // нажатие
        onReleased: parent.variant = containsMouse ? "focus" : "primary"

        onClicked: App.chat.startNewChat()
    }

    Row {
        anchors.centerIn: parent
        spacing: 8

        Text {
            text: "+"
            color: parent.item   // цвет текста из темы
            font.pixelSize: 20
            font.bold: true
        }

        Text {
            text: "Новый чат"
            color: parent.item   // цвет текста из темы
            font.pixelSize: 15
            font.bold: true
        }
    }
}