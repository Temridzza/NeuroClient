// pages/chat/ChatSettingsPage.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import NeuroClient 1.0
import ChatGPT 1.0
import "../../../components"

Pane {
    id: root

    property var vm: App.chatSettings
    property string chatId: ""
    signal backRequested()

    onChatIdChanged: {
        if (chatId.length > 0) {
            vm.getChatInfo(chatId)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        Label {
            text: "Настройки чата"
            font.pixelSize: 22
            font.bold: true
        }

        // ─── Название ───────────────────────────
        Label { text: "Название" }
        TextField {
            id: titleField
            text: vm.chatTitle
            onTextChanged: vm.chatTitle = text
        }

        // ─── Провайдер ─────────────────────────
        Label { text: "Провайдер" }
        ReusableComboBox {
            id: providerBox
            Layout.fillWidth: true
            itemsModel: App.providerSelector.providers
            selectedIndex: {
                for (let i = 0; i < itemsModel.count; i++)
                    if (itemsModel[i].id === vm.providerId)
                        return i
                return -1
            }
            onItemSelected: vm.providerId = id
        }

        // ─── Модель ───────────────────────────
        Label { text: "Модель" }
        ReusableComboBox {
            id: modelBox
            Layout.fillWidth: true
            itemsModel: App.modelSelector.models
            selectedIndex: {
                for (let i = 0; i < itemsModel.count; i++)
                    if (itemsModel[i].id === vm.modelId)
                        return i
                return -1
            }
            onItemSelected: vm.modelId = id
        }

        // ─── Системный промпт ─────────────────
        Label { text: "System Prompt" }
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 250
            TextArea {
                id: promptEdit
                wrapMode: TextEdit.Wrap
                text: vm.prompt
                onTextChanged: vm.prompt = text
                placeholderText: "Введите системный промпт..."
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight

            Button { text: "Отмена" }

            Button {
                text: "Сохранить"
                onClicked: vm.saveChat()
            }
        }
    }
}