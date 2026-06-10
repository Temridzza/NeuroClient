import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components

import ChatGPT 1.0
import NeuroClient

import "../../../components"

StyledRect {
    id: root

    Layout.fillWidth: true
    Layout.preferredHeight: 130

    radius: 10
    variant: "pane"

    function formatSize(bytes) {
        if (bytes < 1024)
            return bytes + " B"

        if (bytes < 1024 * 1024)
            return (bytes / 1024).toFixed(1) + " KB"

        if (bytes < 1024 * 1024 * 1024)
            return (bytes / (1024 * 1024)).toFixed(1) + " MB"

        return (bytes / (1024 * 1024 * 1024)).toFixed(1) + " GB"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 6

        Text {
            text: "AI Контекст"
            color: root.item

        }

        ElidedText {
            text: "Выбрано файлов: " + App.projectTreeModel.selectedFilesCount
            color: Qt.alpha(root.item, 0.8)
        }

        ElidedText {
            text: "Токенов: ~ " + App.projectTreeModel.selectedTokens
            color: "white"
        }

        ElidedText {
            text: "Размер: ~ "
                  + formatSize(App.projectTreeModel.selectedSize)
            color: Qt.alpha(root.item, 0.8)
        }

        Item {
            Layout.fillHeight: true
        }

        StyledToolButton {
            text: "Очистить"
        }
    }
}