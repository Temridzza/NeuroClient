import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import ChatGPT 1.0
import NeuroClient

import "../../components"

Rectangle {
    id: root

    property bool sidebarOpened: true
    property bool sidebarAnimating: false

    signal toggleSidebar()
    signal settings()

    onToggleSidebar: sidebarOpened = !sidebarOpened

    height: 52
    radius: 16

    color: Colors.surfaceContainer
    border.width: 1
    border.color: Colors.primary

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 10

        ToolButton {
            Layout.preferredWidth: 36
            Layout.preferredHeight: 36

            background: Rectangle {
                radius: 18
                color: parent.down
                       ? Qt.alpha(Colors.primary, 0.25)
                       : Qt.alpha(Colors.primary, 0.12)

                border.width: 1
                border.color: Qt.alpha(Colors.primary, 0.25)

                Behavior on color {
                    ColorAnimation { duration: 120 }
                }
            }

            contentItem: Text {
                text: "☰"
                color: Colors.primary
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: root.toggleSidebar()
        }

        Rectangle {
            Layout.preferredWidth: 36
            Layout.preferredHeight: 36

            radius: 18
            color: Qt.alpha(Colors.primary, 0.12)

            border.width: 1
            border.color: Qt.alpha(Colors.primary, 0.25)

            Text {
                anchors.centerIn: parent
                text: "N"

                color: Colors.primary
                font.bold: true
                font.pixelSize: 15
            }
        }
        ReusableComboBox {
            id: providerBox
            itemsModel: App.providerSelector.providers
            selectedIndex: App.providerSelector.currentProviderIndex
            onItemSelected: function(id) { App.providerSelector.currentProvider = id }
        }

        ReusableComboBox {
            id: modelBox
            itemsModel: App.modelSelector.models
            selectedIndex: App.modelSelector.currentModelIndex
            onItemSelected: function(id) { App.modelSelector.currentModel = id }
        }

        Item {
            Layout.fillWidth: true
        }

        ToolButton {
            Layout.preferredWidth: 36
            Layout.preferredHeight: 36

            background: Rectangle {
                radius: 18
                color: Qt.alpha(Colors.primary, 0.12)

                border.width: 1
                border.color: Qt.alpha(Colors.primary, 0.25)
            }

            contentItem: Text {
                text: "⚙"
                color: Colors.primary
                font.pixelSize: 16

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: root.settings()
        }
    }
}