import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import ChatGPT 1.0
import NeuroClient 1.0
import qs.modules.theme
import qs.modules.components

import "../../../components"

StyledRect {
    id: sidebarContext
    signal animationStateChanged(bool animating)

    property bool animating: false

    radius: 22
    variant: "pane"   // фон берётся из темы Ambxst

    width: root.rightSidebarOpened ? 320 : 0
    visible: width > 0

    border.width: 10
    border.color: Colors.background

    layer.enabled: true
    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowOpacity: Theme.shadowOpacity
        shadowBlur: 0.8
    }

    Behavior on width {
        NumberAnimation {
            duration: 200
            onRunningChanged: sidebarContext.animationStateChanged(running)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        ProjectPathHeader {}

        QuickActionsBar {}

        ProjectTreePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        // AiContextPanel {}
    }
}