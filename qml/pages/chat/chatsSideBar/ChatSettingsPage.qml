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

            Binding {
                target: titleField
                property: "text"
                value: vm.chatTitle
            }
        }

        // ─── Провайдер ─────────────────────────
        Label { text: "Провайдер" }
        ReusableComboBox {
            id: providerBox
            Layout.fillWidth: true

            itemsModel: App.providerSelector.providers
            selectedIndex: App.providerSelector.currentProviderIndex
            onItemSelected: function(id) { App.providerSelector.currentProvider = id }
        }

        // ─── Модель ───────────────────────────
        Label { text: "Модель" }
        ReusableComboBox {
            id: modelBox
            Layout.fillWidth: true

            itemsModel: App.modelSelector.models
            selectedIndex: App.modelSelector.currentModelIndex
            onItemSelected: function(id) { App.modelSelector.currentModel = id }
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

                Binding {
                    target: promptEdit
                    property: "text"
                    value: vm.prompt
                }
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