import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import RibbonUI

// Custom your MainView here
RibbonView{
    id: control
    anchors.fill: parent
    property alias view: view
    property var pages: [`../pages/Home.qml`,`../pages/GenKey.qml`,`../pages/SearchKey.qml`]
    SwipeView{
        id: view
        interactive: false
        Layout.fillHeight: true
        Layout.fillWidth: true
        spacing: 0
        Repeater{
            model: control.pages
            Loader {
                active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                source: modelData
            }
        }
    }
}
