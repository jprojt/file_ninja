import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CppTypes 1.0

Item {
    id: root
    anchors.fill: parent


    ColumnLayout {
        anchors.centerIn: parent
        Item {
            Layout.preferredWidth: 328
            Layout.preferredHeight: 268
            Layout.fillHeight: false
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.leftMargin: 50
            Layout.bottomMargin: 70
            Image {
                id: ninja_image
                source: "qrc:/img/ninja_home.png"
                anchors.centerIn: parent
                width: 328
                height: 268
                mipmap: true
                scale: 0
            }
        }

        Item {
            Layout.preferredWidth: 358
            Layout.preferredHeight: 39
            Layout.fillHeight: false
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Image {
                id: ninja_text_image
                source: "qrc:/img/ninja_text_home.png"
                anchors.centerIn: parent
                width: 358
                height: 39
                mipmap: true
                scale: 0
            }
        }
    }


    ParallelAnimation {
        id: intro_animation
        ScaleAnimator {
           target: ninja_image
           from: 0
           to: 1
           duration: 500

           easing.type: Easing.InOutBounce
        }
        SequentialAnimation {
            PauseAnimation {
                duration: 200
            }
            ScaleAnimator {
               target: ninja_text_image
               from: 0
               to: 1
               duration: 500
               easing.type: Easing.InOutBounce
            }
        }


    }

    Component.onCompleted: {
        intro_animation.start();
    }
}

