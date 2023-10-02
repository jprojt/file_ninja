import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CppTypes 1.0


ToolTip {
    id: root
    property QtObject model_data: new QtObject()

    font.pixelSize: 16
    timeout: 9999999
    visible: mouse_area.containsMouse
    width: contentWidth
    height: contentHeight
    clip: false
    contentItem: Item {
        width: childrenRect.width
        height: childrenRect.height
        anchors.fill: parent
        ColumnLayout {
            Text {
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.topMargin: 10
                Layout.bottomMargin: 2
                id: tip_title
                text: model_data.name === "" ? "(No Extension)" : model_data.name
                color: "#9398A0"
                font.pixelSize:19
            }
            Rectangle {
                Layout.minimumWidth: 180
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.bottomMargin: 13
                height: 1
                color: "#9398A0"
                anchors.rightMargin: 20
            }
            RowLayout {
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.bottomMargin: 5
                Image {
                    source: "qrc:/img/file_stack.svg"
                    sourceSize.width: 16
                }
                Text {
                    Layout.leftMargin: 5
                    text: Number(model_data.count).toLocaleString(Qt.locale(), 'f', 0) + " files"
                    color: "#9398A0"
                    font.pixelSize:14
                }
            }
            RowLayout {
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.bottomMargin: 20
                Image {
                    source: "qrc:/img/nav_analyse.svg"
                    sourceSize.width: 16
                }
                Text {
                    Layout.leftMargin: 5
                    text: AnalyseViewModel.bytesToString(model_data.value) + "  /  " + (model_data.percentage*100).toFixed(2).toString() + "%"
                    color: "#9398A0"
                    font.pixelSize:14
                }
            }
        }
    }

    background: Rectangle {
        anchors.fill: parent
        color: "#151618"
        radius: 10
        border.width: 1
        border.color: "#484848"
    }
}
