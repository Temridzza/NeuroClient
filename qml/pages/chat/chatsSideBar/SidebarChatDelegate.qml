import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../../../theme"
import ChatGPT 1.0
import NeuroClient 1.0

Rectangle {
    id: root

    width: ListView.view.width
    height: 44
    radius: 12

    property string chatTitle: title
    property string chatId: id
    property bool hovered: false

    color: hovered ? Theme.hoverColor : "transparent"

    signal renameRequested(string chatId, string currentTitle)
    signal deleteRequested(string chatId)

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onEntered: root.hovered = true
        onExited: root.hovered = false

        onClicked: function(event) {
            if (event.button === Qt.LeftButton) {
                App.chat.loadChat(chatId)
            } else if (event.button === Qt.RightButton) {
                contextMenu.popup()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 8
        spacing: 10

        Rectangle {
            width: 8
            height: 8
            radius: 4
            color: Theme.accentColor
            Layout.alignment: Qt.AlignVCenter
        }

        Text {
            Layout.fillWidth: true
            text: chatTitle
            color: Theme.textColor
            font.pixelSize: 14
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        ToolButton {
            id: moreButton
            visible: root.hovered
            text: "⋮"
            Layout.alignment: Qt.AlignVCenter

            onClicked: contextMenu.popup()
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            text: "Переименовать"
            onTriggered: root.renameRequested(chatId, chatTitle)
        }

        MenuItem {
            text: "Удалить"
            onTriggered: root.deleteRequested(chatId)
        }
    }
}