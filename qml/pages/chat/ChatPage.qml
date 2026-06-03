import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import Theme 1.0
import "../chat/chatsSideBar"
import ChatGPT 1.0
import NeuroClient
import qs.modules.theme

Rectangle {
    id: root

    property bool sidebarOpened: true
    property bool sidebarAnimating: false

    signal requestSettings()

    radius: 24
    color: Colors.surfaceContainer
    border.width: 0
    border.color: "transparent"

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

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true

            sourceComponent: App.chat.hasChat
                ? chatComponent
                : emptyChatComponent

            onLoaded: {
                if (item && item.sendRequested) {
                    item.sendRequested.connect(function(text) {
                        App.chat.sendMessage(text)
                    })
                }
            }
        }

        ChatStatusBar {
            Layout.fillWidth: true
            visible: App.chat.loading
        }

        ChatInput {
            Layout.fillWidth: true

            visible: App.chat.hasChat

            loading: App.chat.loading

            onSendRequested: function(text) {
                App.chat.sendMessage(text)
            }
        }
    }

    Component {
        id: chatComponent

        ChatList {
            sidebarAnimating: root.sidebarAnimating
        }
    }

    Component {
        id: emptyChatComponent
        EmptyChatView {

        }
    }
}