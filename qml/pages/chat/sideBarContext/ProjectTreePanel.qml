import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qs.modules.theme
import qs.modules.components
import ChatGPT 1.0
import NeuroClient
import "../../../components"

ScrollView {
    id: scroll

    Layout.fillWidth: true
    Layout.fillHeight: true

    clip: true

    TreeView {
        id: treeView

        width: scroll.availableWidth
        height: scroll.availableHeight

        model: App.projectTreeModel

        delegate: TreeViewDelegate {
            id: delegateRoot

            implicitHeight: 28

            leftMargin: 8
            rightMargin: 8
            indentation: 20

            background: Rectangle {
                color: delegateRoot.current
                       ? "#2d2d2d"
                       : "transparent"
            }

            contentItem: Row {
               spacing: 6

               CheckBox {
                   // 🔥 главное изменение: используем CheckStateRole
                   checkState: model.checkState

                   onToggled: {
                       model.checkState = checkState
                   }
               }

               Text {
                   text: model.name
                   color: "#d0d0d0"
                   verticalAlignment: Text.AlignVCenter
               }
            }
        }
    }
}