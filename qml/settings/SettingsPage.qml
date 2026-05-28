import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../theme"
import "../pages"
import ChatGPT 1.0

import "pages"

Item {
    id: root

    signal requestBack()

    property int currentIndex: 0
    property string editingProvider: ""

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // ─────────────────────────────
        // LEFT SIDEBAR
        // ─────────────────────────────
        Rectangle {
            Layout.preferredWidth: 260
            Layout.fillHeight: true
            color: Theme.panelColor

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 6

                // BACK BUTTON
                Rectangle {
                    width: parent.width
                    height: 42
                    radius: 10
                    color: "transparent"

                    Text {
                        anchors.centerIn: parent
                        text: "← Back"
                        color: Theme.textColor
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.requestBack()
                    }
                }

                // MENU
                ListView {
                    width: parent.width
                    height: parent.height - 50

                    model: [
                        "General",
                        "Providers",
                        "Models",
                        "API Keys",
                        "Network",
                        "Appearance",
                        "Shortcuts",
                        "Advanced",
                        "About"
                    ]

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 42
                        radius: 10

                        property bool hovered: false

                        color: hovered
                            ? Qt.lighter(Theme.panelColor, 1.2)
                            : (index === root.currentIndex
                                ? Qt.darker(Theme.panelColor, 1.2)
                                : "transparent")

                        Text {
                            anchors.centerIn: parent
                            text: modelData
                            color: Theme.textColor
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: parent.hovered = true
                            onExited: parent.hovered = false

                            onClicked: root.currentIndex = index
                        }
                    }
                }
            }
        }

        // ─────────────────────────────
        // CONTENT AREA
        // ─────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Theme.chatBgColor

            StackLayout {
                anchors.fill: parent
                currentIndex: root.currentIndex

                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "General Settings"
                        color: Theme.textColor
                    }
                }

                ProviderPage {
                    onAddProvider: {
                        console.log("Add provider clicked")
                        root.editingProvider = "New Provider"
                        root.currentIndex = -1
                    }

                    onOpenProvider: function(name) {
                        console.log("Open provider:", name)
                        root.editingProvider = name
                        root.currentIndex = -1
                    }
                }

                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "Models Settings"
                        color: Theme.textColor
                    }
                }

                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "API Keys"
                        color: Theme.textColor
                    }
                }

                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "Network Settings"
                        color: Theme.textColor
                    }
                }

                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "Appearance"
                        color: Theme.textColor
                    }
                }

                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "Shortcuts"
                        color: Theme.textColor
                    }
                }

                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "Advanced"
                        color: Theme.textColor
                    }
                }

                Item {
                    Text {
                        anchors.centerIn: parent
                        text: "About"
                        color: Theme.textColor
                    }
                }
            }

            Loader {
                anchors.fill: parent

                active: root.currentIndex === -1

                sourceComponent: ProviderEditPage {
                    providerName: root.editingProvider

                    onBackRequested: {
                        root.currentIndex = 1
                    }

                    onSaveRequested: {
                        console.log("save provider")
                        root.currentIndex = 1
                    }
                }
            }
        }
    }
}