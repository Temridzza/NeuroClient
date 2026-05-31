import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../../theme"
import ChatGPT 1.0
import NeuroClient 1.0

Rectangle {
    width: ListView.view.width
    height: 44
    radius: 12

    property string chatTitle: title
    property string chatId: id

    color: mouse.containsMouse
           ? Theme.hoverColor
           : "transparent"

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onClicked: {
            console.log("open chat:", chatId, chatTitle)
            App.chats.openChat(chatId)
        }
    }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 12
        spacing: 10

        Rectangle {
            width: 8
            height: 8
            radius: 4
            color: Theme.accentColor
        }

        Text {
            text: chatTitle
            color: Theme.textColor
            font.pixelSize: 14
            elide: Text.ElideRight
            width: sidebar.width - 40
        }
    }
}