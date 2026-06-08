import QtQuick
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components

import "../../../components"

StyledRect {
    id: root

    Layout.fillWidth: true
    Layout.preferredHeight: 130

    radius: 10
    variant: "pane"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 6

        Text {
            text: "AI Контекст"
            color: root.item

        }

        Text {
            text: "Выбрано файлов: 0"

            color: Qt.alpha(root.item, 0.8)
        }

        Text {
            text: "Примерно токенов: 0"

            color: Qt.alpha(root.item, 0.8)
        }

        Text {
            text: "Примерный размер: 0 KB"

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