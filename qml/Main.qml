import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "components"
import "theme"
import "pages"
import "settings"

import ChatGPT 1.0
import NeuroClient

ApplicationWindow {
    id: root

    width: 1280
    height: 820
    visible: true
    color: "transparent"
    title: "NeuroClient"

    property bool sidebarOpened: false
    property string currentPage: "chat"   // chat | settings

    property var modelVM: ModelSelector {
        id: modelVM
    }
    property var chatVM: ChatVM
    property var chatModel

    Rectangle {
        anchors.fill: parent

        radius: 24
        color: Theme.bgColor
        border.color: Theme.borderColor
        border.width: 1

        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.8
            shadowOpacity: Theme.shadowOpacity
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 14

            // SIDEBAR
            ChatsSideBar {
                id: sideBar

                chatModel: root.chatModel

                Layout.fillHeight: true
                Layout.preferredWidth: sidebarOpened ? 320 : 0
                clip: true
                visible: width > 0

                Behavior on Layout.preferredWidth {
                    NumberAnimation {
                        duration: 180
                        easing.type: Easing.OutCubic
                    }
                }
            }

            // MAIN AREA (ROUTER)
            Loader {
                id: pageLoader

                Layout.fillWidth: true
                Layout.fillHeight: true

                sourceComponent: root.currentPage === "settings"
                    ? settingsPage
                    : chatPage
            }
        }
    }

    // =========================
    // CHAT PAGE
    // =========================
    Component {
        id: chatPage

        ChatPage {
            sidebarOpened: root.sidebarOpened
            modelVM: root.modelVM
            chatVM: root.chatVM
            chatModel: root.chatModel

            onSidebarOpenedChanged: root.sidebarOpened = sidebarOpened

            onRequestSettings: root.currentPage = "settings"
        }
    }

    // =========================
    // SETTINGS PAGE
    // =========================
    Component {
        id: settingsPage

        SettingsPage {
            anchors.fill: parent

            onRequestBack: root.currentPage = "chat"
        }
    }
}