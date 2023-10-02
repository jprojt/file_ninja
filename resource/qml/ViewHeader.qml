import QtQuick 2.12
import QtQuick.Window 2.12

Item {
    id: root
    property string titleText: "-"
    property string subtitleText: "-"
    property string imageSource: ""

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.leftMargin: 50
    anchors.topMargin: 32
    implicitHeight: 70

    Image {
        id: title_image
        source: root.imageSource
        sourceSize.width: 64
        anchors.top: parent.top
    }

    Text {
        id: title
        anchors.left: title_image.right
        anchors.leftMargin: 24
        y: title_image.y-3
        padding: 0
        color: "#9398A0"
        font.pixelSize: 30
        text: root.titleText
        verticalAlignment: Text.AlignTop
    }

    Text {
        id: subtitle
        anchors.left: title_image.right
        anchors.leftMargin: 26
        text: root.subtitleText
        color: "#9398A0"
        anchors.top: title.bottom
        anchors.topMargin: 3
        font.pixelSize: 15
    }
}

