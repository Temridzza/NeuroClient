pragma Singleton
import QtQuick 2.15

QtObject {
    // флаг: Quickshell доступен?
    property bool quickShellAvailable: false

    // динамическая тема
    property Item theme: null

    function init(engine) {
        // Проверяем, можем ли импортнуть qs.modules.theme
        try {
            // создаём компонент из Quickshell theme
            var comp = Qt.createComponent("qs.modules.theme/Theme.qml")
            if (comp.status === Component.Ready) {
                theme = comp.createObject(null)
                quickShellAvailable = true
                console.log("Using qs.modules.theme")
                return
            }
        } catch(e) {
            console.log("qs.modules.theme not available, fallback to local theme")
        }

        // fallback на локальную тему
        var fallbackComp = Qt.createComponent("themeBridge/fallback/Theme.qml")
        if (fallbackComp.status === Component.Ready) {
            theme = fallbackComp.createObject(null)
        } else {
            console.error("Failed to load fallback theme")
        }
    }
}