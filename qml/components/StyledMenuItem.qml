// StyledMenuItem.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import qs.modules.theme

MenuItem {
    id: root

    signal itemTriggered()

    contentItem: Text {
        text: root.text
        color: root.hovered ? Colors.background : Colors.overBackground
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    onTriggered: itemTriggered()
}