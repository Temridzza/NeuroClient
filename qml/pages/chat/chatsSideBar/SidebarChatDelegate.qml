import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components
import ChatGPT 1.0
import NeuroClient 1.0

import "../../../components"


Rectangle {
    id: root

    width: ListView.view ? ListView.view.width : implicitWidth
    height: 44
    radius: 12

    property string chatTitle: title
    property string chatId: id

    signal settingsRequested(string chatId)
    signal deleteRequested(string chatId)

    property bool hovered: rowMouse.containsMouse || /*moreButton.hovered ||*/ contextMenu.visible

    StyledRect {
        id: bg
        anchors.fill: parent
        radius: 12
        variant: rowMouse.containsMouse ? "primary" : "pane"
    }

    MouseArea {
        id: rowMouse
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                App.chat.loadChat(chatId)
            }
        }

        onPressed: function(mouse) {
            if (mouse.button === Qt.RightButton) {
                contextMenu.popup(mouse.x, mouse.y)
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
            color: bg.item   // цвет из темы (overbackground/контрастный)
            Layout.alignment: Qt.AlignVCenter
        }

        Text {
            Layout.fillWidth: true
            text: chatTitle
            font.pixelSize: 14
            elide: Text.ElideRight
            color: bg.item   // автоматически меняется с variant
        }

        StyledToolButton {
            buttonText: "⋮"
            onClicked: contextMenu.popup()
        }
    }

    StyledMenu {
        id: contextMenu

        StyledMenuItem {
            text: "Настройки"
            onItemTriggered: root.settingsRequested(chatId)
        }

        StyledMenuItem {
            text: "Удалить"
            onItemTriggered: root.deleteRequested(chatId)
        }
    }
}