import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import ChatGPT 1.0
import NeuroClient 1.0

ListView {
    id: list

    signal settingsRequested(string chatId)

    model: App.chats

    delegate: SidebarChatDelegate {

        onSettingsRequested: function(chatId) {
            list.settingsRequested(chatId)
            console.log("list delegate SidebarChatDelegate " + chatId)
        }

        onDeleteRequested: function(chatId) {
            App.chats.deleteChat(chatId)
        }
    }
}