import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12


Item {    
    id: root
    signal enterPressed
    property alias text: text_area.text
    property bool is_invalid: false
    property alias icon_source: icon_image.source
    property alias icon_sourceWidth: icon_image.sourceSize.width
    property alias icon_sourceHeight: icon_image.sourceSize.height

    Rectangle {
        color: "#1B1D22"
        anchors.fill: parent
        radius: 15
    }

    Image {
       id: icon_image
       anchors.left: parent.left
       anchors.leftMargin: 10
       anchors.verticalCenter: parent.verticalCenter
    }

    TextField {
        id: text_area
        selectByMouse: true
        anchors.left: icon_image.right
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        color: root.is_invalid ? "red" : "#9398A0"
        font.pixelSize: 16

        background: Rectangle {
            color: "transparent"
        }

        Keys.onEscapePressed: {
            text_area.text = "";
        }

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                root.enterPressed();
                event.accepted = true;
            }
        }
    }

    onFocusChanged: {
        if (focus) {
            text_area.focus = true;
            text_area.forceActiveFocus();
        }
    }
}

