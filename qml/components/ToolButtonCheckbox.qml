import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components

import ChatGPT 1.0
import NeuroClient

ToolButton {
    id: root

    // property bool checked: false
    signal toggled(bool checked)

    Layout.preferredWidth: 18
    Layout.preferredHeight: 18

    background: StyledRect {
        id: bg
        anchors.fill: parent
        radius: 4
        variant: root.checked ? "primary" : "internalbg"
    }

    contentItem: Text {
        anchors.centerIn: parent
        text: "✔"
        visible: root.checked
        color: "white"
        font.pixelSize: 11
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onEntered: {
            if (!root.checked)
                bg.variant = "focus"
        }

        onExited: {
            if (!root.checked)
                bg.variant = "internalbg"
        }

        onPressed: {
            bg.variant = "primary"
        }

        onReleased: {
            if (!root.checked)
                bg.variant = containsMouse ? "focus" : "internalbg"
        }

        onClicked: {
            root.checked = !root.checked
            root.toggled(root.checked)
        }
    }

    // синхронизация внешнего состояния (если изменяют снаружи)
    onCheckedChanged: {
        bg.variant = checked ? "primary" : "internalbg"
    }
}

