import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls

import qs.modules.theme
import qs.modules.components
import "../../../components"

import ChatGPT 1.0
import NeuroClient

StyledRect {
    radius: 10
    variant: "pane"

    implicitWidth: content.implicitWidth + 16
    implicitHeight: content.implicitHeight + 16

    FileDialog {
        id: fileDialog
        title: "Выберите файл"

        onAccepted: {
            App.projectTreeModel.loadFile(selectedFile)
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Выберите папку"

        onAccepted: {
            App.projectTreeModel.loadFolder(selectedFolder)
        }
    }

    RowLayout {
        id: content
        anchors.fill: parent
        anchors.margins: 8
        spacing: 10

        StyledToolButton {
            buttonText: "📄"
            onClicked: fileDialog.open()
        }
        StyledToolButton {
            buttonText: "📁"
            onClicked: folderDialog.open()
        }
        StyledToolButton {
            buttonText: "⟳"
            onClicked: App.projectTreeModel.reloadProject()
        }
    }
}