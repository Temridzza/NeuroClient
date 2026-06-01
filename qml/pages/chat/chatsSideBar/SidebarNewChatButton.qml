import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../../../theme"
import ChatGPT 1.0
import NeuroClient 1.0

Rectangle {
    Layout.fillWidth: true
    Layout.preferredHeight: 48

    radius: 14
    color: Theme.accentColor

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onClicked: {
            console.log("new chat")
            const newId = App.chats.createChat("Новый чат")
            App.chat.loadChat(newId)
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: 8

        Text {
            text: "+"
            color: Theme.textColor
            font.pixelSize: 20
            font.bold: true
        }

        Text {
            text: "Новый чат"
            color: Theme.textColor
            font.pixelSize: 15
            font.bold: true
        }
    }
}