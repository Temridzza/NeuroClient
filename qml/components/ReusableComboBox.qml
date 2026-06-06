// ReusableComboBox.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components

ComboBox {
    id: comboBox

    property var itemsModel
    property int selectedIndex: -1
    property string displayRole: "display_name"
    property var onItemSelected

    property int contentHPadding: 12
    property int popupHPadding: 16
    property int rowHeight: 36

    model: itemsModel
    currentIndex: selectedIndex
    textRole: displayRole

    function getModelItem(index) {
        if (!itemsModel)
            return null

        if (Array.isArray(itemsModel))
            return itemsModel[index]

        if (itemsModel.get)
            return itemsModel.get(index)

        return itemsModel[index]
    }

    function getRowCount() {
        if (!itemsModel)
            return 0

        if (Array.isArray(itemsModel))
            return itemsModel.length

        if (itemsModel.count !== undefined)
            return itemsModel.count

        if (itemsModel.length !== undefined)
            return itemsModel.length

        return 0
    }

    function getDisplayTextAt(index) {
        const item = getModelItem(index)
        if (item === null || item === undefined)
            return ""

        if (typeof item === "string")
            return item

        if (item[displayRole] !== undefined)
            return String(item[displayRole])

        return String(item)
    }

    function getItemId(index) {
        const item = getModelItem(index)
        if (!item)
            return undefined
        return item.id !== undefined ? item.id : item
    }

    function recalcPopupWidth() {
        let maxWidth = width

        for (let i = 0; i < getRowCount(); ++i) {
            const text = getDisplayTextAt(i)
            if (!text)
                continue

            textMetrics.text = text
            maxWidth = Math.max(maxWidth, Math.ceil(textMetrics.width) + popupHPadding * 2)
        }

        popupContentWidth = maxWidth
    }

    property int popupContentWidth: width

    Component.onCompleted: recalcPopupWidth()
    onWidthChanged: recalcPopupWidth()
    onItemsModelChanged: recalcPopupWidth()
    onDisplayRoleChanged: recalcPopupWidth()

    TextMetrics {
        id: textMetrics
        font: comboBox.font
    }

    background: StyledRect {
        id: bg
        radius: 10
        variant: "internalbg"
    }

    contentItem: Text {
        text: comboBox.displayText
        color: bg.item
        font: comboBox.font
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        anchors.left: parent.left
        anchors.leftMargin: comboBox.contentHPadding
        anchors.right: parent.right
        anchors.rightMargin: comboBox.contentHPadding + 12
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    indicator: Text {
        text: "⌄"
        color: bg.item
        font: comboBox.font
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }

    popup: Popup {
        y: comboBox.height + 6
        width: Math.max(comboBox.width, comboBox.popupContentWidth)

        background: StyledRect {
            radius: 12
            variant: "pane"
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            width: parent.width
            model: comboBox.popup.visible ? comboBox.delegateModel : null
        }
    }

    delegate: ItemDelegate {
        width: ListView.view ? ListView.view.width : comboBox.width
        height: comboBox.rowHeight

        background: StyledRect {
            id: delegateBg
            radius: 0
            variant: hovered ? "primary" : "internalbg"
        }

        contentItem: Text {
            text: modelData && modelData[comboBox.displayRole] !== undefined
                  ? String(modelData[comboBox.displayRole])
                  : comboBox.getDisplayTextAt(index)

            color: delegateBg.item
            font: comboBox.font
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideNone
            wrapMode: Text.NoWrap
            anchors.left: parent.left
            anchors.leftMargin: comboBox.popupHPadding
            anchors.right: parent.right
            anchors.rightMargin: comboBox.popupHPadding
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        onClicked: {
            comboBox.currentIndex = index
            if (comboBox.onItemSelected)
                comboBox.onItemSelected(comboBox.getItemId(index))
        }
    }
}