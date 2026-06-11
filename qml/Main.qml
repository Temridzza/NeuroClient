import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Effects

import "pages/chat"
import "pages/chat/chatsSideBar"
import "pages/chat/sideBarContext"

import "settings"

import ChatGPT 1.0
import NeuroClient 1.0
import Theme 1.0

import qs.modules.theme
import qs.modules.components

StyledRect {
    id: root

    implicitWidth: 1280
    implicitHeight: 820

    variant: "transparent"

    property string sidebarState: "closed"
    property string rightSidebarState: "closed"

    property string currentPage: "chat"
    property string currentChatId: ""
    property bool showChatSettings: false

    function toggleSidebar() {
        if (sidebarState === "closed") {
            sidebarState = "opening"
        } else if (sidebarState === "open") {
            sidebarState = "closing"
        }
    }

    function toggleRightSidebar() {
        if (rightSidebarState === "closed") {
            rightSidebarState = "opening"
        } else if (rightSidebarState === "open") {
            rightSidebarState = "closing"
        }
    }

    // -------------------------
    // Основной контент
    // -------------------------
    Loader {
        id: pageLoader
        anchors.fill: parent

        sourceComponent: root.currentPage === "settings"
                         ? settingsPage
                         : chatPage
    }

    // -------------------------
    // SIDEBAR OVERLAY (surface layer)
    // -------------------------
    StyledRect {
        id: sideBarContainer

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        variant: "internalbg"
        z: 1000
        clip: true
        visible: true

        width: (root.sidebarState === "open" || root.sidebarState === "opening")
               ? 320 : 0

        Behavior on width {
            NumberAnimation {
                duration: 180
                easing.type: Easing.OutCubic

                onRunningChanged: {
                    if (!running) {
                        if (root.sidebarState === "opening") {
                            root.sidebarState = "open"
                        } else if (root.sidebarState === "closing") {
                            root.sidebarState = "closed"
                        }
                    }
                }
            }
        }

        ChatsSideBar {
            anchors.fill: parent

            onSettingsRequested: function(chatId) {
                root.currentChatId = chatId
                root.showChatSettings = true
            }
        }
    }

    // -------------------------
    // RIGHT SIDEBAR OVERLAY
    // -------------------------
    StyledRect {
        id: rightSideBarContainer

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        variant: "internalbg"
        z: 1000
        clip: true
        visible: true

        width: (root.rightSidebarState === "open" || root.rightSidebarState === "opening")
               ? 320 : 0

        Behavior on width {
            NumberAnimation {
                duration: 180
                easing.type: Easing.OutCubic

                onRunningChanged: {
                    if (!running) {
                        if (root.rightSidebarState === "opening") {
                            root.rightSidebarState = "open"
                        } else if (root.rightSidebarState === "closing") {
                            root.rightSidebarState = "closed"
                        }
                    }
                }
            }
        }

        ProjectContextSidebar {
            anchors.fill: parent
        }
    }

    // -------------------------
    // SCRIM (затемнение)
    // -------------------------
    StyledRect {
        anchors.fill: parent

        variant: "pane"
        color: "black"
        opacity: root.sidebarState === "open" || root.sidebarState === "opening" ? 0.35 : 0
        visible: opacity > 0
        z: 900

        Behavior on opacity {
            NumberAnimation { duration: 180 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.sidebarState = "closing"
        }
    }

    StyledRect {
        anchors.fill: parent

        variant: "pane"
        color: "black"
        opacity: root.rightSidebarState === "open" || root.rightSidebarState === "opening" ? 0.35 : 0
        visible: opacity > 0
        z: 900

        Behavior on opacity {
            NumberAnimation { duration: 180 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.rightSidebarState = "closing"
        }
    }

    // -------------------------
    // ChatPage Component
    // -------------------------
    Component {
        id: chatPage

        ChatPage {
            sidebarState: root.sidebarState === "open" || root.sidebarState === "opening"
            rightSidebarState: root.rightSidebarState === "open" || root.rightSidebarState === "opening"

            currentChatId: root.currentChatId
            showChatSettings: root.showChatSettings

            onRequestToggleSidebar: root.toggleSidebar()
            onRequestToggleRightSidebar: root.toggleRightSidebar()

            onRequestSettings: root.currentPage = "settings"
        }
    }

    // -------------------------
    // SettingsPage Component
    // -------------------------
    Component {
        id: settingsPage

        SettingsPage {
            anchors.fill: parent
            onRequestBack: root.currentPage = "chat"
        }
    }
}