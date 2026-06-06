// StyledMenuItem.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import qs.modules.theme
import qs.modules.components

MenuItem {
    id: root

    signal itemTriggered()

    background: StyledRect {
        id: bg
        anchors.fill: parent
        radius: 8

        variant: root.hovered ? "primary" : "internalbg"
    }

    contentItem: Text {
        text: root.text
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight

        color: bg.item
    }

    onTriggered: itemTriggered()
}