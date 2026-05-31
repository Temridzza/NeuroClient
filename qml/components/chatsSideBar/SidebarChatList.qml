import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../../theme"
import ChatGPT 1.0
import NeuroClient 1.0

ListView {
    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true

    model: App.chats

    delegate: SidebarChatDelegate {}
}