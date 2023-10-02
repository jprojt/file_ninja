import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CppTypes 1.0


GridView {
    id: grid
    implicitWidth: 500
    implicitHeight: 400
    model: AnalyseViewModel.waffleListModel
    cellWidth: (width-0.1) / AnalyseViewModel.waffleListModel.size
    cellHeight: (height-0.1) / AnalyseViewModel.waffleListModel.size
    interactive: false
    property int selected_id: -999
//    flow: GridView.FlowLeftToRight
//    layoutDirection: GridView.RightToLeft

    delegate: Rectangle {
        id: cell
        width: grid.cellWidth
        height: grid.cellHeight
        border.color: "#232627"
        border.width: 1
        radius: grid.cellWidth / 2
        color: {
            if (model.groupId === -1) {
                return "black";
            }

            const colours = ["#6242D2", "#E562E0", "#62E57D", "#D28642", "#B3B53B", "#E56262", "#6299E5", "#498A6D"];
            return colours[model.groupId % colours.length];
        }
        opacity: grid.selected_id === -999 ? 0.6 : grid.selected_id !== model.id ? 0.2 : 1




        MouseArea {
            id: mouse_area
            anchors.fill:  parent
            hoverEnabled: true

            Component.onCompleted: {
                var tool_tip_component = Qt.createComponent("qrc:///AnalyseTooltip.qml");
                var tool_tip;

                if (tool_tip_component.status === Component.Ready) {
                    mouse_area.onEntered.connect(function() {
                        tool_tip = tool_tip_component.createObject(
                            cell,
                            {
                                "target": mouse_area,
                                "visible": true,
                                "model_data": model
                            }
                        );
                    });

                    mouse_area.onExited.connect(function() {
                        tool_tip.visible = false;
                        tool_tip.destroy();
                    });
                }
            }

            onEntered: {
                grid.selected_id = model.id;

                //tooltip is dynamic for performance

            }

            onExited: {
                grid.selected_id = -999;

                //tooltip is dynamic for performance
            }
        }

        Behavior on color {
            ColorAnimation {
                duration: 150
            }
        }
    }

}
