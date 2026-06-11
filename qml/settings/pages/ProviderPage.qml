import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0
import ChatGPT 1.0
import NeuroClient 1.0

import qs.modules.theme
import qs.modules.components

import "../../components"

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
                text: "Провайдеры"
                font.pixelSize: 22
                color: Theme.textColor
            }

            Item {
                Layout.fillWidth: true
            }

            StyledToolButton {
                Layout.fillWidth: true
                buttonText: "+ Провайдер"
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
                variantNormal: "pane"
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

            delegate: StyledCard {
                width: ListView.view.width
                height: 110

                onClicked: root.openProvider(id)
                required property string id
                required property string name
                required property string base_url
                required property string api_key

                Column {
                    anchors.fill: parent
                    anchors.margins: 14

                    spacing: 4

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

                    Text {
                        text: "url: " + (
                                  base_url.length > 0
                                  ? base_url
                                  : "(empty)"
                              )

                        color: Theme.textColor
                        opacity: 0.7
                        font.pixelSize: 13

                        width: parent.width
                        elide: Text.ElideRight
                    }

                    Text {
                        text: "ключ: " + (
                                  api_key.length > 0
                                  ? "********"
                                  : "(none)"
                              )

                        color: Theme.textColor
                        opacity: 0.7
                        font.pixelSize: 13
                    }

                    Text {
                        text: id

                        color: Theme.textColor
                        opacity: 0.35
                        font.pixelSize: 11

                        width: parent.width
                        elide: Text.ElideMiddle
                    }
                }
            }
        }
    }
}