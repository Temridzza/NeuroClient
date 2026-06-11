import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0
import "../pages"
import "pages"
import "pages/models"
import "../components"

import ChatGPT 1.0
import NeuroClient 1.0

import qs.modules.theme
import qs.modules.components

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
        StyledRect {
            Layout.preferredWidth: 260
            Layout.fillHeight: true

            variant: "transparent"

            Column {
                anchors.fill: parent
                anchors.margins: 10

                spacing: 6

                // ─────────────────────────────
                // BACK BUTTON
                // ─────────────────────────────

                StyledToolButton {
                    buttonText: "← Вернуться"
                    onClicked: {
                        root.requestBack()
                    }
                    Layout.fillWidth: true
                    variantNormal: "pane"
                }

                // ─────────────────────────────
                // MENU
                // ─────────────────────────────
                ListView {
                    width: parent.width
                    height: parent.height - 50

                    model: [
                        "Провайдеры",
                        "Модели"
                    ]

                    delegate: StyledToolButton {
                        width: ListView.view.width
                        height: 42
                        variantNormal: "pane"

                        buttonText: modelData
                        radius: 10

                        checkedNormal :
                            index === root.currentIndex
                            && !root.editingProvider
                            && !root.editingModel

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

        // ─────────────────────────────
        // CONTENT AREA
        // ─────────────────────────────
        StyledRect {
            Layout.fillWidth: true
            Layout.fillHeight: true

            variant: "transparent"

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