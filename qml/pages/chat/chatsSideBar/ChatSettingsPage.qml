// pages/chat/ChatSettingsPage.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import NeuroClient 1.0
import ChatGPT 1.0
import "../../../components"
import qs.modules.theme

Rectangle {
    id: root
    color: "transparent"
    anchors.fill: parent

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
            color: Colors.overBackground
        }

        // ─── Название ───────────────────────────
        Label { text: "Название"; color: Colors.overBackground }
        TextField {
            id: titleField
            Layout.fillWidth: true
            text: vm.chatTitle
            color: Colors.overBackground
            onTextChanged: vm.chatTitle = text

            Binding {
                target: titleField
                property: "text"
                value: vm.chatTitle
            }
        }

        // ─── Провайдер ─────────────────────────
        Label { text: "Провайдер"; color: Colors.overBackground }
        ReusableComboBox {
            id: providerBox
            Layout.fillWidth: true

            itemsModel: App.providerSelector.providers
            selectedIndex: App.providerSelector.currentProviderIndex
            onItemSelected: function(id) { App.providerSelector.currentProvider = id }
        }

        // ─── Модель ───────────────────────────
        Label { text: "Модель"; color: Colors.overBackground }
        ReusableComboBox {
            id: modelBox
            Layout.fillWidth: true

            itemsModel: App.modelSelector.models
            selectedIndex: App.modelSelector.currentModelIndex
            onItemSelected: function(id) { App.modelSelector.currentModel = id }
        }

        // ─── Системный промпт ─────────────────
        Label { text: "System Prompt"; color: Colors.overBackground }
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 250
            TextArea {
                id: promptEdit
                wrapMode: TextEdit.Wrap
                text: vm.prompt
                color: Colors.overBackground
                placeholderText: "Введите системный промпт..."
                placeholderTextColor: Colors.overBackground

                onTextChanged: vm.prompt = text

                Binding {
                    target: promptEdit
                    property: "text"
                    value: vm.prompt
                }
            }
        }

        RowLayout {
            anchors.right: parent.right
            spacing: 8

            Button {
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: textItem.implicitWidth + 20

                Text {
                    id: textItem
                    text: "Отмена"
                    color: Colors.overBackground
                }

                onClicked: backRequested()
            }

            Button {
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: textItem2.implicitWidth + 20

                Text {
                    id: textItem2
                    text: "Сохранить"
                    color: Colors.overBackground
                }

                onClicked: vm.saveChat()
            }
        }
    }
}