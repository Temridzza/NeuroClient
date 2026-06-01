pragma Singleton
import QtQuick 2.15

QtObject {
    // BACKGROUND
    property color bgColor: "#0f1117"
    property color panelColor: "#171923"
    property color chatBgColor: "#131720"

    // BORDERS
    property color borderColor: "#2b2f3a"

    // ACCENT
    property color accentColor: "#7c5cff"

    // TEXT
    property color textColor: "#ffffff"
    property color secondaryText: "#8b92a8"

    // STATES
    property color hoverColor: "#2a2f3a"
    property color disabledColor: "#555555"

    property real shadowOpacity: 0.4

    property color selectionColor: Qt.rgba(124/255, 92/255, 255/255, 0.35)

    // ICONS (NEW)
    property string iconCopy: "⧉"
    property string iconCheck: "✔"
    property string iconSend: "➤"
    property string iconMenu: "⋮"
}