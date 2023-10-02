import QtQuick 2.12
import QtQuick.Window 2.12

Item {
    id: root

    property alias text: title.text

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.topMargin: 40
    anchors.leftMargin: 34
    anchors.rightMargin: anchors.leftMargin
    implicitHeight: 50
    Text {
        id: title
        font.pixelSize: 26
        color: "#979797"
    }
    Rectangle {
        anchors.top: title.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 8
        height: 1
        color: title.color
    }
}
