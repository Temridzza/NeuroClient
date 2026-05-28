import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../theme"
import "../pages"
import "pages"

import ChatGPT 1.0
import NeuroClient 1.0



Item {
    id: root

    signal requestBack()

    property int currentIndex: 0

    // selected provider
    property string editingProviderId: ""

    // edit mode
    property bool editingProvider: false

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

                // ─────────────────────────────
                // BACK BUTTON
                // ─────────────────────────────
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

                        onClicked: {
                            root.requestBack()
                        }
                    }
                }

                // ─────────────────────────────
                // MENU
                // ─────────────────────────────
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

                        color:
                            hovered
                            ? Qt.lighter(
                                  Theme.panelColor,
                                  1.2
                                  )
                            : (
                                  index === root.currentIndex
                                  && !root.editingProvider
                                  ? Qt.darker(
                                        Theme.panelColor,
                                        1.2
                                        )
                                  : "transparent"
                                  )

                        Text {
                            anchors.centerIn: parent

                            text: modelData

                            color: Theme.textColor
                        }

                        MouseArea {
                            anchors.fill: parent

                            hoverEnabled: true

                            onEntered: {
                                parent.hovered = true
                            }

                            onExited: {
                                parent.hovered = false
                            }

                            onClicked: {
                                root.editingProvider = false
                                root.currentIndex = index
                            }
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

            // ─────────────────────────────
            // NORMAL SETTINGS PAGES
            // ─────────────────────────────
            StackLayout {
                anchors.fill: parent

                visible: !root.editingProvider

                currentIndex: root.currentIndex

                // GENERAL
                Item {
                    Text {
                        anchors.centerIn: parent

                        text: "General Settings"

                        color: Theme.textColor
                    }
                }

                // PROVIDERS
                ProviderPage {
                    onOpenProvider: function(providerId) {
                        console.log(
                                    "Open provider:",
                                    providerId
                                    )

                        root.editingProviderId =
                                providerId

                        root.editingProvider = true
                    }
                }

                // MODELS
                Item {
                    Text {
                        anchors.centerIn: parent

                        text: "Models Settings"

                        color: Theme.textColor
                    }
                }

                // API KEYS
                Item {
                    Text {
                        anchors.centerIn: parent

                        text: "API Keys"

                        color: Theme.textColor
                    }
                }

                // NETWORK
                Item {
                    Text {
                        anchors.centerIn: parent

                        text: "Network Settings"

                        color: Theme.textColor
                    }
                }

                // APPEARANCE
                Item {
                    Text {
                        anchors.centerIn: parent

                        text: "Appearance"

                        color: Theme.textColor
                    }
                }

                // SHORTCUTS
                Item {
                    Text {
                        anchors.centerIn: parent

                        text: "Shortcuts"

                        color: Theme.textColor
                    }
                }

                // ADVANCED
                Item {
                    Text {
                        anchors.centerIn: parent

                        text: "Advanced"

                        color: Theme.textColor
                    }
                }

                // ABOUT
                Item {
                    Text {
                        anchors.centerIn: parent

                        text: "About"

                        color: Theme.textColor
                    }
                }
            }

            // ─────────────────────────────
            // PROVIDER EDITOR
            // ─────────────────────────────
            Loader {
                anchors.fill: parent

                active: root.editingProvider

                visible: root.editingProvider

                sourceComponent: ProviderEditPage {
                    providerId:
                        root.editingProviderId

                    onBackRequested: {
                        root.editingProvider = false
                    }
                }
            }
        }
    }
}