import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../../theme"
import ChatGPT 1.0

Item {
    id: root

    property string providerName: "OpenAI"
    property string providerUrl: "https://api.openai.com"
    property string providerKey: ""
    property var providerModels: [
        "gpt-4o",
        "gpt-4.1",
        "gpt-4.1-mini"
    ]

    signal backRequested()
    signal saveRequested()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 18

        // ─────────────────────────────
        // HEADER
        // ─────────────────────────────
        RowLayout {
            Layout.fillWidth: true

            Rectangle {
                width: 42
                height: 42
                radius: 10
                color: Qt.lighter(Theme.panelColor, 1.1)

                Text {
                    anchors.centerIn: parent
                    text: "←"
                    color: Theme.textColor
                    font.pixelSize: 18
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.backRequested()
                }
            }

            Text {
                text: "Edit Provider"
                font.pixelSize: 24
                color: Theme.textColor
            }

            Item {
                Layout.fillWidth: true
            }

            Rectangle {
                width: 100
                height: 40
                radius: 10
                color: "#3b82f6"

                Text {
                    anchors.centerIn: parent
                    text: "Save"
                    color: "white"
                    font.pixelSize: 14
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.saveRequested()
                }
            }
        }

        // ─────────────────────────────
        // FORM
        // ─────────────────────────────
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true

            contentWidth: width
            contentHeight: contentColumn.height

            clip: true

            ColumnLayout {
                id: contentColumn

                width: parent.width
                spacing: 18

                // NAME
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Provider Name"
                        color: Theme.textColor
                        opacity: 0.8
                    }

                    TextField {
                        Layout.fillWidth: true
                        text: root.providerName

                        background: Rectangle {
                            radius: 12
                            color: Theme.panelColor
                            border.color: Theme.borderColor
                        }

                        color: Theme.textColor
                        padding: 12
                    }
                }

                // URL
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Base URL"
                        color: Theme.textColor
                        opacity: 0.8
                    }

                    TextField {
                        Layout.fillWidth: true
                        text: root.providerUrl

                        background: Rectangle {
                            radius: 12
                            color: Theme.panelColor
                            border.color: Theme.borderColor
                        }

                        color: Theme.textColor
                        padding: 12
                    }
                }

                // API KEY
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "API Key"
                        color: Theme.textColor
                        opacity: 0.8
                    }

                    TextField {
                        Layout.fillWidth: true
                        text: root.providerKey
                        echoMode: TextInput.Password

                        background: Rectangle {
                            radius: 12
                            color: Theme.panelColor
                            border.color: Theme.borderColor
                        }

                        color: Theme.textColor
                        padding: 12
                    }
                }

                // FETCH MODELS SECTION
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Rectangle {
                        Layout.fillWidth: true
                        radius: 14
                        implicitHeight: contentLayout.implicitHeight + 32

                        color: Qt.lighter(Theme.panelColor, 1.03)

                        ColumnLayout {
                            id: contentLayout
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 14

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Model Discovery"
                                    font.pixelSize: 16
                                    color: Theme.textColor
                                }

                                Item {
                                    Layout.fillWidth: true
                                }

                                Rectangle {
                                    width: 90
                                    height: 30
                                    radius: 8
                                    color: "#1f3a28"

                                    Text {
                                        anchors.centerIn: parent
                                        text: "Connected"
                                        color: "#7dffae"
                                        font.pixelSize: 12
                                    }
                                }
                            }

                            Text {
                                text: "Automatically fetch available models from provider API or add them manually."
                                wrapMode: Text.WordWrap
                                color: Theme.textColor
                                opacity: 0.7

                                Layout.fillWidth: true
                            }

                            RowLayout {
                                spacing: 10

                                // FETCH BUTTON
                                Rectangle {
                                    width: 140
                                    height: 38
                                    radius: 10
                                    color: "#2563eb"

                                    Text {
                                        anchors.centerIn: parent
                                        text: "Fetch Models"
                                        color: "white"
                                        font.pixelSize: 13
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            console.log("fetch models")
                                        }
                                    }
                                }

                                // ADD MODEL BUTTON
                                Rectangle {
                                    width: 120
                                    height: 38
                                    radius: 10
                                    color: Qt.lighter(Theme.panelColor, 1.15)

                                    Text {
                                        anchors.centerIn: parent
                                        text: "+ Add Model"
                                        color: Theme.textColor
                                        font.pixelSize: 13
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            console.log("add model")
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 1
                                color: Theme.borderColor
                                opacity: 0.3
                            }

                            RowLayout {
                                spacing: 8

                                Rectangle {
                                    width: 8
                                    height: 8
                                    radius: 999
                                    color: "#4ade80"
                                }

                                Text {
                                    text: "Last sync: just now"
                                    color: Theme.textColor
                                    opacity: 0.55
                                    font.pixelSize: 12
                                }
                            }
                        }
                    }
                }

                // MODELS
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Text {
                        text: "Models"
                        color: Theme.textColor
                        opacity: 0.8
                    }

                    Repeater {
                        model: root.providerModels

                        delegate: Rectangle {
                            Layout.fillWidth: true
                            height: 44
                            radius: 10

                            color: Theme.panelColor

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12

                                CheckBox {
                                    checked: true
                                }

                                Text {
                                    text: modelData
                                    color: Theme.textColor
                                }

                                Item {
                                    Layout.fillWidth: true
                                }

                                Rectangle {
                                    width: 70
                                    height: 28
                                    radius: 8
                                    color: Qt.lighter(Theme.panelColor, 1.15)

                                    Text {
                                        anchors.centerIn: parent
                                        text: "Key"
                                        color: Theme.textColor
                                        font.pixelSize: 12
                                    }
                                }
                            }
                        }
                    }
                }

                // DANGER ZONE
                Rectangle {
                    Layout.fillWidth: true
                    radius: 14
                    color: "#2a1616"
                    border.color: "#5a2a2a"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10

                        Text {
                            text: "Danger Zone"
                            color: "#ff8080"
                            font.pixelSize: 16
                        }

                        Text {
                            text: "Remove this provider permanently."
                            color: Theme.textColor
                            opacity: 0.7
                        }

                        Rectangle {
                            width: 120
                            height: 36
                            radius: 10
                            color: "#c0392b"

                            Text {
                                anchors.centerIn: parent
                                text: "Delete"
                                color: "white"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.log("delete provider")
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    height: 40
                }
            }
        }
    }
}
