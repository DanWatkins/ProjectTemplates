import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

import BaseQtOpenGL 1.0

Item {
    id: appWindow
    width: 800
    height: 600


    Camera {
        id: mainCamera
        objectName: "BaseQtOpenGL_mainCamera"
		anchors.fill: parent

        //we have to flip the image vertically since the FBO is upside down
        transform: Scale { origin.y: height/2; yScale: -1.0 }

        onIsLoadedChanged: {
            menuButtons.visible = true;
        }
    }


    Item {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.65
        }

        RowLayout {
            id: menuButtons
            x: 10
            y: 10
            spacing: 10

            width: Layout.minimumWidth
            height: Layout.minimumHeight

            Button {
                id: button_captureCursor
                text: "Capture Cursor (F1)"
                height: 30
                onClicked: {
                    terrain3D.toggleCaptureCursor();
                    settingsPane.item.refreshIfNeeded();
                }
            }
            Button {
                id: button_restart
                text: "Restart (F5)"
                height: 30
                onClicked: terrain3D.requestRestart();
            }
            Button {
                id: button_fullscreen
                text: "Fullscreen (F11)"
                height: 30
                onClicked: {
                    terrain3D.toggleFullscreen();
                    settingsPane.item.refreshIfNeeded();
                }
            }
            Button {
                id: button_exit
                text: "Exit (Esc)"
                height: 30
                onClicked: terrain3D.close();
            }

            Text {
                text: "FPS: " + terrain3D.fps;
                font.pointSize: 16
                color: "white"
            }
        }
    }
}