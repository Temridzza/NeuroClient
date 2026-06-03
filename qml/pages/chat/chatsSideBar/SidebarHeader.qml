import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import qs.modules.theme
import ChatGPT 1.0
import NeuroClient 1.0

RowLayout {
    Layout.fillWidth: true
    spacing: 10

    Text {
        text: "Чаты"
        color: Colors.overBackground
        font.pixelSize: 22
        font.bold: true
    }

    Item { Layout.fillWidth: true }

    ToolButton {
        Text {
            text: "✕"
            color: Colors.overBackground
        }
        onClicked: root.sidebarOpened = !root.sidebarOpened
    }
}
