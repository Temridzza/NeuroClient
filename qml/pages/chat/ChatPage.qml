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

    property string currentChatId: ""
    property bool showChatSettings: false

    signal openChatSettings(string chatId)

    radius: 24
    color: Colors.surfaceContainer
    border.width: 0
    border.color: "transparent"

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
            id: loader
            Layout.fillWidth: true
            Layout.fillHeight: true

            sourceComponent: root.showChatSettings
                    ? chatSettingsComponent
                    : (App.chat.hasChat ? chatComponent : emptyChatComponent)

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

        ChatsSideBar {
            id: sidebar

            onSettingsRequested: {
                root.openChatSettings(chatId)
            }

            onAnimationStateChanged: function(animating) {
                root.sidebarAnimating = animating
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

    Component {
        id: chatSettingsComponent

        ChatSettingsPage {
            chatId: root.currentChatId

            onBackRequested: {
                root.showChatSettings = false
                loader.sourceComponent = App.chat.hasChat ? chatComponent : emptyChatComponent
            }
        }
    }
}