import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components

import ChatGPT 1.0
import NeuroClient

import "../../../components"

StyledRect {
    Layout.fillWidth: true
    Layout.preferredHeight: 40

    radius: 10
    variant: "pane"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 6

        Text {
            Layout.fillWidth: true

            text: App.projectTreeModel.path
            color: "white"

            elide: Text.ElideRight

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                ToolTip.visible: containsMouse
                ToolTip.text: parent.text
            }
        }
    }
}