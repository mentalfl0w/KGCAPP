import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import RibbonUI
import "components"

RibbonWindow {
    id:root
    width: title_bar.minimumWidth * 1.5
    height: title_bar.minimumWidth * 1.2
    minimumHeight: title_bar.minimumWidth * 1.2
    maximumHeight: title_bar.minimumWidth * 1.5
    minimumWidth: title_bar.minimumWidth * 1.5
    maximumWidth: title_bar.minimumWidth * 1.8
    title: qsTr(KGCAppUri)
    comfirmed_quit: true

    // Title bar's right content
    title_bar.right_content:RowLayout{
        spacing: 1
        layoutDirection: Qt.RightToLeft
        RibbonButton{
            show_bg:false
            icon_source: RibbonIcons.Info
            icon_source_filled: RibbonIcons_Filled.Info
            tip_text: qsTr("About")
            hover_color: Qt.rgba(0,0,0, 0.3)
            pressed_color: Qt.rgba(0,0,0, 0.4)
            text_color: title_bar.title_text_color
            text_color_reverse: false
            onClicked: root.show_window(Qt.resolvedUrl("about.qml"))
        }
    }


    // TabBar
    TabBar{
        id: tab_bar
        main_view: main_view
    }

    // MainView is behind TabBar
    MainView{
        id: main_view
        top_padding: tab_bar.height
        bottom_padding: bottom_bar.height
    }

    // BottomBar
    BottomBar{
        id: bottom_bar
    }

    Component.onCompleted: RibbonTheme.modern_style = true
}
