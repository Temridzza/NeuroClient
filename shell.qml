// shell.qml – точка входа Quickshell
import QtQuick 2.15
import QtQuick.Window 2.15
import ChatGPT 1.0          // <-- наш модуль
import NeuroClient 1.0     // <-- singleton (App)

Window {
    id: win
    width: 1280
    height: 820
    visible: true
    title: qsTr("Quickshell + NeuroClient")

    // Подключаем главный UI‑компонент, объявленный в qml/Main.qml
    Main {               // имя компонента = файл Main.qml
        anchors.fill: parent
    }
}