// ReusableComboBox.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import qs.modules.theme

ComboBox {
    id: comboBox

    // обычные свойства-обёртки (НЕ alias)
    property var itemsModel
    property int selectedIndex: -1
    property string displayRole: "display_name"
    property var onItemSelected

    model: itemsModel
    currentIndex: selectedIndex
    textRole: displayRole

    background: Rectangle {
        radius: 10
        color: Qt.alpha(Colors.primary, 0.08)
        border.width: 1
        border.color: Qt.alpha(Colors.primary, 0.18)
    }

    contentItem: Text {
        text: comboBox.displayText
        color: Colors.overBackground
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
        leftPadding: 12
        rightPadding: 24
        elide: Text.ElideRight
    }

    indicator: Text {
        text: "⌄"
        color: Colors.primary
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }

    popup: Popup {
        y: comboBox.height + 6
        width: comboBox.width

        background: Rectangle {
            radius: 12
            color: Colors.surfaceContainer
            border.width: 1
            border.color: Qt.alpha(Colors.primary, 0.15)
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: comboBox.popup.visible ? comboBox.delegateModel : null
        }
    }

    delegate: ItemDelegate {
        width: comboBox.width
        height: 36

        background: Rectangle {
            color: highlighted ? Qt.alpha(Colors.primary, 0.12) : "transparent"
        }

        contentItem: Text {
            text: modelData[comboBox.displayRole]
            color: highlighted ? Colors.primary : Colors.overBackground
            verticalAlignment: Text.AlignVCenter
        }
    }

    onActivated: function(index) {
        if (comboBox.onItemSelected) {
            comboBox.onItemSelected(model[index].id)
        }
    }
}