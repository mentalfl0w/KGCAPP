import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import RibbonUI
import KGCAPP

// Custom your TabBar here
RibbonTabBar {
    id: control
    property var main_view
    RibbonTabPage{
        id: gen_key_page
        title: qsTr("Key")
        RibbonTabGroup{
            width: key_property_layout.width + 30
            text: qsTr("Key Properties")
            RowLayout{
                id: key_property_layout
                anchors.centerIn: parent
                height: parent.height
                spacing: 10
                ColumnLayout{
                    Layout.preferredHeight: parent.height
                    RowLayout{
                        RibbonText{
                            text: 'q: '
                        }
                        RibbonLineEdit{
                            id: q
                            Layout.fillWidth: true
                            placeholderText: 'modulus of a finite field'
                        }
                    }
                    RowLayout{
                        RibbonText{
                            text: 'a: '
                        }
                        RibbonLineEdit{
                            id: a
                            Layout.fillWidth: true
                            placeholderText: 'curvilinear coefficient'
                        }
                    }
                    RowLayout{
                        RibbonText{
                            text: 'b: '
                        }
                        RibbonLineEdit{
                            id: b
                            Layout.fillWidth: true
                            placeholderText: 'curvilinear coefficient'
                        }
                    }
                }
                ColumnLayout{
                    Layout.preferredHeight: parent.height
                    RowLayout{
                        RibbonText{
                            text: 'P_x: '
                        }
                        RibbonLineEdit{
                            id: p_x
                            Layout.fillWidth: true
                            placeholderText: 'base point'
                        }
                    }
                    RowLayout{
                        RibbonText{
                            text: 'P_y: '
                        }
                        RibbonLineEdit{
                            id: p_y
                            Layout.fillWidth: true
                            placeholderText: 'base point'
                        }
                    }
                    RowLayout{
                        RibbonText{
                            text: 'O_P: '
                        }
                        RibbonLineEdit{
                            id: o_p
                            Layout.fillWidth: true
                            placeholderText: 'the order of P'
                        }
                    }
                }
            }
        }
        RibbonTabGroup{
            width: key_property_para_action_layout.width + 30
            text: qsTr("Parameter Actions")

            ColumnLayout{
                id: key_property_para_action_layout
                anchors.centerIn: parent
                height: parent.height
                RowLayout{
                    RibbonText{
                        text: 'ID: '
                    }
                    RibbonLineEdit{
                        id: id
                        Layout.preferredWidth: 100
                        placeholderText: 'parameter ID'
                    }
                }
                RibbonButton{
                    id: key_property_save_btn
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    icon_source: RibbonIcons.Save
                    show_tooltip: false
                    text: qsTr("Save")
                    onClicked: {
                        KeyDataManager.q = q.text
                        KeyDataManager.a = a.text
                        KeyDataManager.b = b.text
                        KeyDataManager.P_x = p_x.text
                        KeyDataManager.P_y = p_y.text
                        KeyDataManager.O_P = o_p.text
                        KeyDataManager.ID = id.text
                        KeyDataManager.need_save_key_properties()
                        main_view.view.currentIndex = 1
                    }
                }
                RibbonButton{
                    id: key_property_reset_btn
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    icon_source: RibbonIcons.ArrowReset
                    show_tooltip: false
                    text: qsTr("Use Default")
                    onClicked: {
                        KeyDataManager.need_reset_key_properties()
                        q.text = KeyDataManager.q
                        a.text = KeyDataManager.a
                        b.text = KeyDataManager.b
                        p_x.text = KeyDataManager.P_x
                        p_y.text = KeyDataManager.P_y
                        o_p.text = KeyDataManager.O_P
                    }
                }
            }
        }
        RibbonTabGroup{
            show_border: false
            width: key_property_save_action_layout.width + 30
            text: qsTr("Other Actions")
            ColumnLayout{
                id: key_property_save_action_layout
                anchors.centerIn: parent
                height: parent.height
                RibbonButton{
                    id: key_property_save_database_btn
                    Layout.alignment: Qt.AlignHCenter
                    icon_source: RibbonIcons.DatabaseArrowDown
                    show_tooltip: false
                    text: qsTr("Save to Database")
                    onClicked: {
                        KeyDataManager.q = q.text
                        KeyDataManager.a = a.text
                        KeyDataManager.b = b.text
                        KeyDataManager.P_x = p_x.text
                        KeyDataManager.P_y = p_y.text
                        KeyDataManager.O_P = o_p.text
                        KeyDataManager.ID = id.text
                        KeyDataManager.need_save_key_to_database()
                    }
                }
                RibbonButton{
                    id: key_property_save_file_btn
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    icon_source: RibbonIcons.DocumentSave
                    show_tooltip: false
                    text: qsTr("Save to file")
                    onClicked: {
                        KeyDataManager.need_save_key_to_file()
                    }
                }
                RibbonButton{
                    id: key_property_search_key_btn
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    icon_source: RibbonIcons.Search
                    show_tooltip: true
                    tip_text: qsTr("To search key, you need input the ID")
                    text: qsTr("Search Key")
                    onClicked: {
                        KeyDataManager.ID = id.text
                        KeyDataManager.need_search_key()
                        main_view.view.currentIndex = 2
                    }
                }
            }
        }
    }
    RibbonTabPage{
        title: qsTr("Others")
        RibbonTabGroup{
            width: theme_layout.width + 30
            text: qsTr("Theme")
            RowLayout{
                id: theme_layout
                anchors.centerIn: parent
                height: parent.height
                spacing: 10
                RibbonComboBox{
                    id: theme_combo
                    model: ListModel {
                        id: model_theme
                        ListElement { text: "Light" }
                        ListElement { text: "Dark" }
                        ListElement { text: "System" }
                    }
                    icon_source: RibbonIcons.DarkTheme
                    Component.onCompleted: update_state()
                    onActivated: {
                        if (currentText === "System")
                            RibbonTheme.theme_mode = RibbonThemeType.System
                        else if (currentText === "Light")
                            RibbonTheme.theme_mode = RibbonThemeType.Light
                        else
                            RibbonTheme.theme_mode = RibbonThemeType.Dark
                    }
                    Connections{
                        target: RibbonTheme
                        function onTheme_modeChanged(){
                            theme_combo.update_state()
                        }
                    }
                    function update_state(){
                        let str = (RibbonTheme.theme_mode === RibbonThemeType.System ? "System" : RibbonTheme.theme_mode === RibbonThemeType.Light ? "Light" : "Dark")
                        currentIndex = find(str)
                    }
                }
            }
        }
    }
}
