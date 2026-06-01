// qml/Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Effects
// ── Ваши локальные модули ─────────────────────
import "pages"
import "pages/chat"
import "pages/chat/chatsSideBar"
import "settings"

// ── Плагин, который мы собираем в CMake ───────
import ChatGPT 1.0               // ← URI, объявленный в CMake
import NeuroClient 1.0           // ← singleton‑provider, объявлен в QmlModule.cpp
import Theme 1.0

/*-------------------------------------------------
   Корневой элемент. В Quickshell‑скрипте это будет
   вложено в окно, которое уже создано Quickshell.
-------------------------------------------------*/
Item {
    id: root

    implicitWidth: 1280
    implicitHeight: 820

    // -----------------------------------------------------------------
    //  Свойства, которыми пользуется ваш UI (сайд‑бар, роутер страниц)
    // -----------------------------------------------------------------
    property bool sidebarOpened: false
    property string currentPage: "chat"

    // -----------------------------------------------------------------
    //  Фоновый прямоугольник (стилизация, тень и пр.)
    // -----------------------------------------------------------------
    Rectangle {
        anchors.fill: parent
        radius: 24
        color: Theme.bgColor
        border.color: Theme.borderColor
        border.width: 1

        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.8
            shadowOpacity: Theme.shadowOpacity
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 14

            // ---------------------------------------------------------
            //  Сайд‑бар
            // ---------------------------------------------------------
            ChatsSideBar {
                id: sideBar
                Layout.fillHeight: true
                Layout.preferredWidth: sidebarOpened ? 320 : 0
                clip: true
                visible: width > 0

                Behavior on Layout.preferredWidth {
                    NumberAnimation {
                        duration: 180
                        easing.type: Easing.OutCubic
                    }
                }
            }

            // ---------------------------------------------------------
            //  Основная область – роутер между «чатом» и «настройками»
            // ---------------------------------------------------------
            Loader {
                id: pageLoader
                Layout.fillWidth: true
                Layout.fillHeight: true

                sourceComponent: root.currentPage === "settings"
                                 ? settingsPage
                                 : chatPage
            }
        }
    }

    // -----------------------------------------------------------------
    //  Чат‑страница
    // -----------------------------------------------------------------
    Component {
        id: chatPage
        ChatPage {
            sidebarOpened: root.sidebarOpened

            // Прокидываем двунаправленную связь
            onSidebarOpenedChanged: root.sidebarOpened = sidebarOpened
            onRequestSettings: root.currentPage = "settings"
        }
    }

    // // -----------------------------------------------------------------
    // //  Страница настроек
    // // -----------------------------------------------------------------
    Component {
        id: settingsPage
        SettingsPage {
            anchors.fill: parent
            onRequestBack: root.currentPage = "chat"
        }
    }
}