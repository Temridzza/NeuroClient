import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Text {
    id: root

    Layout.fillWidth: true
    elide: Text.ElideRight
    wrapMode: Text.NoWrap

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        ToolTip.visible: containsMouse
        ToolTip.text: root.text
    }
}