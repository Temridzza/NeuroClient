// shell.qml – точка входа Quickshell
import QtQuick 2.15
import QtQuick.Window 2.15
import ChatGPT 1.0          // <-- наш модуль
import "themeBridge" 1.0

Window {
    id: win
    width: 1280
    height: 820
    visible: true
    title: qsTr("Quickshell + NeuroClient")

    Component.onCompleted: {
        ThemeBridge.init(Qt.application.engine)
        if (ThemeBridge.theme) {
            ThemeBridge.theme.parent = win
        }
    }
}