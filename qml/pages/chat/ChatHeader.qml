import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import Theme 1.0
import ChatGPT 1.0
import NeuroClient

Rectangle {
    id: root

    property bool sidebarOpened: true
    property bool sidebarAnimating: false

    signal toggleSidebar()
    signal settings()
    onToggleSidebar: {
        sidebarOpened = !sidebarOpened
    }

    height: 44
    radius: 14
    color: Theme.panelColor

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 14
        anchors.rightMargin: 14
        spacing: 10

        ToolButton {
            text: "☰"
            onClicked: {
                root.toggleSidebar()
            }
        }

        Rectangle {
            width: 28
            height: 28
            radius: 14
            color: Theme.accentColor

            Text {
                anchors.centerIn: parent
                text: "N"
                color: "white"
                font.bold: true
            }
        }

        ComboBox {
            Layout.preferredWidth: 180
            model: App.providerSelector.providers
            textRole: "display_name"
            currentIndex: App.providerSelector.currentProviderIndex

            onActivated: function(index) {
                App.providerSelector.currentProvider = model[index].id
            }
        }

        ComboBox {
            Layout.preferredWidth: 180
            model: App.modelSelector.models
            textRole: "display_name"
            currentIndex: App.modelSelector.currentModelIndex

            onActivated: function(index) {
                App.modelSelector.currentModel = model[index].id
            }
        }

        Item { Layout.fillWidth: true }

        ToolButton {
            text: "⚙"
            onClicked: root.settings()
        }
    }
}