import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components

ToolButton {
    id: root

    property string buttonText: ""
    property int radius: 18

    property string variantNormal: "internalbg"
    property bool checkedNormal: false

    signal clicked

    Layout.preferredWidth: 36
    Layout.preferredHeight: 36

    background: StyledRect {
        id: bg

        radius: root.radius

        variant: {
            if (root.checkedNormal)
                return "primary"

            if (mouseArea.pressed)
                return "primary"

            if (mouseArea.containsMouse)
                return "focus"

            return root.variantNormal
        }
    }

    contentItem: Text {
        anchors.centerIn: parent

        text: root.buttonText

        color: bg.item

        font.pixelSize: 16

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor

        onClicked: root.clicked()
    }

    onCheckedChanged: {
        bg.variant = checkedNormal
                     ? "primary"
                     : root.variantNormal
    }
}