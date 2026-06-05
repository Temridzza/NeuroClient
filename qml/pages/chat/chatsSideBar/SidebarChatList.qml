import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import ChatGPT 1.0
import NeuroClient 1.0

ListView {
    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true

    model: App.chats

    delegate: SidebarChatDelegate {
        onRenameRequested: function(chatId, currentTitle) {
            renameDialog.chatId = chatId
            renameDialog.currentTitle = currentTitle
            renameDialog.open()
        }

        onDeleteRequested: function(chatId) {
            App.chats.deleteChat(chatId)

        }
    }

    Dialog {
        id: renameDialog
        modal: true
        title: "Переименовать чат"
        standardButtons: Dialog.Ok | Dialog.Cancel

        property string chatId: ""
        property string currentTitle: ""

        onOpened: titleField.forceActiveFocus()

        contentItem: ColumnLayout {
            spacing: 12
            // padding: 16

            TextField {
                id: titleField
                Layout.preferredWidth: 320
                text: renameDialog.currentTitle
                selectByMouse: true
            }
        }

        onAccepted: {
            App.chats.renameChat(renameDialog.chatId, titleField.text.trim())
        }
    }
}