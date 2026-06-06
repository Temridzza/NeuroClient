import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Effects

import "pages/chat"
import "pages/chat/chatsSideBar"
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

    // -------------------------
    // Общие свойства
    // -------------------------
    property bool sidebarOpened: false
    property string currentPage: "chat"
    property string currentChatId: ""
    property bool showChatSettings: false

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

        width: sidebarOpened ? 320 : 0
        z: 1000
        clip: true
        visible: width > 0

        Behavior on width {
            NumberAnimation { duration: 180; easing.type: Easing.OutCubic }
        }

        ChatsSideBar {
            anchors.fill: parent

            onSettingsRequested: function(chatId) {
                console.log("MAIN GOT openChatSettings:", chatId)
                root.currentChatId = chatId
                root.showChatSettings = true
            }
        }
    }

    // -------------------------
    // SCRIM (затемнение)
    // -------------------------
    StyledRect {
        anchors.fill: parent

        variant: "pane"
        color: "black"
        opacity: sidebarOpened ? 0.35 : 0
        visible: opacity > 0
        z: 900

        Behavior on opacity {
            NumberAnimation { duration: 180 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.sidebarOpened = false
        }
    }

    // -------------------------
    // ChatPage Component
    // -------------------------
    Component {
        id: chatPage

        ChatPage {
            sidebarOpened: root.sidebarOpened
            currentChatId: root.currentChatId
            showChatSettings: root.showChatSettings

            onSidebarOpenedChanged: root.sidebarOpened = sidebarOpened
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