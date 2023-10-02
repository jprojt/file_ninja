import QtQuick 2.12
import QtQuick.Window 2.12
//import QtQuick.Controls 2.12


Item {
    signal navItemSelected(string navItem)

    Text {
        id: menu_title
        text: qsTr("MENU")
        anchors.top: parent.top
        anchors.left: parent.left
        font.pixelSize: 14
        color: "#5E6268"
    }

    Rectangle {
        id: menu_underline
        anchors.top: menu_title.bottom
        anchors.topMargin: 3
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 17
        height: 1
        color: menu_title.color
    }

    ListView {
        anchors.top: menu_underline.bottom
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        interactive: false
        model: ListModel {
            ListElement { icon: "qrc:/img/nav_search.svg"; name: "File Search"; content: "qrc:/Search.qml"}
            ListElement { icon: "qrc:/img/nav_analyse.svg"; name: "Analyse"; content: "qrc:/Analyse.qml"}

        }

        delegate: Item {
            width: parent.width
            height: 40

            Item {
                anchors.fill: parent
                anchors.margins: 2
                anchors.rightMargin: 20

                Rectangle {
                    id: nav_item_background
                    anchors.fill: parent
                    color: "#232627"
                    radius: 5
                    opacity: mouse_area.containsMouse ? 1 : 0

                    Behavior on opacity {
                        NumberAnimation {
                            duration: 200
                        }
                    }
                }

                Image {
                    id: nav_icon
                    source: model.icon
                    sourceSize.height: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    font.pixelSize: 15
                    color: "#9398A0"
                    text: model.name
                    anchors.left: nav_icon.right
                    anchors.right: parent.right
                    anchors.leftMargin: mouse_area.containsMouse ? 14 : 10
                    anchors.verticalCenter: parent.verticalCenter

                    Behavior on anchors.leftMargin {
                        NumberAnimation {
                            duration: 200
                        }
                    }
                }
            }

            MouseArea {
                id: mouse_area
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onClicked: {
                    navItemSelected(model.content);
                }
            }
        }
    }
}

