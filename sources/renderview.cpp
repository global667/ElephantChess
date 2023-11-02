#include "renderview.h"
#include "qtexturematerial.h"

RenderView::RenderView()
{
    // Root entity
    rootEntity = new Qt3DCore::QEntity();

    // Camera
    cameraEntity = camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    lightEntity = new Qt3DCore::QEntity(rootEntity);
    light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // For camera controls
    camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    // Scenemodifier
    //SceneModifier *modifier = new SceneModifier(rootEntity);

    // Torus shape data
    //! [0]
    m_torus = new Qt3DExtras::QCuboidMesh();
    //m_torus->setRadius(1.0f);
    //m_torus->setMinorRadius(0.4f);
    //m_torus->setRings(100);
    //m_torus->setSlices(20);
    //! [0]

    // TorusMesh Transform
    //! [1]
    torusTransform = new Qt3DCore::QTransform();
    torusTransform->setScale(5.8f);
    //torusTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 25.0f));
    //torusTransform->setTranslation(QVector3D(5.0f, 4.0f, 0.0f));
    torusTransform->setTranslation(QVector3D(0.0f, 0.0f, 10.0f));

    //! [1]

    //! [2]
    //torusMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    torusMaterial = new Qt3DExtras::QTextureMaterial();
    texture = new Qt3DRender::QTextureRectangle();
    qDebug() << texture;
    if (!texture)
        qDebug() << "Error with 'texture'";
    QUrl url = QUrl::fromLocalFile("/home/wsk/OIP.jpg");
    qDebug() << url.isValid();
    if (!url.isValid())
        qDebug() << "Error in url";
    textureImage = new Qt3DRender::QTextureImage();
    textureImage->setSource(url);
    qDebug() << textureImage;
    if (textureImage->status() == Qt3DRender::QTextureImage::Error)
        qDebug() << "Error in 'textreImage'";
    texture->addTextureImage(textureImage);
    torusMaterial->setTexture(texture);
    //torusMaterial->setDiffuse(QColor(QRgb(0xfcaf3e)));
    //! [2]

    //{
    // Torus
    //! [3]
    m_torusEntity = new Qt3DCore::QEntity(rootEntity); //m_rootEntity);
    m_torusEntity->addComponent(m_torus);
    m_torusEntity->addComponent(torusMaterial);
    m_torusEntity->addComponent(torusTransform);
    //! [3]
    /*   //}
*/
    // Set root object of the scene
    setRootEntity(rootEntity);
}
