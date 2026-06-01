import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0
import ChatGPT 1.0
import NeuroClient 1.0

Item {
    id: root

    property string providerId: ""

    property var providerData: ({ })

    property string providerName: ""
    property string providerUrl: ""
    property string providerKey: ""

    signal backRequested()

    Component.onCompleted: {
        loadProvider()

        App.models.loadModelsByProvider(providerId)
    }

    function loadProvider() {
        providerData =
                App.providers.getProviderById(providerId)

        providerName =
                providerData.name || ""

        providerUrl =
                providerData.base_url || ""

        providerKey =
                providerData.api_key || ""
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24

        spacing: 18

        // HEADER
        RowLayout {
            Layout.fillWidth: true

            Rectangle {
                width: 42
                height: 42
                radius: 10

                color: Qt.lighter(
                           Theme.panelColor,
                           1.1
                       )

                Text {
                    anchors.centerIn: parent

                    text: "←"

                    color: Theme.textColor
                    font.pixelSize: 18
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        root.backRequested()
                    }
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

                    onClicked: {
                        App.providers.updateProvider(
                                    providerId,
                                    {
                                        "name": providerName,
                                        "base_url": providerUrl,
                                        "api_key": providerKey
                                    }
                                    )

                        root.backRequested()
                    }
                }
            }
        }

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

                        onTextChanged: {
                            root.providerName = text
                        }

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

                        onTextChanged: {
                            root.providerUrl = text
                        }

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

                        onTextChanged: {
                            root.providerKey = text
                        }

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

                // MODEL DISCOVERY
                Rectangle {
                    Layout.fillWidth: true

                    radius: 14

                    implicitHeight:
                        contentLayout.implicitHeight + 32

                    color: Qt.lighter(
                               Theme.panelColor,
                               1.03
                           )

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

                                color:
                                    providerUrl.length > 0
                                    ? "#1f3a28"
                                    : "#3a1f1f"

                                Text {
                                    anchors.centerIn: parent

                                    text:
                                        providerUrl.length > 0
                                        ? "Configured"
                                        : "Incomplete"

                                    color:
                                        providerUrl.length > 0
                                        ? "#7dffae"
                                        : "#ff9b9b"

                                    font.pixelSize: 12
                                }
                            }
                        }

                        Text {
                            text:
                                "Automatically fetch available models from provider API."

                            wrapMode: Text.WordWrap

                            color: Theme.textColor
                            opacity: 0.7

                            Layout.fillWidth: true
                        }

                        RowLayout {
                            spacing: 10

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
                                        console.log(
                                                    "fetch models for",
                                                    providerId
                                                    )
                                    }
                                }
                            }

                            Rectangle {
                                width: 120
                                height: 38
                                radius: 10

                                color: Qt.lighter(
                                           Theme.panelColor,
                                           1.15
                                       )

                                Text {
                                    anchors.centerIn: parent

                                    text: "+ Add Model"

                                    color: Theme.textColor
                                    font.pixelSize: 13
                                }

                                MouseArea {
                                    anchors.fill: parent

                                    onClicked: {
                                        App.models.createModel(
                                                    providerId,
                                                    "new-model",
                                                    "New Model"
                                                    )

                                        App.models.loadModelsByProvider(
                                                    providerId
                                                    )
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

                        Text {
                            text:
                                "Provider ID: " + providerId

                            color: Theme.textColor
                            opacity: 0.45
                            font.pixelSize: 11

                            Layout.fillWidth: true

                            elide: Text.ElideMiddle
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
                        model: root.App.models.models

                        delegate: Rectangle {
                            Layout.fillWidth: true

                            height: 52
                            radius: 10

                            color: Theme.panelColor

                            property var item: modelData

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12

                                CheckBox {
                                    checked: item.enabled

                                    onToggled: {
                                        App.models.updateModel(
                                                    item.id,
                                                    item.display_name,
                                                    item.api_key_override || "",
                                                    item.context_window || 0,
                                                    item.max_output_tokens || 0,
                                                    checked
                                                    )
                                    }
                                }

                                ColumnLayout {
                                    spacing: 2

                                    Text {
                                        text: item.display_name

                                        color: Theme.textColor
                                        font.pixelSize: 14
                                    }

                                    Text {
                                        text: item.model_key

                                        color: Theme.textColor
                                        opacity: 0.5
                                        font.pixelSize: 11
                                    }
                                }

                                Item {
                                    Layout.fillWidth: true
                                }

                                Rectangle {
                                    width: 26
                                    height: 26
                                    radius: 8

                                    color: "#402020"

                                    Text {
                                        anchors.centerIn: parent

                                        text: "×"

                                        color: "#ff8080"
                                        font.pixelSize: 16
                                    }

                                    MouseArea {
                                        anchors.fill: parent

                                        onClicked: {
                                            App.models.deleteModel(item.id)

                                            App.models.loadModelsByProvider(
                                                        providerId
                                                        )
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true

                        height:
                            App.models.models.length === 0
                            ? 80
                            : 0

                        visible:
                            App.models.models.length === 0

                        radius: 12

                        color: Qt.lighter(
                                   Theme.panelColor,
                                   1.02
                               )

                        Text {
                            anchors.centerIn: parent

                            text: "No models yet"

                            color: Theme.textColor
                            opacity: 0.5
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
                            text:
                                "Remove this provider permanently."

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
                                    App.providers.deleteProvider(
                                                providerId
                                                )

                                    root.backRequested()
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