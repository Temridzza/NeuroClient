import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../../theme"
import ChatGPT 1.0

Item {
    id: root

    signal addProvider()
    signal openProvider(string name)

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

            Item { Layout.fillWidth: true }

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
                    onClicked: root.addProvider()
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

            model: [
                {
                    name: "OpenAI",
                    url: "https://api.openai.com",
                    models: "gpt-4o, gpt-4.1",
                    key: "************"
                },
                {
                    name: "Ollama",
                    url: "http://localhost:11434",
                    models: "llama3, mistral",
                    key: "(none)"
                }
            ]

            delegate: Rectangle {
                width: ListView.view.width
                height: 110
                radius: 14

                color: Qt.lighter(Theme.panelColor, 1.05)

                property bool hovered: false

                scale: hovered ? 1.01 : 1.0

                Behavior on scale {
                    NumberAnimation { duration: 120 }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: parent.hovered = true
                    onExited: parent.hovered = false

                    onClicked: root.openProvider(modelData.name)
                }

                Column {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 4

                    // NAME + ARROW
                    Row {
                        width: parent.width

                        Text {
                            text: modelData.name
                            font.pixelSize: 18
                            color: Theme.textColor
                        }

                        Item { width: 10 }

                        Text {
                            text: "›"
                            font.pixelSize: 18
                            color: Theme.textColor
                            opacity: 0.6
                        }
                    }

                    Text {
                        text: "url: " + modelData.url
                        color: Theme.textColor
                        opacity: 0.7
                        font.pixelSize: 13
                    }

                    Text {
                        text: "models: " + modelData.models
                        color: Theme.textColor
                        opacity: 0.7
                        font.pixelSize: 13
                    }

                    Text {
                        text: "key: " + modelData.key
                        color: Theme.textColor
                        opacity: 0.7
                        font.pixelSize: 13
                    }
                }
            }
        }
    }
}
