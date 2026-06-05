import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import qs.modules.theme
import ChatGPT 1.0
import NeuroClient 1.0

Rectangle {
    id: root

    width: ListView.view ? ListView.view.width : implicitWidth
    height: 44
    radius: 12

    property string chatTitle: title
    property string chatId: id

    signal settingsRequested(string chatId)
    signal deleteRequested(string chatId)

    property bool hovered: rowMouse.containsMouse || moreButton.hovered || contextMenu.visible

    color: hovered ? Colors.primary : Colors.surfaceContainer

    MouseArea {
        id: rowMouse
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: App.chat.loadChat(chatId)

        onPressed: function(mouse) {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup(mouse.x, mouse.y)
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
            color: Colors.overBackground
            Layout.alignment: Qt.AlignVCenter
        }

        Text {
            Layout.fillWidth: true
            text: chatTitle
            color: hovered ? Colors.background : Colors.overBackground
            font.pixelSize: 14
            elide: Text.ElideRight
        }

        ToolButton {
            id: moreButton
            visible: root.hovered
            hoverEnabled: true
            padding: 0

            contentItem: Text {
                text: "⋮"
                color: Colors.overBackground
                font.pixelSize: 18
            }

            onClicked: contextMenu.popup()
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            contentItem: Text {
                text: "Настройки"
                color: hovered ? Colors.background : Colors.overBackground
            }
            onTriggered: {
                root.settingsRequested(chatId)
                console.log("contextMenu MenuItem onTriggered " + chatId)
            }
        }

        MenuItem {
            contentItem: Text {
                text: "Удалить"
                color: hovered ? Colors.background : Colors.overBackground
            }

            onTriggered: root.deleteRequested(chatId)
        }
    }
}