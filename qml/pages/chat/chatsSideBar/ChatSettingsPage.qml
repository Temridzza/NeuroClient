// pages/chat/ChatSettingsPage.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import NeuroClient 1.0
import ChatGPT 1.0
import "../../../components"
import qs.modules.theme
import qs.modules.components

StyledRect {
    id: root

    variant: "pane"
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
        spacing: 12

        // ─── Заголовок ─────────────────────────
        StyledRect {
            id: settingsBg
            Layout.fillWidth: true
            height: 48
            radius: 12
            variant: "internalbg"

            Text {
                anchors.centerIn: parent
                text: "Настройки чата"
                font.pixelSize: 20
                font.bold: true
                color: settingsBg.item
            }
        }

        // ─── Название ───────────────────────────
        Text {
            text: "Название"
            color: root.item
        }

        StyledRect {
            id: titleBg
            Layout.fillWidth: true
            height: 42
            radius: 10
            variant: "internalbg"

            TextField {
                id: titleField
                anchors.fill: parent
                anchors.margins: 8

                text: vm.chatTitle
                color: titleBg.item
                background: null

                onTextChanged: vm.chatTitle = text

                Binding {
                    target: titleField
                    property: "text"
                    value: vm.chatTitle
                }
            }
        }

        // ─── Провайдер ─────────────────────────
        Text {
            text: "Провайдер"
            color: root.item
        }

        StyledRect {
            Layout.fillWidth: true
            height: 42
            radius: 10
            variant: "internalbg"

            ReusableComboBox {
                id: providerBox
                anchors.fill: parent

                itemsModel: App.providerSelector.providers
                selectedIndex: App.providerSelector.currentProviderIndex
                onItemSelected: function(id) {
                    App.providerSelector.currentProvider = id
                }
            }
        }

        // ─── Модель ───────────────────────────
        Text {
            text: "Модель"
            color: root.item
        }

        StyledRect {
            Layout.fillWidth: true
            height: 42
            radius: 10
            variant: "internalbg"

            ReusableComboBox {
                id: modelBox
                anchors.fill: parent

                itemsModel: App.modelSelector.models
                selectedIndex: App.modelSelector.currentModelIndex
                onItemSelected: function(id) {
                    App.modelSelector.currentModel = id
                }
            }
        }

        // ─── System prompt ─────────────────────
        Text {
            text: "System Prompt"
            color: root.item
        }

        StyledRect {
            id: promptBg
            Layout.fillWidth: true
            Layout.fillHeight: true
            // minimumHeight: 250
            radius: 12
            variant: "internalbg"

            ScrollView {
                anchors.fill: parent
                anchors.margins: 8

                TextArea {
                    id: promptEdit
                    wrapMode: TextEdit.Wrap

                    text: vm.prompt
                    color: promptBg.item
                    background: null

                    placeholderText: "Введите системный промпт..."
                    placeholderTextColor: promptBg.item

                    onTextChanged: vm.prompt = text

                    Binding {
                        target: promptEdit
                        property: "text"
                        value: vm.prompt
                    }
                }
            }
        }

        // ─── Кнопки ───────────────────────────
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Item { Layout.fillWidth: true }

            StyledRect {
                id: cancelBg
                width: 110
                height: 36
                radius: 10
                variant: "internalbg"

                Text {
                    anchors.centerIn: parent
                    text: "Отмена"
                    color: cancelBg.item
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: backRequested()
                    onEntered: cancelBg.variant = "focus"
                    onExited: cancelBg.variant = "internalbg"
                }
            }

            StyledRect {
                id: saveBg
                width: 120
                height: 36
                radius: 10
                variant: "primary"

                Text {
                    anchors.centerIn: parent
                    text: "Сохранить"
                    color: saveBg.item
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: vm.saveChat()
                    onEntered: saveBg.variant = "focus"
                    onExited: saveBg.variant = "primary"
                }
            }
        }
    }
}