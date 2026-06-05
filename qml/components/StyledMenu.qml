import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import qs.modules.theme
import ChatGPT 1.0
import NeuroClient 1.0

Menu {
    id: root

    background: Rectangle {
        implicitWidth: 180
        implicitHeight: 40

        color: Colors.surfaceContainer
        radius: 12
        border.color: Colors.primary
        border.width: 1
    }
}
