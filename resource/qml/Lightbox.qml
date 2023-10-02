import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12


Item {
    id: root
    property alias content_source: loader.source
    property alias content_width: loader.width
    property alias content_height: loader.height
    anchors.fill: parent
//    anchors.margins: 20
    visible: true // TODO: bind to view model
    // TODO: extract this stuff out into a separate qml file
    Rectangle {
        color: "black"
        opacity: 0.8
        anchors.fill: parent
        radius: content_background.radius
    }

    Rectangle {
        id: lightbox_content_background
        anchors.centerIn: parent
        width: loader.width + 20
        height: loader.height + 20
        radius: 30
        color: "#232627"
        Loader {
            id: loader
            anchors.centerIn: parent
        }

        Image {
            source: "qrc:/img/close_icon.svg"
            sourceSize.width: 16
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 32
            anchors.rightMargin: 38
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.visible = false;
                    loader.source = "";
                }
            }
        }
    }
    }

    // TODO: change for settings cog image

