import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../../theme"
import "../chat"
import ChatGPT 1.0
import NeuroClient

Rectangle {
    id: root

    property bool sidebarOpened: true
    property bool sidebarAnimating: false

    signal requestSettings()

    radius: 24
    color: Theme.chatBgColor
    border.color: Theme.borderColor

    ChatsSideBar {
        id: sidebar

        onAnimationStateChanged: function(animating) {
            root.sidebarAnimating = animating
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        ChatHeader {
            Layout.fillWidth: true

            sidebarOpened: root.sidebarOpened

            onToggleSidebar: {
                root.sidebarAnimating = true
                root.sidebarOpened = !root.sidebarOpened
            }

            onSettings: root.requestSettings()
        }

        ChatList {
            Layout.fillWidth: true
            Layout.fillHeight: true

            sidebarAnimating: root.sidebarAnimating
        }

        ChatStatusBar {
            Layout.fillWidth: true
            visible: App.chat.loading
        }

        ChatInput {
            Layout.fillWidth: true
            loading: App.chat.loading

            onSendRequested: function(text) {
                App.chat.sendMessage(text)
            }
        }
    }
}