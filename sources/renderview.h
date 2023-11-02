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
#include <QTextureImage>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QTexture>

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
    Qt3DRender::QTextureImage *textureImage;

signals:
};

#endif // RENDERVIEW_H
