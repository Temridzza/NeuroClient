import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import ChatGPT 1.0
import NeuroClient 1.0
import Theme 1.0
import qs.modules.theme
import qs.modules.components

StyledRect {
    id: root
    anchors.fill: parent
    radius: 50
    variant: "transparent"
    clip: true

    property bool sidebarOpened: false
    property bool rightSidebarOpened: false

    property bool showChatSettings: false
    property string currentChatId: ""

    signal requestSettings()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        Item {
            Layout.fillWidth: true
            height: header.height

            ChatHeader {
                id: header
                anchors.fill: parent

                sidebarOpened: root.sidebarOpened
                rightSidebarOpened: root.rightSidebarOpened

                onToggleSidebar: root.sidebarOpened = !root.sidebarOpened
                onToggleRightSidebar: root.rightSidebarOpened = !root.rightSidebarOpened

                onSettings: root.requestSettings()
            }
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
            visible: App.chat.hasChat && !root.showChatSettings
            loading: App.chat.loading

            onSendRequested: function(text) {
                App.chat.sendMessage(text)
            }
        }
    }

    // -------------------------
    // Компоненты Chat
    // -------------------------
    Component {
        id: chatComponent
        ChatList {}
    }

    Component {
        id: emptyChatComponent
        EmptyChatView {}
    }

    Component {
        id: chatSettingsComponent
        ChatSettingsPage {
            chatId: root.currentChatId
            onBackRequested: root.showChatSettings = false
        }
    }
}