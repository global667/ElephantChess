#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <QFirstPersonCameraController>
#include <QQuaternion>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QPointLight>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/QForwardRenderer>
//#include <Qt3DExtras/QPhongMaterial>
#include <QTextureMaterial>
#include <Qt3DExtras/QSphereMesh>
//#include <Qt3DExtras/QTorusMesh>
#include <QCylinderMesh>
#include <QDiffuseSpecularMaterial>
#include <QObjectPicker>
#include <QPaintedTextureImage>
#include <QPainter>
#include <QPickEvent>
#include <QPickingSettings>
#include <QSceneLoader>
#include <QTextureImage>
#include <QTextureWrapMode>
#include <QVector2D>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QTexture>
#include "qtexturematerial.h"

#include <QAttribute>
#include <QDirectionalLight>

#include "basemodel.h"
#include "qmesh.h"

class RenderView : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    explicit RenderView();

    Qt3DCore::QEntity *rootEntity;
    Qt3DRender::QCamera *cameraEntity;
    Qt3DCore::QEntity *lightEntity;
    Qt3DExtras::QCuboidMesh *m_torus;
    Qt3DCore::QEntity *m_torusEntity;
    Qt3DCore::QTransform *torusTransform;
    Qt3DCore::QTransform *cylinderTransform;
    Qt3DExtras::QTextureMaterial *torusMaterial;
    Qt3DExtras::QFirstPersonCameraController *camController;
    Qt3DCore::QTransform *lightTransform;
    Qt3DRender::QDirectionalLight *dirlight;
    Qt3DRender::QTexture2D *texture;
    Qt3DRender::QTextureImage *textureImage;

    Qt3DRender::QMesh *pieceCylinderMesh;
    Qt3DCore::QEntity *boardSurfaceEntity, *boardEntity;
    Qt3DRender::QTextureWrapMode *textureWrapMode;

    Qt3DRender::QPickingSettings PickingSettings;
    Qt3DRender::QObjectPicker *objectPicker;

    Qt3DRender::QSceneLoader *loader;

    float transl = 0.0;
    QStringList entityList;

public slots:
    void clicked(Qt3DRender::QPickEvent *pick);

protected:
    //    void mousePressEvent(QMouseEvent *ev) override;

signals:
private:
    Qt3DCore::QTransform *camtrans;
    Qt3DRender::QMesh *makeMesh(QString name);
    Qt3DExtras::QTextureMaterial *makeTex(QString name);
};

class PaintedTextureImage : public Qt3DRender::QPaintedTextureImage
{
    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den roten Figuren gehört.
    QString redRiver = QString(
        "\u695a"
        "\u6cb3"); //"楚 河"; // (Chǔ hé) - Dieser Schriftzug bedeutet "Fluss von Chu".

    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den schwarzen Figuren gehört.
    QString blackRiver = QString(
        "\u6f22"
        "\u754c"); //"漢 界"; //  (Hàn jiè) - Dieser Schriftzug bedeutet "Grenze von Han".
    QPixmap pix;

public:
    PaintedTextureImage()
    {
        setHeight(900);
        setWidth(900);
    };

protected:
    void paint(QPainter *p);

private:
    void paintBoard(QPainter *p);
    void PaintPieces(QPainter *p);
    QPixmap *PrepareNativePiece(QPainter *p, int row, int col);
};

class LoadScene : public QObject
{
    Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();

public:
    LoadScene()
    {
        mesh->setSource(QUrl(QUrl::fromLocalFile("/home/wsk/ElephantChess/res/chinese-chess.obj")));
    };

    Qt3DRender::QMesh *makeMesh(QString name)
    {
        mesh->setMeshName(name);

        return mesh;
    }

    Qt3DExtras::QTextureMaterial *makeTex(QString name)
    {
        Qt3DRender::QTextureImage *textureImage = new Qt3DRender::QTextureImage();
        textureImage->setSource(QUrl(QUrl::fromLocalFile(
            "/home/wsk/ElephantChess/res/"
            + name))); // Xiangqi_Advisor_(Trad)+ wooden.png"))); //wooden.png")));
        Qt3DRender::QTexture2D *texture = new Qt3DRender::QTexture2D();
        texture->addTextureImage(textureImage);
        Qt3DExtras::QTextureMaterial *material = new Qt3DExtras::QTextureMaterial();
        material->setTexture(texture);
        return material;
    }
};

#endif // RENDERVIEW_H
