import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import ChatGPT 1.0
import NeuroClient

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

        ComboBox {
            id: providerBox

            Layout.preferredWidth: 180
            Layout.preferredHeight: 36

            model: App.providerSelector.providers
            textRole: "display_name"
            currentIndex: App.providerSelector.currentProviderIndex

            background: Rectangle {
                radius: 10
                color: Qt.alpha(Colors.primary, 0.08)

                border.width: 1
                border.color: Qt.alpha(Colors.primary, 0.18)
            }

            contentItem: Text {
                text: providerBox.displayText
                color: Colors.overBackground
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter

                leftPadding: 12
                rightPadding: 24

                elide: Text.ElideRight
            }

            indicator: Text {
                text: "⌄"
                color: Colors.primary

                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            popup: Popup {
                y: providerBox.height + 6
                width: providerBox.width

                background: Rectangle {
                    radius: 12
                    color: Colors.surfaceContainer

                    border.width: 1
                    border.color: Qt.alpha(Colors.primary, 0.15)
                }

                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight

                    model: providerBox.popup.visible
                           ? providerBox.delegateModel
                           : null
                }
            }

            delegate: ItemDelegate {
                width: providerBox.width
                height: 36

                background: Rectangle {
                    color: highlighted
                           ? Qt.alpha(Colors.primary, 0.12)
                           : "transparent"
                }

                contentItem: Text {
                    text: modelData.display_name
                    color: highlighted ? Colors.primary : Colors.overBackground
                    verticalAlignment: Text.AlignVCenter
                }
            }

            onActivated: function(index) {
                App.providerSelector.currentProvider = model[index].id
            }
        }

        ComboBox {
            id: modelBox

            Layout.preferredWidth: 220
            Layout.preferredHeight: 36

            model: App.modelSelector.models
            textRole: "display_name"
            currentIndex: App.modelSelector.currentModelIndex

            background: Rectangle {
                radius: 10
                color: Qt.alpha(Colors.primary, 0.08)

                border.width: 1
                border.color: Qt.alpha(Colors.primary, 0.18)
            }

            contentItem: Text {
                text: modelBox.displayText
                color: Colors.overBackground
                font.pixelSize: 14

                verticalAlignment: Text.AlignVCenter

                leftPadding: 12
                rightPadding: 24

                elide: Text.ElideRight
            }

            indicator: Text {
                text: "⌄"
                color: Colors.primary

                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            popup: Popup {
                y: modelBox.height + 6
                width: modelBox.width

                background: Rectangle {
                    radius: 12
                    color: Colors.surfaceContainer

                    border.width: 1
                    border.color: Qt.alpha(Colors.primary, 0.15)
                }

                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight

                    model: modelBox.popup.visible
                           ? modelBox.delegateModel
                           : null
                }
            }

            delegate: ItemDelegate {
                width: modelBox.width
                height: 36

                background: Rectangle {
                    color: highlighted
                           ? Qt.alpha(Colors.primary, 0.12)
                           : "transparent"
                }

                contentItem: Text {
                    text: modelData.display_name
                    color: highlighted ? Colors.primary : Colors.overBackground
                    verticalAlignment: Text.AlignVCenter
                }
            }

            onActivated: function(index) {
                App.modelSelector.currentModel = model[index].id
            }
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