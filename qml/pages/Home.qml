import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import RibbonUI

Item {
    height: content.height + content.anchors.margins * 2

    ColumnLayout{
        id: content
        anchors{
            centerIn: parent
            margins: 10
        }
        spacing: 5
        Image {
            source: `qrc:/qt/qml/${KGCAppUri}/resources/imgs/icon.png`
            fillMode:Image.PreserveAspectFit
            Layout.preferredHeight: 200
            Layout.preferredWidth: height
            Layout.alignment: Qt.AlignHCenter
            layer.enabled: true
            layer.effect: RibbonShadow{}
        }
        RibbonText{
            Layout.alignment: Qt.AlignHCenter
            text: KGCAppUri
            font.pixelSize: 20
        }
        RibbonText{
            Layout.alignment: Qt.AlignHCenter
            text: `©${new Date().getFullYear()} WuXinYu`
            font.pixelSize: 14
        }
        RibbonText{
            Layout.alignment: Qt.AlignHCenter
            text: `Version: V${KGCAppVersion}`
            font.pixelSize: 14
        }
    }
}
