import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../../theme"
import ChatGPT 1.0
import NeuroClient 1.0

Item {
    id: root

    signal openProvider(string providerId)

    Component.onCompleted: {
        App.providers.loadProviders()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 14

        // ─────────────────────────────
        // HEADER
        // ─────────────────────────────
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "Providers"
                font.pixelSize: 22
                color: Theme.textColor
            }

            Item {
                Layout.fillWidth: true
            }

            Rectangle {
                width: 140
                height: 36
                radius: 10

                color: Qt.lighter(Theme.panelColor, 1.1)

                Text {
                    anchors.centerIn: parent
                    text: "+ Add Provider"
                    color: Theme.textColor
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        const providerId =
                                App.providers.createProvider(
                                    "New Provider",
                                    "",
                                    ""
                                )

                        if (providerId.length > 0) {
                            root.openProvider(providerId)
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.borderColor
            opacity: 0.4
        }

        // ─────────────────────────────
        // PROVIDERS LIST
        // ─────────────────────────────
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 10
            clip: true

            model: App.providers

            delegate: Rectangle {
                width: ListView.view.width
                height: 110
                radius: 14

                color: Qt.lighter(
                           Theme.panelColor,
                           hovered ? 1.08 : 1.05
                       )

                property bool hovered: false

                scale: hovered ? 1.01 : 1.0

                Behavior on scale {
                    NumberAnimation {
                        duration: 120
                    }
                }

                Behavior on color {
                    ColorAnimation {
                        duration: 120
                    }
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
                        root.openProvider(id)
                    }
                }

                Column {
                    anchors.fill: parent
                    anchors.margins: 14

                    spacing: 4

                    // ─────────────────────────────
                    // NAME
                    // ─────────────────────────────
                    Row {
                        width: parent.width

                        Text {
                            text: name

                            font.pixelSize: 18
                            color: Theme.textColor
                        }

                        Item {
                            width: 10
                        }

                        Text {
                            text: "›"

                            font.pixelSize: 18
                            color: Theme.textColor
                            opacity: 0.6
                        }
                    }

                    // ─────────────────────────────
                    // URL
                    // ─────────────────────────────
                    Text {
                        text: "url: " + (
                                  baseUrl.length > 0
                                  ? baseUrl
                                  : "(empty)"
                              )

                        color: Theme.textColor
                        opacity: 0.7
                        font.pixelSize: 13

                        elide: Text.ElideRight
                        width: parent.width
                    }

                    // ─────────────────────────────
                    // API KEY
                    // ─────────────────────────────
                    Text {
                        text: "key: " + (
                                  apiKey.length > 0
                                  ? "********"
                                  : "(none)"
                              )

                        color: Theme.textColor
                        opacity: 0.7
                        font.pixelSize: 13
                    }

                    // ─────────────────────────────
                    // ID
                    // ─────────────────────────────
                    Text {
                        text: id

                        color: Theme.textColor
                        opacity: 0.35
                        font.pixelSize: 11

                        elide: Text.ElideMiddle
                        width: parent.width
                    }
                }
            }

            // EMPTY STATE
            Rectangle {
                anchors.centerIn: parent

                width: 320
                height: 140
                radius: 18

                visible: App.providers.rowCount() === 0

                color: Qt.lighter(Theme.panelColor, 1.03)

                Column {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter

                        text: "No Providers"

                        font.pixelSize: 20
                        color: Theme.textColor
                    }

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter

                        text: "Add your first AI provider"

                        color: Theme.textColor
                        opacity: 0.6
                    }
                }
            }
        }
    }
}