import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import ChatGPT 1.0
import NeuroClient 1.0
import Theme 1.0
import qs.modules.theme
import qs.modules.components

StyledRect {
    Layout.fillWidth: true
    Layout.preferredHeight: 42

    radius: 12
    variant: "internalbg"

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 10

        Text {
            text: "🔍"
            font.pixelSize: 14
            color: parent.item
        }

        TextField {
            Layout.fillWidth: true

            placeholderText: "Поиск в чатах"
            color: Styling.srItem("overpane")
            selectedTextColor: parent.item

            background: Rectangle {
                color: "transparent"
            }
        }
    }
}