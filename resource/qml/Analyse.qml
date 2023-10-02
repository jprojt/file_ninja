import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import CppTypes 1.0

Item {
    id: root
    anchors.fill: parent
    ViewHeader {
        id: header
        titleText: qsTr("File Type Analyser")
        subtitleText: {
            if (AnalyseViewModel.isRunning) {
                return `Indexing Extensions: (${Number(AnalyseViewModel.filesScanned).toLocaleString(Qt.locale(), 'f', 0)} files / ${Number(AnalyseViewModel.dirsScanned).toLocaleString(Qt.locale(), 'f', 0)} dirs).`;
            } else if (AnalyseViewModel.firstRunComplete) {
                return `Finished: ${Number(AnalyseViewModel.filesScanned).toLocaleString(Qt.locale(), 'f', 0)} Files Indexed.`;
            }

            return qsTr("Index all file types on the current system.");
        }
        imageSource: "qrc:/img/nav_analyse.svg"
    }

    DynamicWaffle {
        id: waffle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: start_button.top
        anchors.margins: 20

        opacity: AnalyseViewModel.firstRunComplete ? 1 : 0

        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }

    Slider {
        id: slider
        value: AnalyseViewModel.waffleListModel.size
        from: 10
        to: 50
        stepSize: 1
        anchors.left: parent.left
        anchors.right: start_button.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
        height: start_button.height
        opacity: AnalyseViewModel.firstRunComplete ? 1 : 0

        onValueChanged: {
            AnalyseViewModel.waffleListModel.size = value;
        }

        Text {
            color: "#71767E"
            id: tool_tip
            visible: slider.pressed
            text: AnalyseViewModel.waffleListModel.size + " x " + AnalyseViewModel.waffleListModel.size
            anchors.horizontalCenter: slider.handle.horizontalCenter
            anchors.bottom: slider.handle.top
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }

    AppButton {
        id: start_button
        buttonText: AnalyseViewModel.isRunning ? qsTr("Stop") :  qsTr("Start")
        buttonColor: AnalyseViewModel.isRunning ? "#8A1313" : defaultButtonColor
        hoverColor: AnalyseViewModel.isRunning ? "#B31818" : defaultHoverColor
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        state: AnalyseViewModel.firstRunComplete ? "afterFirstRun" : "beforeFirstRun"
        minimumWidth: 200
        minimumHeight: 48
        font.pixelSize: AnalyseViewModel.firstRunComplete ? 16 : 24

        onButtonClicked: {
            if (AnalyseViewModel.isRunning) {
                AnalyseViewModel.stop();
            } else {
                AnalyseViewModel.start();
            }
        }

        states: [
                State {
                    name: "afterFirstRun"
                    PropertyChanges {
                        target: start_button.anchors
                        rightMargin: 20
                        bottomMargin: 18
                    }
                    PropertyChanges {
                        target: start_button
                        minimumWidth: 100
                    }
                    AnchorChanges {
                        target: start_button
                        anchors.right: root.right
                        anchors.verticalCenter: undefined
                        anchors.horizontalCenter: undefined
                        anchors.bottom: root.bottom
                    }

                }
            ]

        transitions: Transition {
            AnchorAnimation { duration: 200 }
            PropertyAnimation {
                properties: "leftMargin,rightMargin,topMargin,bottomMargin,width,height,minimumWidth,minimumHeight,pixelSize"
                duration: 200
            }
        }
    }
}
