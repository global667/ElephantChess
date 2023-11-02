#include "renderview.h"
#include "qtexturematerial.h"

extern BaseModel basemodel;

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
    //    QUrl url = QUrl::fromLocalFile("/home/wsk/OIP.jpg");
    //    qDebug() << url.isValid();
    //    if (!url.isValid())
    //        qDebug() << "Error in url";
    textureImage = new PaintedTextureImage();
    //textureImage->setSource(url);
    qDebug() << textureImage;
    //if (textureImage->status() == Qt3DRender::QTextureImage::Error)
    //    qDebug() << "Error in 'textreImage'";
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

void PaintedTextureImage::paint(QPainter *p)
{
    paintBoard(p);
}

void PaintedTextureImage::paintBoard(QPainter *p)
{
    Q_ASSERT(p);

    const QColor background(252, 175, 62);
    const QColor sides(206, 92, 0);
    const QColor river("#3A438F");

    p->setViewport(0, height(), width(), -height());
    p->fillRect(p->viewport(), background);

    QPen pn(Qt::black);
    pn.setWidth(2);
    p->setPen(pn);

    // Palaeste
    p->drawLine(50 + (3 * (width() - 2 * 50) / BaseModel::BoardColPoints),
                50 + 0 * (height() - 50 - 100) / BaseModel::BoardRowPoints,
                50 + (5 * (width() - 2 * 50) / BaseModel::BoardColPoints),
                50 + 2 * (height() - 50 - 100) / BaseModel::BoardRowPoints);

    p->drawLine(50 + (3 * (width() - 2 * 50) / BaseModel::BoardColPoints),
                50 + 2 * (height() - 50 - 100) / BaseModel::BoardRowPoints,
                50 + (5 * (width() - 2 * 50) / BaseModel::BoardColPoints),
                50 + 0 * (height() - 50 - 100) / BaseModel::BoardRowPoints);

    p->drawLine(50 + (3 * (width() - 2 * 50) / BaseModel::BoardColPoints),
                50 + 9 * (height() - 50 - 100) / BaseModel::BoardRowPoints,
                50 + (5 * (width() - 2 * 50) / BaseModel::BoardColPoints),
                50 + 7 * (height() - 50 - 100) / BaseModel::BoardRowPoints);

    p->drawLine(50 + (3 * (width() - 2 * 50) / BaseModel::BoardColPoints),
                50 + 7 * (height() - 50 - 100) / BaseModel::BoardRowPoints,
                50 + (5 * (width() - 2 * 50) / BaseModel::BoardColPoints),
                50 + 9 * (height() - 50 - 100) / BaseModel::BoardRowPoints);

    // Vertikale Linien
    for (int i = 0; i <= BaseModel::BoardColPoints; i++) {
        p->drawLine(50 + (i * (width() - 2 * 50) / BaseModel::BoardColPoints),
                    50,
                    50 + (i * (width() - 2 * 50) / BaseModel::BoardColPoints),
                    height() - 100);
    }

    // Horizontale Linien
    for (int i = 0; i <= BaseModel::BoardRowPoints; i++) {
        p->drawLine(50,
                    50 + i * (height() - 50 - 100) / BaseModel::BoardRowPoints,
                    width() - 50,
                    50 + i * (height() - 50 - 100) / BaseModel::BoardRowPoints);
    }

    // TODO: Hervorgehoben Punkte von Soldaten und Kanonen implementieren

    // Oberer Rand
    p->fillRect(0, 0, width(), 50, sides);

    // Fluss
    p->fillRect(0,
                50 + 4 * (height() - 50 - 100) / BaseModel::BoardRowPoints,
                width(),
                (height() - 50 - 100) / BaseModel::BoardRowPoints,
                river); //background);

    // Flussufer
    QFont tmp = QFont(p->font());
    QFont font = QFont();
    font.setPointSize(30);
    font.setBold(false);
    font.setItalic(true);
    p->setFont(font);
    p->drawText(QRect(100,
                      5 * (height() - 50 - 100) / BaseModel::BoardRowPoints + 10,
                      150, //width(),
                      (height() - 50 - 100) / BaseModel::BoardRowPoints / 2),
                Qt::AlignCenter,
                redRiver);
    p->drawText(QRect(600,
                      5 * (height() - 50 - 100) / BaseModel::BoardRowPoints - 20,
                      150, //width(),
                      (height() - 50 - 100) / BaseModel::BoardRowPoints / 2),
                Qt::AlignCenter,
                blackRiver);

    p->setFont(tmp);

    // Seitenraender
    // Linker Rand
    p->fillRect(0, 0, 50, height(), sides);
    // Unterer Rand
    p->fillRect(0, height() - 2 * 50, width(), 2 * 50, sides);
    // Rechter Rand
    p->fillRect(width() - 50, 0, 50, height(), sides);

    p->setPen(QColor(0, 0, 0));

    // Notation
    if (0 == 0) {
        // Westliche, an Schach angelehnte Notation

        // Vertikaler Text
        for (int i = 0; i <= BaseModel::BoardRowPoints; i++) {
            p->drawText(50 / 2,
                        50 + i * (height() - 50 - 100) / BaseModel::BoardRowPoints,
                        //width() - 50,
                        //50 + i * (height() - 50 - 100) / cutp_height,
                        //0,
                        QString("%1").arg(QChar('9' - i)));
        }
        // Horizontaler Text
        for (int i = 0; i <= BaseModel::BoardColPoints; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / BaseModel::BoardColPoints),
                        height() - (2 * 50 / 2),
                        //50 + (i * (width() - 2 * 50) / cutp_width),
                        //height() - 100,
                        //0,
                        QString("%1").arg(QChar('a' + i)));
        }
    } else {
        // TODO: Traditionelle Notation, wird in späteren Versionen implementiert

        // Horizontaler Text ,unten
        QStringList list;
        list << "\u4e00"  //"一"
             << "\u4e8c"  //"二"
             << "\u4e09"  //"三"
             << "\u56db"  //"四"
             << "\u4e94"  //"五"
             << "\u516d"  //"六"
             << "\u4e03"  //"七"
             << "\u516b"  //"八"
             << "\u4e5d"; //"九";

        for (int i = 0; i <= BaseModel::BoardColPoints; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / BaseModel::BoardColPoints),
                        height() - (2 * 50 / 2),
                        QString("%1").arg(list.at(8 - i)));
        }

        // Horizontaler Text, oben
        for (int i = 0; i <= BaseModel::BoardColPoints; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / BaseModel::BoardColPoints),
                        10,
                        QString("%1").arg(QChar('1' + i)));
        }
    }

    // Aussenraender
    pn.setWidth(3);
    p->setPen(pn);
    p->drawRect(46, 46, width() - 93, height() - 143);

    pn.setWidth(2);
    p->setPen(pn);
    p->drawRect(50, 50, width() - 100, height() - 150);

    pn.setWidth(2);
    p->setPen(pn);
}