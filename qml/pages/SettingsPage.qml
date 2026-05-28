import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../theme"
import ChatGPT 1.0

Item {
    id: root

    signal sectionSelected(string section)
    signal requestBack()

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.preferredWidth: 260
            Layout.fillHeight: true
            color: Theme.panelColor

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 6

                // 👇 КНОПКА НАЗАД
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
                            : "transparent"

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

                            onClicked: root.sectionSelected(modelData)
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Theme.chatBgColor

            Text {
                anchors.centerIn: parent
                text: "Select settings section"
                color: Theme.textColor
            }
        }
    }
}