import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0
import ChatGPT 1.0
import NeuroClient 1.0

Item {
    id: root

    property var modelData: ({})

    property string displayName: ""
    property string modelKey: ""
    property string apiKeyOverride: ""
    property int contextWindow: 0
    property int maxOutputTokens: 0
    property bool enabled: true

    signal backRequested()

    Component.onCompleted: {
        displayName = modelData.display_name || ""
        modelKey = modelData.modelKey || ""
        apiKeyOverride = modelData.apiKeyOverride || ""
        contextWindow = modelData.contextWindow || 0
        maxOutputTokens = modelData.maxOutputTokens || 0
        enabled = modelData.enabled || false
    }

    ScrollView {
        anchors.fill: parent

        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width

            spacing: 14

            // Header
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

                        onClicked: root.backRequested()
                    }
                }

                Text {
                    text: "Edit Model"

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
                            App.models.updateModel(
                                modelData.id,
                                displayName,
                                apiKeyOverride,
                                contextWindow,
                                maxOutputTokens,
                                enabled
                            )

                            root.backRequested()
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1

                color: Theme.borderColor
                opacity: 0.4
            }

            // Model info
            Rectangle {
                Layout.fillWidth: true

                radius: 14

                implicitHeight:
                    infoColumn.implicitHeight + 32

                color: Qt.lighter(
                           Theme.panelColor,
                           1.03
                       )

                ColumnLayout {
                    id: infoColumn

                    anchors.fill: parent
                    anchors.margins: 16

                    spacing: 8

                    Text {
                        text: "Model Information"

                        font.pixelSize: 16
                        color: Theme.textColor
                    }

                    Text {
                        text: "Provider: " + modelData.providerId

                        color: Theme.textColor
                        opacity: 0.7
                    }

                    Rectangle {
                        Layout.fillWidth: true

                        height: 1

                        color: Theme.borderColor
                        opacity: 0.3
                    }

                    Text {
                        Layout.fillWidth: true

                        text: "ID: " + modelData.id

                        color: Theme.textColor
                        opacity: 0.45
                        font.pixelSize: 11

                        elide: Text.ElideMiddle
                    }
                }
            }

            // Display name
            ColumnLayout {
                Layout.fillWidth: true

                Text {
                    text: "Display Name"
                    color: Theme.textColor
                }

                TextField {
                    Layout.fillWidth: true

                    text: root.displayName

                    onTextChanged: root.displayName = text

                    color: Theme.textColor

                    background: Rectangle {
                        radius: 12

                        color: Theme.panelColor
                        border.color: Theme.borderColor
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true

                spacing: 4

                Text {
                    text: "Model Key"

                    color: Theme.textColor
                    opacity: 0.8
                }

                TextField {
                    Layout.fillWidth: true

                    text: root.modelKey

                    onTextChanged: root.modelKey = text

                    color: Theme.textColor

                    background: Rectangle {
                        radius: 12

                        color: Theme.panelColor
                        border.color: Theme.borderColor
                    }
                }
            }

            // API key override
            ColumnLayout {
                Layout.fillWidth: true

                Text {
                    text: "API Key Override"
                    color: Theme.textColor
                }

                TextField {
                    Layout.fillWidth: true

                    text: root.apiKeyOverride

                    echoMode: TextInput.Password

                    onTextChanged: root.apiKeyOverride = text

                    color: Theme.textColor

                    background: Rectangle {
                        radius: 12

                        color: Theme.panelColor
                        border.color: Theme.borderColor
                    }
                }
            }

            // Context window
            ColumnLayout {
                Layout.fillWidth: true

                spacing: 6

                Text {
                    text: "Context Window"

                    color: Theme.textColor
                    opacity: 0.8
                }

                TextField {
                    Layout.fillWidth: true

                    text: root.contextWindow.toString()

                    validator: IntValidator {
                        bottom: 0
                    }

                    onTextChanged: {
                        root.contextWindow =
                                parseInt(text || "0")
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

            // Max output tokens
            Rectangle {
                Layout.fillWidth: true

                radius: 14

                color: Qt.lighter(
                           Theme.panelColor,
                           1.03
                       )

                implicitHeight: 64

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16

                    Text {
                        text: "Enabled"

                        color: Theme.textColor
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    CheckBox {
                        checked: root.enabled

                        onToggled: {
                            root.enabled = checked
                        }
                    }
                }
            }

            // Danger zone
            Rectangle {
                Layout.fillWidth: true

                radius: 14

                color: "#2a1616"
                border.color: "#5a2a2a"

                implicitHeight: dangerColumn.implicitHeight + 32

                ColumnLayout {
                    id: dangerColumn

                    anchors.fill: parent
                    anchors.margins: 16

                    Text {
                        text: "Danger Zone"

                        font.pixelSize: 16
                        color: "#ff8080"
                    }

                    Text {
                        text: "Delete this model permanently."

                        color: Theme.textColor
                        opacity: 0.7
                    }

                    Button {
                        text: "Delete"

                        onClicked: {
                            App.models.deleteModel(modelData.id)
                            root.backRequested()
                        }
                    }
                }
            }

            Item {
                Layout.preferredHeight: 20
            }
        }
    }
}