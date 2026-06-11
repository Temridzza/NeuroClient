import QtQuick
import QtQuick.Controls

import qs.modules.theme
import qs.modules.components

StyledRect {
    id: root

    default property alias content: contentContainer.data

    signal clicked()

    property bool hovered: false

    radius: 14

    variant: hovered ? "focus" : "pane"

    scale: hovered ? 1.01 : 1.0

    Behavior on scale {
        NumberAnimation {
            duration: 120
        }
    }

    MouseArea {
        anchors.fill: parent

        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor

        onEntered: root.hovered = true
        onExited: root.hovered = false
        onClicked: root.clicked()
    }

    Item {
        id: contentContainer

        anchors.fill: parent
    }
}