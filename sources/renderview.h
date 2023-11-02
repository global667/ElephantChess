#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <QFirstPersonCameraController>
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
#include <QPaintedTextureImage>
#include <QTextureImage>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QTexture>

#include <QPainter>

#include "basemodel.h"

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
    Qt3DExtras::QTextureMaterial *torusMaterial;
    Qt3DExtras::QFirstPersonCameraController *camController;
    Qt3DCore::QTransform *lightTransform;
    Qt3DRender::QPointLight *light;
    Qt3DRender::QTextureRectangle *texture;
    Qt3DRender::QPaintedTextureImage *textureImage;

signals:
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
};

#endif // RENDERVIEW_H
