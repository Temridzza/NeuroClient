import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../../theme"
import ChatGPT 1.0
import NeuroClient

Item {
    id: root

    signal sendRequested(string text)

    ColumnLayout {
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.8, 900)
        spacing: 24

        Label {
            Layout.alignment: Qt.AlignHCenter

            text: "Чем могу помочь?"
            color: Theme.textColor

            font.pixelSize: 32
            font.bold: true
        }

        ChatInput {
            Layout.fillWidth: true
            loading: false

            onSendRequested: function(text) {
                root.sendRequested(text)
            }
        }
    }
}