import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import ChatGPT 1.0
import NeuroClient 1.0
import Theme 1.0

Rectangle {
    Layout.fillWidth: true
    Layout.preferredHeight: 42

    radius: 12
    color: Theme.panelColor
    border.color: Theme.borderColor

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 10

        Text {
            text: "🔍"
            font.pixelSize: 14
            color: Theme.secondaryText
        }

        TextField {
            Layout.fillWidth: true

            placeholderText: "Поиск в чатах"
            color: Theme.textColor
            selectedTextColor: "white"

            background: Rectangle {
                color: "transparent"
            }
        }
    }
}