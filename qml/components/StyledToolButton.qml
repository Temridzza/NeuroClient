import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components

ToolButton {
    id: root

    // Параметры для переиспользования
    property string buttonText: ""                  // <-- новое имя
    property int radius: 18
    property string variantNormal: "internalbg"
    signal clicked

    Layout.preferredWidth: 36
    Layout.preferredHeight: 36

    background: StyledRect {
        id: bg
        radius: root.radius
        variant: root.variantNormal
    }

    contentItem: Text {
        anchors.centerIn: parent
        text: root.buttonText                           // <-- используем новое имя
        color: bg.item
        font.pixelSize: 16
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: bg.variant = "focus"
        onExited: bg.variant = root.variantNormal
        onPressed: bg.variant = "primary"
        onReleased: bg.variant = containsMouse ? "focus" : root.variantNormal
        onClicked: root.clicked()
        cursorShape: Qt.PointingHandCursor
    }
}