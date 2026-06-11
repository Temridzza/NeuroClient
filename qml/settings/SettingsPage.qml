import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0
import "../pages"
import "pages"
import "pages/models"

import ChatGPT 1.0
import NeuroClient 1.0



Item {
    id: root

    signal requestBack()

    property int currentIndex: 0

    property string editingProviderId: ""
    property bool editingProvider: false

    property var editingModelData: ({})
    property bool editingModel: false

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // ─────────────────────────────
        // LEFT SIDEBAR
        // ─────────────────────────────
        Rectangle {
            Layout.preferredWidth: 260
            Layout.fillHeight: true

            color: Theme.panelColor

            Column {
                anchors.fill: parent
                anchors.margins: 10

                spacing: 6

                // ─────────────────────────────
                // BACK BUTTON
                // ─────────────────────────────
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

                        onClicked: {
                            root.requestBack()
                        }
                    }
                }

                // ─────────────────────────────
                // MENU
                // ─────────────────────────────
                ListView {
                    width: parent.width
                    height: parent.height - 50

                    model: [
                        "Providers",
                        "Models"
                    ]

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 42
                        radius: 10

                        property bool hovered: false

                        color:
                            hovered
                            ? Qt.lighter(
                                  Theme.panelColor,
                                  1.2
                                  )
                            : (
                                  index === root.currentIndex
                                  && !root.editingProvider
                                  && !root.editingModel
                                  ? Qt.darker(
                                        Theme.panelColor,
                                        1.2
                                        )
                                  : "transparent"
                                  )

                        Text {
                            anchors.centerIn: parent

                            text: modelData

                            color: Theme.textColor
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
                                root.editingProvider = false
                                root.editingModel = false

                                root.currentIndex = index

                                if (modelData === "Models")
                                       App.models.loadModels()
                            }
                        }
                    }
                }
            }
        }

        // ─────────────────────────────
        // CONTENT AREA
        // ─────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: Theme.chatBgColor

            // ─────────────────────────────
            // NORMAL SETTINGS PAGES
            // ─────────────────────────────
            StackLayout {
                anchors.fill: parent

                visible: !root.editingProvider && !root.editingModel

                currentIndex: root.currentIndex

                // PROVIDERS
                ProviderPage {
                    onOpenProvider: function(providerId) {
                        root.editingProviderId =
                                providerId

                        root.editingProvider = true
                    }
                }

                // MODELS
                ModelPage {
                    onOpenModel: function(modelData) {
                        root.editingModelData = modelData
                        root.editingModel = true
                    }
                }
            }

            // ─────────────────────────────
            // PROVIDER EDITOR
            // ─────────────────────────────
            Loader {
                anchors.fill: parent

                active: root.editingProvider

                visible: root.editingProvider

                sourceComponent: ProviderEditPage {
                    providerId:
                        root.editingProviderId

                    onBackRequested: {
                        root.editingProvider = false
                    }

                    onOpenModel: function(modelData) {
                            root.editingModelData = modelData

                            root.editingProvider = false
                            root.editingModel = true
                        }
                }
            }

            // ─────────────────────────────
            // MODEL EDITOR
            // ─────────────────────────────
            Loader {
                anchors.fill: parent

                active: root.editingModel

                visible: root.editingModel

                sourceComponent: ModelEditPage {
                    modelData: root.editingModelData

                    onBackRequested: {
                        root.editingModel = false
                    }
                }
            }
        }
    }
}