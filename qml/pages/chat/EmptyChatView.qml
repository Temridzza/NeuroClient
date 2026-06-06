import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components
import ChatGPT 1.0
import NeuroClient

Item {
    id: root

    signal sendRequested(string text)

    StyledRect {
        anchors.fill: parent
        variant: "transparent"

        ColumnLayout {
            anchors.centerIn: parent
            width: Math.min(parent.width * 0.8, 900)
            spacing: 24

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "Чем могу помочь?"

                // Берём контрастный цвет для поверхностей pane / internalbg
                color: Styling.srItem("overBackground")  // как в Ambxst
                font.pixelSize: 32
                font.bold: true
            }

            ChatInput {
                Layout.fillWidth: true
                loading: false

                onSendRequested: App.chat.sendMessage(text)
            }
        }
    }
}