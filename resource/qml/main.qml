import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import CppTypes 1.0

ApplicationWindow {
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.TransparentMode
    color: "transparent"
    id: root
    visible: true
    width: 800
    height: 600
    minimumHeight: height
    maximumHeight: height
    minimumWidth: width
    maximumWidth: width
    title: qsTr("Capstone")

    function handleNavItemSelected(navItem) {
        content_container.source = navItem;
    }

    Item {
        id:app_main_container
        anchors.fill: parent

        MouseArea {
            id: mousedragger
            property int prevX: 0
            property int prevY: 0
            anchors.fill: parent

            onPressAndHold: {
            }

            onPressed: {prevX=mouseX; prevY=mouseY}
            onPositionChanged:{
                var deltaX = mouseX - prevX;
                root.x += deltaX;
                prevX = mouseX - deltaX;

                var deltaY = mouseY - prevY
                root.y += deltaY;
                prevY = mouseY - deltaY;
            }
        }

        Rectangle {
            color: "#151618"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            radius: 8
        }

        Item {
            id: sidebar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: 200
            enabled: !lightbox.visible // TODO: bind to settings viewmodel

            Image {
                id: close_button
                source: "qrc:/img/window_close.svg"
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 8
                sourceSize.width: 14
                sourceSize.height: 14
                width: 14
                height: 14
                opacity: close_mouse_area.containsMouse ? 1 : 0.7


                MouseArea {
                    id: close_mouse_area
                    hoverEnabled: true
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        AnalyseViewModel.stop();
                        SearchViewModel.stop();
                        Qt.quit();
                    }
                }
            }

            Image {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 22
                anchors.left: parent.left
                anchors.leftMargin: 18
                id: logo
                source: "qrc:/img/logo.svg"
                sourceSize.width: 147
            }

            Navigation {
                id: navigation
                anchors.topMargin: 25
                anchors.leftMargin: 20
                anchors.top: close_button.bottom
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: 200

                Component.onCompleted: {
                    navItemSelected.connect(root.handleNavItemSelected);
                }
            }
        }

        Rectangle {
            id: content_background
            color: "#232627"
            radius: 8
            anchors.left: sidebar.right
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            enabled: !lightbox.visible // TODO: bind to settings viewmodel

            Rectangle {
                id: radius_fix
                color: parent.color
                anchors.left: parent.left
                width: parent.radius+1
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }

            Item {
                id: loader_animator
                anchors.fill: parent
                opacity: 0

                Loader {
                    id: content_container
                    anchors.fill: parent
                    source: "qrc:/Default.qml"

                    onSourceChanged: {
                        animate_bahaviour.enabled = false;
                        loader_animator.opacity = 0;
                        animate_bahaviour.enabled = true;
                        loader_animator.opacity = 1;
                    }
                }

                Behavior on opacity {
                    id: animate_bahaviour
                    NumberAnimation {
                        duration: 400
                    }
                }
            }
        }
    }


    Lightbox {
        id: lightbox
        visible: false
        content_width: 700
        content_height: 500

        Connections {
            target: SearchViewModel

            function onRequestedDisplaySettings() {
                lightbox.content_source = "qrc:/SearchSettings.qml"
                lightbox.visible = true;
            }
        }
    }
}
