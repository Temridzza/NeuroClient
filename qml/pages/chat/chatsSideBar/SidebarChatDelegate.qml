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

    // hover у всей строки + hover у кнопки + открытое меню
    property bool hovered: rowMouse.containsMouse || moreButton.hovered || contextMenu.visible

    color: hovered ? Colors.primary : Colors.surfaceContainer

    signal renameRequested(string chatId, string currentTitle)
    signal deleteRequested(string chatId)

    MouseArea {
        id: rowMouse
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        onClicked: App.chat.loadChat(chatId)
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
            verticalAlignment: Text.AlignVCenter
        }

        ToolButton {
            id: moreButton
            visible: root.hovered
            hoverEnabled: true
            padding: 0
            Layout.alignment: Qt.AlignVCenter

            contentItem: Text {
                text: "⋮"
                color: Colors.overBackground
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 18
            }

            background: Rectangle {
                anchors.fill: parent
                anchors.margins: -10

                radius: 13

                color: moreButton.hovered ? Colors.background : "transparent"
            }

            onClicked: contextMenu.popup()
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            id: renameItem

            contentItem: Text {
                text: "Переименовать"
                color: renameItem.hovered ? Colors.background : Colors.overBackground
                verticalAlignment: Text.AlignVCenter
            }

            onTriggered: root.renameRequested(chatId, chatTitle)
        }

        MenuItem {
            id: deleteItem

            contentItem: Text {
                text: "Удалить"
                color: deleteItem.hovered ? Colors.background : Colors.overBackground
                verticalAlignment: Text.AlignVCenter
            }

            onTriggered: root.deleteRequested(chatId)
        }
    }
}