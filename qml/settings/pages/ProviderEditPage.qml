import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0
import ChatGPT 1.0
import NeuroClient 1.0

import "../../components"

import qs.modules.theme
import qs.modules.components

Item {
    id: root

    property string providerId: ""

    property var providerData: ({ })

    property string providerName: ""
    property string providerUrl: ""
    property string providerKey: ""

    signal openModel(var modelData)
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

            StyledToolButton {
                buttonText: "←"
                onClicked: {
                    root.backRequested()
                }
                variantNormal: "pane"
            }

            Text {
                text: "Редактор"

                font.pixelSize: 24
                color: Theme.textColor
            }

            Item {
                Layout.fillWidth: true
            }

            StyledToolButton {
                buttonText: "Сохранить"
                Layout.fillWidth: true
                variantNormal: "primary"
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
                        text: "Название"

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
                        text: "Базовый URL"

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

                            StyledToolButton {
                                buttonText: "+ модель"
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
                                variantNormal: "internalbg"
                                Layout.fillWidth: true
                            }
                        }
                    }
                }

                // MODELS
                ColumnLayout {
                    Layout.fillWidth: true

                    spacing: 10

                    Text {
                        text: "Модели"

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

                                StyledToolButton {
                                    buttonText: "🖉"
                                    onClicked: root.openModel(item)
                                }

                                StyledToolButton {
                                    buttonText: "×"
                                    onClicked: {
                                        App.models.deleteModel(item.id)

                                        App.models.loadModelsByProvider(
                                                    providerId
                                                    )
                                    }
                                    variantNormal: Colors.red
                                }
                            }
                        }
                    }

                    StyledRect {
                        Layout.fillWidth: true

                        height:
                            App.models.models.length === 0
                            ? 80
                            : 0

                        visible:
                            App.models.models.length === 0

                        radius: 12

                        variant: "pane"

                        Text {
                            anchors.centerIn: parent

                            text: "Сейчас нет моделей"

                            color: Theme.textColor
                            opacity: 0.5
                        }
                    }
                }

                StyledToolButton {
                    buttonText: "Удалить провайдера"
                    onClicked: {
                        App.providers.deleteProvider(
                                    providerId
                                    )

                        root.backRequested()
                    }
                    variantNormal: Colors.red
                    Layout.fillWidth: true
                }

                Item {
                    Layout.fillHeight: true
                    height: 40
                }
            }
        }
    }
}