// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick 2.15
import QtQuick.Controls
import QtQuick3D

import QtQuick.Window 2.15
import Qt3D.Core 2.15
import Qt3D.Render 2.7
import Qt3D.Extras 2.15
import Qt3D.Input 2.15
import Qt3D.Logic 2.15
import Qt3D.Animation 2.15
import QtQuick.Scene3D 2.7


Rectangle {
    id: view
    visible: true
    anchors.fill: parent

    Scene3D {
        anchors.fill: parent
        focus: true
        id: scene3d
        hoverEnabled: true


        Entity {
            id: rootEntity

            // Render settings
                        RenderSettings {
                            activeFrameGraph: ForwardRenderer {
                                camera: camera
                                clearColor: "black"
                            }
                        }

            // Camera
            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 60
                aspectRatio: width / height
                nearPlane: 0.1
                farPlane: 1000
                position: Qt.vector3d(0, 50, 100)
                upVector: Qt.vector3d(0, 1, 0)
                viewCenter: Qt.vector3d(0, 0, 0)
            }

            // Camera controller
            OrbitCameraController {
                camera: camera
                linearSpeed: 50
                lookSpeed: 180
            }

            // Light
            DirectionalLight {
                intensity: 1
                worldDirection: Qt.vector3d(0, -1, -1)
            }



            // SceneLoader to load the model
            SceneLoader {
                id: sceneLoader
                source: "qrc:/res/3dBoard/board.dae"
                onStatusChanged: {
                    if (sceneLoader.status === SceneLoader.Ready) {
                        console.log("Model loaded successfully!")
                        view.setupObjectPickers(rootEntity);
                    }
                    if (sceneLoader.status === SceneLoader.Error)
                        console.error("Error loading model: " + sceneLoader.errorString());
                }
            }

            components: [sceneLoader]

        }


    }
    // Function to setup object pickers recursively
    function setupObjectPickers(entity) {
        if (entity.components) {
            var picker = new ObjectPicker()
            picker.hoverEnabled = true
            picker.pressed.connect(function (pick) {
                console.log("Object picked:", pick.entity.objectName)
            })
            entity.addComponent(picker)
        }

        // Check if entity.children is defined and has elements before looping
            if (entity.children && entity.children.length > 0) {
                for (var i = 0; i < entity.children.length; ++i) {
                    setupObjectPickers(entity.children[i]);
                }
            } else {
                console.log("No children found for entity", entity.objectName);
            }
    }

    // Mouse Area for 2D interactions
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Mouse clicked at:", mouse.x, mouse.y)
                    }
                }

/*

    MouseArea {
        anchors.fill: view

        onClicked: (mouse) => {
            // Get screen coordinates of the click
            pickPosition.text = "(" + mouse.x + ", " + mouse.y + ")"
            var result = view.pick(mouse.x, mouse.y);
            if (result.objectHit) {
                var pickedObject = result.objectHit;
                // Toggle the isPicked property for the model
                pickedObject.isPicked = !pickedObject.isPicked;
                // Get picked model name
                pickName.text = pickedObject.objectName;
                // Get other pick specifics

            } else {
                pickName.text = "None";
                uvPosition.text = "";
                distance.text = "";
                scenePosition.text = "";
                localPosition.text = "";
                worldNormal.text = "";
                localNormal.text = "";
            }
        }
    }
    */
}
