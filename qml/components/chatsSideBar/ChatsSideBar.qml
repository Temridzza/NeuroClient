import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../../theme"
import ChatGPT 1.0
import NeuroClient 1.0

Rectangle {
    id: sidebar

    color: Theme.bgColor
    radius: 22

    width: root.sidebarOpened ? 320 : 0
    Behavior on width { NumberAnimation { duration: 200 } }
    visible: root.sidebarOpened
    layer.enabled: true

    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowOpacity: Theme.shadowOpacity
        shadowBlur: 0.8
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        SidebarHeader {
            Layout.fillWidth: true
        }

        SidebarNewChatButton {
            Layout.fillWidth: true
        }

        SidebarSearch {
            Layout.fillWidth: true
        }

        SidebarChatList {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}