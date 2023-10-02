import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import CppTypes 1.0
//import Qt.labs.qmlmodels
import QtQml.Models 2.1

Item {
    id: root
    anchors.fill: parent

    function rowHeight(row) {
        return 25;
    }

    ViewHeader {
        id: title_header
        titleText: qsTr("File Search")
        subtitleText: {
            if (SearchViewModel.isSearching) {
                return qsTr(`Searching: checked ${Number(SearchViewModel.filesScanned).toLocaleString(Qt.locale(), 'f', 0)} files in ${Number(SearchViewModel.dirsScanned).toLocaleString(Qt.locale(), 'f', 0)} dirs`);
            } else if (SearchViewModel.firstSearchComplete) {
                return qsTr(`Found ${Number(table_view.rows).toLocaleString(Qt.locale(), 'f', 0)} files [checked ${Number(SearchViewModel.filesScanned).toLocaleString(Qt.locale(), 'f', 0)} files in ${Number(SearchViewModel.dirsScanned).toLocaleString(Qt.locale(), 'f', 0)} dirs]`);
            } else {
                return qsTr("Search for files on this sytem by name");
            }
        }
        imageSource: "qrc:/img/nav_search.svg"
    }

    Image {
        source: "qrc:/img/settings_cog.svg"
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 35
        anchors.topMargin: 40
        sourceSize.width: 24
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                SearchViewModel.showSettings();
            }
        }
    }

    ListView {
        visible: SearchViewModel.firstSearchComplete
        id: placeholder_rows
        anchors.top: table_view.top
        anchors.left: table_view.left
        anchors.right: table_view.right
        anchors.bottom: table_view.bottom
        model: 20
        interactive: false
        clip: true
        z:-1

        delegate: Rectangle {
            width: placeholder_rows.width
            height: rowHeight(0)
            color: index % 2 === 0 ? "#232627" : "#292D2E"
        }
    }

    HorizontalHeaderView {
        visible: SearchViewModel.firstSearchComplete
        id: header_view
        anchors.top: title_header.bottom
        anchors.topMargin: 18
        anchors.left: table_view.left
        anchors.right: table_view.right
        syncView: table_view
        clip: true
        interactive: false

        delegate: ItemDelegate {
            Rectangle {
                color: "#151618"
                anchors.fill: parent
            }
            Text {
                color: "#71767E"
                font.pixelSize: 14
                anchors.fill: parent
                anchors.leftMargin: 6
                anchors.rightMargin: 6
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: index === 2 ? Text.AlignRight : Text.AlignLeft
                text: {
                    switch(index) {
                        case 0:
                           return qsTr("Name");
                        case 1:
                          return qsTr("Path");
                        case 2:
                          return qsTr("Size");
                    }
                    return "";
                }
            }
        }
    }

    TableView {
        visible: SearchViewModel.firstSearchComplete
        id: table_view
        model: SearchViewModel.listModel
        anchors.top: header_view.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: search_box.top
        anchors.leftMargin: 20
        anchors.bottomMargin: 20
        anchors.rightMargin: 20
        clip: true
        columnSpacing: 0
        rowHeightProvider: root.rowHeight

        boundsBehavior: Flickable.StopAtBounds

        columnWidthProvider: function(column) {
           const col0 = 150;
           const col2 = 70;
           switch(column) {
           case 0:
               return col0;
           case 1:
               return table_view.width - col0 - col2 - (2*rowSpacing);
           case 2:
               return col2;
           }
           return 0;
        }

        Connections {
            target: SearchViewModel.listModel
            function onModelReset() {
                table_view.contentY = 0;
            }
        }

        delegate: ItemDelegate {
            Rectangle {
                anchors.fill: parent
                color: row % 2 === 0 ? "#232627" : "#292D2E"
            }
            Text {
                color: "#9398A0"
                font.pixelSize: 13
                text: model.display
                anchors.fill: parent
                anchors.leftMargin: 6
                anchors.rightMargin: 6
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: column === 2 ? Text.AlignRight : Text.AlignLeft
                clip: true
            }
        }

        ScrollBar.vertical: ScrollBar {
            id: control
            policy: ScrollBar.AsNeeded
            minimumSize: 0.08

            contentItem: Rectangle {
                id: handle_bar
                implicitWidth: 6
                implicitHeight: 10
                radius: width/2
                opacity: control.active ? 0.8 : 0.0
                color: control.pressed ? "#121213" : "#151618" // TODO: sort these colors out and add animation

                Behavior on opacity {
                    NumberAnimation {
                        duration: 200
                    }
                }
            }

            background: Rectangle {
                            anchors.fill: handle_bar
                            visible: false
                        }
        }

    }

    Textbox {
        id: search_box
        text: SearchViewModel.searchTerm
        is_invalid: !SearchViewModel.isSearchTermValid
        height: 50
        state: SearchViewModel.firstSearchComplete ? "afterFirstSearch" : "beforeFirstSearch"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 75
        anchors.rightMargin: anchors.leftMargin
        icon_source: "qrc:/img/search_field.svg"
        icon_sourceWidth: 25
        enabled: !SearchViewModel.isSearching

        onEnterPressed: {
            SearchViewModel.startSearch();
        }

        onTextChanged: {
            SearchViewModel.searchTerm = text;
        }


        states: [
                State {
                    name: "afterFirstSearch"
                    PropertyChanges {
                        target: search_box.anchors
                        leftMargin: 20
                        bottomMargin: 18
                    }
                    AnchorChanges {
                        target: search_box
                        anchors.right: stopStartButton.left
                        anchors.verticalCenter: undefined
                        anchors.bottom: root.bottom
                    }

                }
            ]

        transitions: Transition {
            AnchorAnimation { duration: 200 }
            PropertyAnimation {
                properties: "leftMargin,rightMargin,topMargin,bottomMargin"
                duration: 200
            }
        }
    }

    AppButton {
        id: stopStartButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.leftMargin: 10
        width: 118
        height: 48
        buttonColor: SearchViewModel.isSearching ? "#8A1313" : defaultButtonColor
        hoverColor: SearchViewModel.isSearching ? "#B31818" : defaultHoverColor
        buttonText: SearchViewModel.isSearching ? "Stop" : "Search"
        opacity: SearchViewModel.firstSearchComplete ? 1 : 0;
        font.pixelSize: 16

        Behavior on opacity {
            OpacityAnimator {
                duration: 200
            }
        }

        onButtonClicked: {
            if (SearchViewModel.isSearching) {
                SearchViewModel.stop()
            } else {
                SearchViewModel.startSearch();
            }
        }
    }

    Component.onCompleted: {
        search_box.focus = true;
    }
}

