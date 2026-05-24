import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "components"
import NeuroClient

ApplicationWindow {
    id: root

    width: 1280
    height: 820
    visible: true

    color: "transparent"

    title: "NeuroClient"

    property bool sidebarOpened: true

    property color bgColor: "#0f1117"
    property color panelColor: "#171923"
    property color borderColor: "#2b2f3a"
    property color accentColor: "#7c5cff"
    property color textColor: "#ffffff"
    property color secondaryText: "#8b92a8"

    ModelSelectorViewModel {
        id: modelVM
    }

    Rectangle {
        anchors.fill: parent
        radius: 24

        color: bgColor
        border.color: borderColor
        border.width: 1

        layer.enabled: true

        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.8
            shadowOpacity: 0.4
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 14

            spacing: 14

            // SIDEBAR
            ChatsSideBar {
                id: sideBar

                Layout.fillHeight: true
                Layout.preferredWidth: sidebarOpened ? 320 : 0

                clip: true
                visible: width > 0

                Behavior on Layout.preferredWidth {
                    NumberAnimation {
                        duration: 180
                        easing.type: Easing.OutCubic
                    }
                }
            }

            // MAIN CHAT AREA
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                radius: 24
                color: "#131720"

                border.color: borderColor

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 14

                    spacing: 12

                    // HEADER
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 44

                        radius: 14
                        color: panelColor

                        RowLayout {
                            anchors.fill: parent

                            anchors.leftMargin: 14
                            anchors.rightMargin: 14

                            spacing: 10

                            ToolButton {
                                text: "☰"

                                onClicked: {
                                    sidebarOpened = !sidebarOpened
                                }
                            }

                            Rectangle {
                                width: 28
                                height: 28
                                radius: 14

                                color: accentColor

                                Text {
                                    anchors.centerIn: parent

                                    text: "N"
                                    color: "white"

                                    font.bold: true
                                }
                            }

                            ComboBox {
                                id: modelBox

                                Layout.preferredWidth: 180

                                model: modelVM.models

                                currentIndex: model.indexOf(modelVM.currentModel)

                                onCurrentTextChanged: {
                                    modelVM.currentModel = currentText
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            ToolButton {
                                text: "🔍"
                            }

                            ToolButton {
                                text: "⚙"
                            }
                        }
                    }

                    // CHAT AREA
                    ListView {
                        id: chatView

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        spacing: 12
                        clip: true

                        model: ListModel {
                            ListElement {
                                role: "user"
                                message: "Как работает квантовая запутанность?"
                            }

                            ListElement {
                                role: "assistant"
                                message: "Квантовая запутанность — это явление, при котором частицы оказываются связанными независимо от расстояния."
                            }
                        }

                        delegate: Item {
                            width: chatView.width
                            height: bubble.height + 8

                            Rectangle {
                                id: bubble

                                width: Math.min(
                                           parent.width * 0.82,
                                           textItem.implicitWidth + 32
                                       )

                                height: textItem.implicitHeight + 28

                                radius: 18

                                anchors {
                                    right: role === "user"
                                           ? parent.right
                                           : undefined

                                    left: role === "assistant"
                                          ? parent.left
                                          : undefined
                                }

                                color: role === "user"
                                       ? accentColor
                                       : panelColor

                                border.color: borderColor

                                Text {
                                    id: textItem

                                    anchors.fill: parent
                                    anchors.margins: 14

                                    text: message

                                    color: textColor

                                    wrapMode: Text.Wrap
                                    font.pixelSize: 15

                                    lineHeight: 1.25
                                }
                            }
                        }

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                        }
                    }

                    // INPUT PANEL
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 110

                        radius: 20

                        color: panelColor
                        border.color: borderColor

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12

                            spacing: 8

                            TextArea {
                                id: inputField

                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                placeholderText: "Напишите сообщение..."

                                color: textColor

                                wrapMode: TextEdit.Wrap

                                background: Rectangle {
                                    color: "transparent"
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                spacing: 10

                                ToolButton {
                                    text: "📎"
                                }

                                ToolButton {
                                    text: "🌐"
                                }

                                ToolButton {
                                    text: "🧠"
                                }

                                Item {
                                    Layout.fillWidth: true
                                }

                                Rectangle {
                                    width: 42
                                    height: 42
                                    radius: 21

                                    color: accentColor

                                    MouseArea {
                                        anchors.fill: parent

                                        cursorShape: Qt.PointingHandCursor

                                        onClicked: {
                                            console.log("send")
                                        }
                                    }

                                    Text {
                                        anchors.centerIn: parent

                                        text: "➤"
                                        color: "white"

                                        font.pixelSize: 18
                                        font.bold: true
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}