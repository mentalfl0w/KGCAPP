import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import RibbonUI
import KGCAPP

Item {
    height: content.height + content.anchors.margins * 2
    RibbonButton{
        anchors{
            topMargin: 5
            top: parent.top
            left: parent.left
            leftMargin: content.anchors.margins
        }
        icon_source: RibbonIcons.ArrowReply
        show_bg: false
        text: qsTr('Return to Home')
        onClicked: view.currentIndex = 0
        show_tooltip: false
    }

    ColumnLayout{
        id: content
        anchors{
            centerIn: parent
            margins: 10
        }
        spacing: 10
        RibbonText{
            id: title
            text: qsTr('Key Generate Result')
            font.pixelSize: 20
            Layout.alignment: Qt.AlignHCenter
        }
        ColumnLayout{
            RibbonText{
                text: qsTr('Your secret value:')
            }
            RowLayout{
                Layout.alignment: Qt.AlignHCenter
                RibbonText{
                    text: 'x: '
                }
                RibbonLineEdit{
                    placeholderText: 'your secret value'
                    readOnly: true
                    Layout.fillWidth: true
                    text: KeyDataManager.x
                    RibbonToolTip{
                        text: parent.placeholderText
                        visible: parent.hovered
                    }
                }
            }
        }
        ColumnLayout{
            RibbonText{
                text: qsTr('Your public key:')
            }
            RowLayout{
                RibbonText{
                    text: 'Q_x: '
                }
                RibbonLineEdit{
                    placeholderText: 'point Q_x'
                    readOnly: true
                    Layout.fillWidth: true
                    text: KeyDataManager.Q_x
                    RibbonToolTip{
                        text: parent.placeholderText
                        visible: parent.hovered
                    }
                }
            }
            RowLayout{
                RibbonText{
                    text: 'Q_y: '
                }
                RibbonLineEdit{
                    placeholderText: 'point Q_y'
                    readOnly: true
                    Layout.fillWidth: true
                    text: KeyDataManager.Q_y
                    RibbonToolTip{
                        text: parent.placeholderText
                        visible: parent.hovered
                    }
                }
            }
        }
        ColumnLayout{
            RibbonText{
                text: qsTr('Your private key:')
            }
            RowLayout{
                RibbonText{
                    text: 'S: '
                }
                RibbonLineEdit{
                    placeholderText: 'your private key'
                    readOnly: true
                    Layout.fillWidth: true
                    text: KeyDataManager.S
                    RibbonToolTip{
                        text: parent.placeholderText
                        visible: parent.hovered
                    }
                }
            }
        }
        ColumnLayout{
            RibbonText{
                text: qsTr('Your KGC public key:')
            }
            RowLayout{
                RibbonText{
                    text: 'Pk_x: '
                }
                RibbonLineEdit{
                    placeholderText: 'point Pk_x'
                    readOnly: true
                    Layout.fillWidth: true
                    text: KeyDataManager.Pk_x
                    RibbonToolTip{
                        text: parent.placeholderText
                        visible: parent.hovered
                    }
                }
            }
            RowLayout{
                RibbonText{
                    text: 'Pk_y: '
                }
                RibbonLineEdit{
                    placeholderText: 'point Pk_y'
                    readOnly: true
                    Layout.preferredWidth: Math.max(contentWidth + 10, title.contentWidth * 2)
                    text: KeyDataManager.Pk_y
                    RibbonToolTip{
                        text: parent.placeholderText
                        visible: parent.hovered
                    }
                }
            }
        }
    }
}
