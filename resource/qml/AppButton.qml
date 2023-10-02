import QtQuick 2.12
import QtQuick.Window 2.12
//import QtQuick.Controls 2.12

Rectangle {
    id: root
    property string buttonText: "Button"
    property int minimumWidth: 130
    property int minimumHeight: 48
    property string buttonColor: "#1F7022"
    property string hoverColor: "#248427"
    property string textColor: "white"
    readonly property string defaultButtonColor: "#1F7022"
    readonly property string defaultHoverColor: "#248427"
    property alias font: button_text.font
    signal buttonClicked

    radius: 8
    color: mouse_area.containsMouse ? root.hoverColor : root.buttonColor


    width: Math.max(button_text.implicitWidth + 20, root.minimumWidth)
    height: Math.max(button_text.implicitHeight + 10, root.minimumHeight)

    Text {
        id: button_text
        text: root.buttonText
        color: root.textColor
        font.pixelSize: 24
        verticalAlignment: Qt.AlignCenter
        horizontalAlignment: Qt.AlignCenter
        anchors.centerIn: parent
        leftPadding: 20
        rightPadding: 20
        topPadding: 5
        bottomPadding: 8

        Behavior on font.pixelSize {
            NumberAnimation {
                duration: 200
            }
        }
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            root.buttonClicked();
        }
    }

    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }
}
