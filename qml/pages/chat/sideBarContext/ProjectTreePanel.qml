import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components
import ChatGPT 1.0
import NeuroClient
import "../../../components"

StyledRect {
    id: root
    radius: 10
    variant: "internalbg"
    anchors.margins: 6
    Layout.fillWidth: true
    Layout.fillHeight: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 4

        // Заголовок
        StyledRect {
            id: header
            variant: "pane"
            radius: 0
            Layout.fillWidth: true
            Layout.preferredHeight: 24

            Text {
                anchors.centerIn: parent
                text: "Файлы проекта"
                color: header.item
                font.pixelSize: 13
                font.bold: true
            }
        }

        // Дерево файлов
        TreeView {
            id: treeView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: App.projectTreeModel
            selectionMode: TreeView.SingleSelection

            delegate: TreeViewDelegate {
                id: delegateItem

                width: treeView.width
                implicitHeight: 28

                property int indent: 18

                StyledRect {
                    anchors.fill: parent
                    radius: 4
                    variant: checked ? "focus" : "internalbg"
                }

                Row {
                    anchors.fill: parent
                    anchors.leftMargin: model.depth * indent
                    anchors.rightMargin: 6
                    spacing: 6

                    // 🔽 EXPANDER (место под раскрытие)
                    ToolButton {
                        visible: model.hasChildren

                        width: 16
                        height: 16

                        text: model.expanded ? "▾" : "▸"

                        background: Item {}
                        contentItem: Text {
                            text: parent.text
                            color: rowBg.item
                            font.pixelSize: 12
                        }

                        onClicked: model.expanded = !model.expanded
                    }

                    // ☑ CHECKBOX
                    ToolButton {
                        width: 18
                        height: 18

                        checkable: true
                        checked: model.selected
                        onClicked: model.selected = checked

                        background: StyledRect {
                            anchors.fill: parent
                            radius: 4
                            variant: parent.checked ? "primary" : "internalbg"
                        }

                        contentItem: Text {
                            anchors.centerIn: parent
                            text: "✔"
                            visible: parent.checked
                            color: rowBg.item
                            font.pixelSize: 11
                        }
                    }

                    // 📄 TEXT
                    Text {
                        text: model.name
                        verticalAlignment: Text.AlignVCenter
                        color: rowBg.item
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }

    function setSelectedRecursive(path, value) {
        for (let i = 0; i < App.projectTreeModel.count; i++) {
            let item = App.projectTreeModel.get(i)
            if (item.path === path || item.path.startsWith(path + "/")) {
                App.projectTreeModel.setProperty(i, "selected", value)
            }
        }
    }
}