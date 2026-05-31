import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../../../theme"
import ChatGPT 1.0
import NeuroClient 1.0

RowLayout {
    Layout.fillWidth: true
    spacing: 10

    Text {
        text: "Чаты"
        color: Theme.textColor
        font.pixelSize: 22
        font.bold: true
    }

    Item { Layout.fillWidth: true }

    ToolButton {
        text: "✕"
        onClicked: root.sidebarOpened = !root.sidebarOpened
    }
}
