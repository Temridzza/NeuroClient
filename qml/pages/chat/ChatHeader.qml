import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components
import ChatGPT 1.0
import NeuroClient

import "../../components"

StyledRect {
    id: root

    property bool sidebarOpened: true
    property bool sidebarAnimating: false

    signal toggleSidebar()
    signal settings()

    onToggleSidebar: sidebarOpened = !sidebarOpened

    height: 52
    radius: 16
    variant: "pane"

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 10

        StyledToolButton {
            buttonText: "☰"
            onClicked: root.toggleSidebar()
        }

        StyledToolButton {
            buttonText: "N"
        }

        // Распределяем ширину комбобоксов
        ReusableComboBox {
            id: providerBox
            Layout.preferredWidth: 180
            Layout.minimumWidth: 120
            Layout.maximumWidth: 250

            itemsModel: App.providerSelector.providers
            selectedIndex: App.providerSelector.currentProviderIndex
            onItemSelected: function(id) {
                App.providerSelector.currentProvider = id
            }
        }

        ReusableComboBox {
            id: modelBox
            Layout.preferredWidth: 180
            Layout.minimumWidth: 120
            Layout.maximumWidth: 250

            itemsModel: App.modelSelector.models
            selectedIndex: App.modelSelector.currentModelIndex
            onItemSelected: function(id) {
                App.modelSelector.currentModel = id
            }
        }

        // Этот Item "растягивает" оставшееся пространство
        Item {
            Layout.fillWidth: true
        }

        StyledToolButton {
            buttonText: "⚙"
            onClicked: root.settings()
        }
    }
}