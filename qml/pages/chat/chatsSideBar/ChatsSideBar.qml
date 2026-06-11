import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import ChatGPT 1.0
import NeuroClient 1.0
import qs.modules.theme
import qs.modules.components

StyledRect {
    id: sidebar
    signal animationStateChanged(bool animating)
    signal settingsRequested(string chatId)

    property bool animating: false

    radius: 22
    variant: "internalbg"

    width: root.sidebarOpened ? 320 : 0
    visible: width > 0

    border.width: 10
    border.color: Colors.background

    layer.enabled: true
    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowOpacity: Theme.shadowOpacity
        shadowBlur: 0.8
    }

    Behavior on width {
        NumberAnimation {
            duration: 200
            onRunningChanged: sidebar.animationStateChanged(running)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        SidebarHeader { Layout.fillWidth: true }
        SidebarNewChatButton { Layout.fillWidth: true }
        SidebarSearch { Layout.fillWidth: true }
        SidebarChatList {
            Layout.fillWidth: true
            Layout.fillHeight: true

            onSettingsRequested: function(chatId) {
                sidebar.settingsRequested(chatId)
                console.log("sidebar onSettingsRequested " + chatId)
            }
        }
    }
}