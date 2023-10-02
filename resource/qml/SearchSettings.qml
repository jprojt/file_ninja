import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CppTypes 1.0

Item {
    id: root
    anchors.fill: parent
    readonly property int first_row_size: 180

    SettingHeader {
        id: header
        text: qsTr("File Search Settings")
    }

    ColumnLayout {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: header.anchors.leftMargin
        anchors.rightMargin: anchors.leftMargin
        anchors.topMargin: 30

        RowLayout {
            Layout.fillWidth: true
            Text {
                font.pixelSize: 16
                color: "#9398A0"
                text: qsTr("Limit Results:") + " (" + Number(limit_slider.value).toLocaleString(Qt.locale(), 'f', 0) + ")"
                Layout.preferredWidth: root.first_row_size
            }

            Slider {
                id: limit_slider
                from: 10
                to: 100000
                stepSize: 1
                implicitWidth: 300
                Layout.fillWidth: true
                value: Number(SearchViewModel.resultLimit)
                onValueChanged: {
                    SearchViewModel.resultLimit = value;
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                font.pixelSize: 16
                color: "#9398A0"
                text: qsTr("Search Directory:")
                Layout.preferredWidth: root.first_row_size
            }
            Textbox {
                text: SearchViewModel.searchPath
                width: 300
                height: 40
                Layout.fillWidth: true

                onTextChanged: {
                    SearchViewModel.searchPath = text;
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                font.pixelSize: 16
                color: "#9398A0"
                text: qsTr("Use Regex:")
                Layout.preferredWidth: root.first_row_size
            }
            CheckBox {
                checkState: SearchViewModel.useRegex ? Qt.Checked : Qt.Unchecked

                onCheckedChanged: {
                    SearchViewModel.useRegex = checked;
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                font.pixelSize: 16
                color: "#9398A0"
                text: qsTr("Case Sensitive:")
                Layout.preferredWidth: root.first_row_size
            }
            CheckBox {
                checkState: SearchViewModel.caseSensitive ? Qt.Checked : Qt.Unchecked

                onCheckedChanged: {
                    SearchViewModel.caseSensitive = checked;
                }
            }
        }
    }
}

