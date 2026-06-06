import QtQuick
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components
import "../../../components"

RowLayout {
    spacing: 6

    Repeater {
        model: ["📄", "📁", "⟳"]

        delegate: StyledRect {
            id: buttonBg

            required property string modelData

            Layout.preferredWidth: 36
            Layout.preferredHeight: 36

            radius: 18
            variant: mouseArea.pressed
                     ? "primary"
                     : mouseArea.containsMouse
                       ? "focus"
                       : "internalbg"

            Text {
                anchors.centerIn: parent

                text: modelData
                color: buttonBg.item

                font.pixelSize: 16
            }

            MouseArea {
                id: mouseArea

                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
            }
        }
    }
}