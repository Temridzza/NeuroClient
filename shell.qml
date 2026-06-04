// shell.qml – точка входа Quickshell
import QtQuick 2.15
import QtQuick.Window 2.15
import ChatGPT 1.0          // <-- наш модуль
import "qml/themeBridge" 1.0
import "qml"

Window {
    width: 400
    height: 400
    visible: true

    Component.onCompleted: {
        ThemeBridge.init(Qt.application.engine)
    }

    Main {
        visible: true
    }
}