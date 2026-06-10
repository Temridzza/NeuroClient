import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0
import ChatGPT 1.0
import NeuroClient 1.0

Item {
    id: root

    signal openModel(var modelData)

    Component.onCompleted: {
        App.models.loadModels()
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
                text: "Models"
                font.pixelSize: 22
                color: Theme.textColor
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.borderColor
            opacity: 0.4
        }

        // ─────────────────────────────
        // MODELS LIST
        // ─────────────────────────────
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            spacing: 10

            model: App.models.models

            delegate: Rectangle {
                width: ListView.view.width
                height: 130
                radius: 14

                property bool hovered: false

                color: Qt.lighter(
                           Theme.panelColor,
                           hovered ? 1.08 : 1.05
                       )

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

                    onEntered: parent.hovered = true
                    onExited: parent.hovered = false

                    onClicked: {
                        root.openModel(modelData)
                    }
                }

                Column {
                    anchors.fill: parent
                    anchors.margins: 14

                    spacing: 4

                    Row {
                        width: parent.width

                        Text {
                            text: modelData.display_name

                            font.pixelSize: 18
                            color: Theme.textColor
                        }

                        Item {
                            width: 8
                        }

                        Text {
                            text: modelData.enabled
                                  ? "● enabled"
                                  : "● disabled"

                            color: modelData.enabled
                                   ? "#6fcf97"
                                   : "#eb5757"

                            font.pixelSize: 12
                        }
                    }

                    Text {
                        text: "Model Key: "
                              + modelData.modelKey

                        color: Theme.textColor
                        opacity: 0.8

                        width: parent.width
                        elide: Text.ElideRight
                    }

                    // Text {
                    //     text: "Context Window: "
                    //           + modelData.contextWindow

                    //     color: Theme.textColor
                    //     opacity: 0.7

                    //     font.pixelSize: 13
                    // }

                    // Text {
                    //     text: "Max Output Tokens: "
                    //           + modelData.maxOutputTokens

                    //     color: Theme.textColor
                    //     opacity: 0.7

                    //     font.pixelSize: 13
                    // }

                    Text {
                        text: "Provider: "
                              + modelData.providerId

                        color: Theme.textColor
                        opacity: 0.5

                        font.pixelSize: 12

                        width: parent.width
                        elide: Text.ElideMiddle
                    }

                    // Text {
                    //     text: modelData.id

                    //     color: Theme.textColor
                    //     opacity: 0.35

                    //     font.pixelSize: 11

                    //     width: parent.width
                    //     elide: Text.ElideMiddle
                    // }
                }
            }

            // EMPTY STATE
            Rectangle {
                anchors.centerIn: parent

                width: 320
                height: 140
                radius: 18

                visible: App.models.models.length === 0

                color: Qt.lighter(
                           Theme.panelColor,
                           1.03
                       )

                Column {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter

                        text: "No Models"

                        font.pixelSize: 20
                        color: Theme.textColor
                    }

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter

                        text: "Create your first model"

                        color: Theme.textColor
                        opacity: 0.6
                    }
                }
            }
        }
    }
}