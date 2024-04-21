#include "renderview.h"

extern BaseModel basemodel;

RenderView::RenderView()
{
	// Root entity
	rootEntity = new Qt3DCore::QEntity();

	//auto renderSettings = new Qt3DRender::QRenderSettings(rootEntity);

	//    renderSettings->pickingSettings()->setPickMethod(
	//        Qt3DRender::QPickingSettings::BoundingVolumePicking);
	//    renderSettings->pickingSettings()->setFaceOrientationPickingMode(
	//        Qt3DRender::QPickingSettings::FrontAndBackFace);
	//    renderSettings->pickingSettings()->setPickResultMode(
	//        Qt3DRender::QPickingSettings::NearestPriorityPick);
	//    renderSettings->pickingSettings()->setWorldSpaceTolerance(10.0f);
	//    qDebug() << PickingSettings.worldSpaceTolerance();
	//frameGraphNode = new Qt3DRender::QFrameGraphNode;
	//renderSettings->setActiveFrameGraph(rootEntity);
	//rootEntity->addComponent(renderSettings);

	// Camera
	cameraEntity = camera();

	cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	cameraEntity->setPosition(QVector3D(150, 150, 50.0f));
	cameraEntity->setUpVector(QVector3D(0, 1, 0));
	cameraEntity->setViewCenter(QVector3D(0, 0, 0));

	lightEntity = new Qt3DCore::QEntity(rootEntity);
	dirlight = new Qt3DRender::QDirectionalLight(lightEntity);
	dirlight->setColor("white");
	dirlight->setIntensity(1);
	lightEntity->addComponent(dirlight);
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
	//m_torus = new Qt3DExtras::QCuboidMesh();
	//pieceCylinderMesh = makeMesh("QiPan_13_-_Default_0");

	/*new Qt3DRender::QMesh();
	pieceCylinderMesh->setMeshName("QiPan_13_-_Default_0");
	pieceCylinderMesh->setSource(
		QUrl(QUrl::fromLocalFile("/home/wsk/ElephantChess/res/chinese-chess.obj")));*/

		//qDebug() << pieceCylinderMesh;

		//pieceCylinderMesh->setSlices(400);
		//m_torus->setRadius(1.0f);
		//m_torus->setMinorRadius(0.4f);
		//m_torus->setRings(100);
		//m_torus->setSlices(20);
		//! [0]

		// TorusMesh Transform
		//! [1]
		//    torusTransform = new Qt3DCore::QTransform();
		//    torusTransform->setScale(5.8f);
		//    //torusTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 25.0f));
		//    //torusTransform->setTranslation(QVector3D(5.0f, 4.0f, 0.0f));

		//    cylinderTransform = new Qt3DCore::QTransform();
		//    cylinderTransform->setScale(5.8f);
		//    cylinderTransform->setRotation(
		//        QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f));

		//    torusTransform->setTranslation(QVector3D(0.0f, 0.0f, 10.0f));
		/*        torusMaterial = new Qt3DExtras::QTextureMaterial();
		texture = new Qt3DRender::QTexture2D(); //QTextureRectangle();
		//    textureWrapMode = new Qt3DRender::QTextureWrapMode(Qt3DRender::QTextureWrapMode::ClampToEdge);
		//    textureWrapMode->x()
		//    texture->setWrapMode();

		loader = new Qt3DRender::QSceneLoader();

		//loader->setSource(QUrl(QUrl::fromLocalFile("/home/wsk/ElephantChess/res/ChineseChess.obj")));
		// m_torusEntity = loader->entity("Cube");
		textureImage = new Qt3DRender::QTextureImage();
		textureImage->setSource(QUrl(QUrl::fromLocalFile(
			"/home/wsk/ElephantChess/res/textures/Image_0.jpg"))); // Xiangqi_Advisor_(Trad)+  wooden.png"))); //wooden.png")));
		texture->addTextureImage(textureImage);
		torusMaterial->setTexture(texture);*/
		//torusMaterial->setTextureTransform(QVector3D(0.0f, 0.4f, 0.0f));

		//loader = new Qt3DRender::QSceneLoader();
		//loader->setSource(QUrl(QUrl::fromLocalFile("/home/wsk/ElephantChess/res/wooden-piece-3d.obj")));
		//m_torusEntity =
		//qDebug() << loader->entity("Cube");
		//entityList =
		//qDebug() << loader->entityNames().join("");
		//<< m_torusEntity;
		//    m_torusEntity = new Qt3DCore::QEntity(rootEntity); //m_rootEntity);
		//    m_torusEntity->addComponent(m_torus);
		//    m_torusEntity->addComponent(torusMaterial);
		//    m_torusEntity->addComponent(objectPicker);
		//    m_torusEntity->addComponent(torusTransform);

		// Erstellen eines neuen Pickers
		//    Qt3DRender::QObjectPicker *picker = new Qt3DRender::QObjectPicker(rootEntity);
		//    picker->setHoverEnabled(false);
		//    picker->setEnabled(true);
		//    picker->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
		//    picker->setDragEnabled(false);
		//    picker->setObjectName(QStringLiteral("picker"));
		//    picker->setWorldSpacePicker(true);
		//    picker->setIntersectionThreshold(0.0001f);

	// #define BYHAND
	/*
	#ifdef BYHAND
		LoadScene scene;

		board = new Qt3DExtras::QCuboidMesh();
		cuboidTransform = new Qt3DCore::QTransform();
		cuboidTransform->setScale(90);

		pieces = new QList<Qt3DExtras::QCylinderMesh *>();

		entity1 = new Qt3DCore::QEntity(rootEntity);
		entity1->addComponent(scene.makeTex("Image_0.jpg"));
		entity1->addComponent(board);
		entity1->addComponent(cuboidTransform);
		cameraEntity->viewAll();

	#elif NO_SCENELOADER
		LoadScene scene;

		//Mesh 1: Boardsurface
		objectPicker1 = new Qt3DRender::QObjectPicker();
		mesh->setMeshName("13_-_Default");
		mesh->setSource(QUrl(QUrl::fromLocalFile("/home/wsk/ElephantChess/res/chinese-chess.obj")));
		//Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh[10]();
		entity1 = new Qt3DCore::QEntity(rootEntity);
		entity1->addComponent(mesh);
		entity1->addComponent(scene.makeTex("Image_0.jpg"));
		objectPicker1->setObjectName("Mesh1");
		entity1->addComponent(objectPicker1);
		connect(objectPicker1,
				SIGNAL(pressed(Qt3DRender::QPickEvent *)),
				this,
				SLOT(clicked(Qt3DRender::QPickEvent *)));

		// Mesh 2: Board
		objectPicker2 = new Qt3DRender::QObjectPicker();
		mesh2->setMeshName("12_-_Default");
		mesh2->setSource(QUrl(QUrl::fromLocalFile("/home/wsk/ElephantChess/res/chinese-chess.obj")));
		entity2 = new Qt3DCore::QEntity(rootEntity);
		entity2->addComponent(mesh2);
		//cylinderEntity->addComponent(cylinderTransform);
		entity2->addComponent(scene.makeTex("Image_2.jpg"));
		objectPicker2->setObjectName(QStringLiteral("Mesh2"));
		entity2->addComponent(objectPicker2);
		connect(objectPicker2,
				SIGNAL(pressed(Qt3DRender::QPickEvent *)),
				this,
				SLOT(clicked(Qt3DRender::QPickEvent *)));

		// Mesh 3: First Piece
		objectPicker3 = new Qt3DRender::QObjectPicker();
		mesh3->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
		mesh3->setMeshName("Black-jiang-diffuse");
		mesh3->setSource(QUrl(QUrl::fromLocalFile("/home/wsk/ElephantChess/res/chinese-chess.obj")));
		entity3 = new Qt3DCore::QEntity(rootEntity);
		entity3->addComponent(mesh3);
		//cylinderEntity->addComponent(cylinderTransform);
		entity3->addComponent(scene.makeTex("Image_3.png"));
		objectPicker3->setObjectName("Mesh3");
		entity3->addComponent(objectPicker3);
		connect(objectPicker3,
				SIGNAL(pressed(Qt3DRender::QPickEvent *)),
				this,
				SLOT(clicked(Qt3DRender::QPickEvent *)));

		// Mesh 4: Second Piece
		objectPicker4 = new Qt3DRender::QObjectPicker();
		mesh4->setMeshName("Black-jiang-diffuse");
		mesh4->setSource(QUrl(QUrl::fromLocalFile("/home/wsk/ElephantChess/res/chinese-chess.obj")));
		entity4 = new Qt3DCore::QEntity(rootEntity);
		entity4->addComponent(mesh3);

		Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
		transform->setTranslation(QVector3D(-20.0, 0.1, 20.0));
		entity4->addComponent(transform);

		//cylinderEntity->addComponent(cylinderTransform);
		entity4->addComponent(scene.makeTex("Image_5.png"));
		objectPicker4->setObjectName("Mesh4");

		entity4->addComponent(objectPicker4);
		connect(objectPicker4,
				SIGNAL(pressed(Qt3DRender::QPickEvent *)),
				this,
				SLOT(clicked(Qt3DRender::QPickEvent *)));

	#else
		*/


	auto loader = new Qt3DRender::QSceneLoader(rootEntity);
	loader->setSource(
		QUrl(QUrl::fromLocalFile(":/res/3dBoard/board.dae")));
    entity1 = new Qt3DCore::QEntity(rootEntity);
	//cylinderEntity->addComponent(objectPicker);
//#endif
		// Set root object of the scene
	
	connect(loader, &Qt3DRender::QSceneLoader::statusChanged, this, [this, loader](Qt3DRender::QSceneLoader::Status status) {
	if (status == Qt3DRender::QSceneLoader::Ready) {
		const QVector<Qt3DCore::QEntity*> entities = loader->entities();
		for (Qt3DCore::QEntity* entity : entities) {
			makeEntityPickable(entity);            
		}
	}
    auto list = loader->entityNames();
    qDebug() << list;

    });

    //m_renderSettings = new Qt3DRender::QRenderSettings();
    // m_renderSettings->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
    //m_renderSettings->pickingSettings()->setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);


    entity1->addComponent(loader);
    //rootEntity->addComponent(m_renderSettings);

    setRootEntity(rootEntity);
}

void RenderView::makeEntityPickable(Qt3DCore::QEntity* entity) {

	auto picker = new Qt3DRender::QObjectPicker(entity);
    picker->setHoverEnabled(false);
    picker->setEnabled(true);
	entity->addComponent(picker);

	QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, [](Qt3DRender::QPickEvent* pickEvent) {
		qDebug() << "Entity clicked:" << pickEvent->entity();
		});
}



//void RenderView::clicked(Qt3DRender::QPickEvent* pick)
//{
	//cameraEntity->viewAll();
	//qDebug() << "Picking!" << transl;
	//Qt3DCore::QEntity *name = new Qt3DCore::QEntity();
	//QString n = name->name();
/*	qDebug() << pick->entity() << entity4 << entity3 << entity2 << entity1;
	qDebug() << pick->objectName();
	if (pick->entity() == loader->entity("Red - bing - diffuse_baseColor")) {
		if (pick->button() == Qt3DRender::QPickEvent::RightButton)
			transl = transl - 10.0;
		if (pick->button() == Qt3DRender::QPickEvent::LeftButton)
			transl = transl + 10.0;
		//cylinderTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
		//cylinderTransform->setRotation(
		//    QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f + transl));
		//camtrans = cameraEntity->transform(); //);
		cameraEntity->rotate(
			QQuaternion::fromAxisAndAngle(QVector3D(0.1f, 0.0f, 0.0f), 45.0f + transl));
		//cameraEntity->setPosition(QVector3D(0, 0, 20.0f + transl));

		pick->entity();
		//}
	}
	*/
//}
//void RenderView::mousePressEvent(QMouseEvent *ev)
//{
//    qDebug() << "Mousebutton pressed";
//    //Qt3DCore::QEntity *ent = PickingSettings.entity();
//}

void PaintedTextureImage::paint(QPainter* p)
{
	//paintBoard(p);
	//PaintPieces(p);
}

void PaintedTextureImage::paintBoard(QPainter* p)
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
	}
	else {
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
// Draws the pieces on the board (native)
QPixmap* PaintedTextureImage::PrepareNativePiece(QPainter* p, int row, int col)
{
	//QPainter *p;
	Q_ASSERT(p);

	auto w = 900;
	//p->viewport().width(); //p->viewport().width();
	auto h = 900;
	//p->viewport().height(); //p->viewport().height();

	QPen pen;
	pen.setColor(Qt::red);
	pen.setWidth(3);
	p->setPen(pen);

	QFont font;
	font.setPointSize(30);
	font.setWeight(QFont::DemiBold);
	p->setFont(font);
	// Draws all pieces
    if (basemodel.position.board[row][8 - col]->getName() != "") {
        if (basemodel.position.board[row][8 - col]->getColor() == Color::Red) {
			// Draw red
			pen.setColor(Qt::red);
			p->setPen(pen);
		}
		else {
			// Draw black
			pen.setColor(Qt::black);
			p->setPen(pen);
		}
		QRadialGradient
			gradient1(QPointF(50.0 + (((8 - col)) * (w - 2.0 * 50.0) / BaseModel::BoardColPoints),
				50 + (9 - row) * (h - 2 * 50) / BaseModel::BoardRowPoints),
				50);
		gradient1.setColorAt(1, QColor::fromRgb(222, 91, 16, 255));
		gradient1.setColorAt(0, QColor::fromRgb(255, 255, 255, 255));

		QBrush brush1(gradient1);
		p->setBrush(brush1);

		p->drawChord(QRect((50 + (((8 - col)) * (w - 2 * 50) / BaseModel::BoardColPoints))
			- w / BaseModel::BoardColPoints / 2 / 1.5,
			(50 + (9 - row) * (h - 50 - 100) / BaseModel::BoardRowPoints)
			- h / BaseModel::BoardColPoints / 2 / 1.5,
			w / (BaseModel::BoardColPoints) / 1.5,
			h / BaseModel::BoardColPoints / 1.5),
			0,
			5760);

		p->drawText(QRect((50 + (((8 - col)) * (w - 2 * 50) / BaseModel::BoardColPoints))
			- w / BaseModel::BoardColPoints / 2 / 1.8,
			(50 + (9 - row) * (h - 50 - 100) / BaseModel::BoardRowPoints)
			- h / BaseModel::BoardColPoints / 2 / 2,
			w / (BaseModel::BoardColPoints),
			h / BaseModel::BoardColPoints),
            basemodel.position.board[row][8 - col]->getName());
		p->drawPixmap(0, 0, pix);
	}
	return &pix;
}

// Painted from upper left!
void PaintedTextureImage::PaintPieces(QPainter* p)
{
	Q_ASSERT(p);

	auto w = 900;
	//p->window().width(); //p->viewport().width();
	auto h = 900;
	//p->window().height(); //p->viewport().height();

	// Draws all pieces
	//basemodel.viewStyleModeVar = viewStyleMode::western_png;
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 9; i++) {
			QPixmap pixm;
			QPixmap pixm2;
            if (basemodel.position.board[j][8 - i]->getColor() == Color::Red) {
                if (basemodel.viewStyleModeVar == BaseModel::viewStyleMode::western_png) {
                    //pixm = QPixmap::fromImage(basemodel.position.board[j][8 - i].piece->img);
                    //pixm2 = pixm.copy(100, 0, 100, 100);
				}
                else if (basemodel.viewStyleModeVar == BaseModel::viewStyleMode::traditional_png) {
                    //pixm = QPixmap::fromImage(basemodel.position.board[j][8 - i].piece->img,
                    //	Qt::PreferDither);
                    //pixm2 = pixm.copy(0, 0, 100, 100);
				}
                else if (basemodel.viewStyleModeVar == BaseModel::viewStyleMode::traditional_native) {
					pixm2 = *PrepareNativePiece(p, j, i);
				}
			}
			else {
                if (basemodel.viewStyleModeVar == BaseModel::viewStyleMode::western_png) {
                    //pixm = QPixmap::fromImage(basemodel.position.board[j][8 - i].piece->img);
                    //pixm2 = pixm.copy(300, 0, 100, 100);
				}
                else if (basemodel.viewStyleModeVar == BaseModel::viewStyleMode::traditional_png) {
                    //pixm = QPixmap::fromImage(basemodel.position.board[j][8 - i].piece->img,
                    //	Qt::PreferDither);
                    //pixm2 = pixm.copy(200, 0, 100, 100);
				}
                else if (basemodel.viewStyleModeVar == BaseModel::viewStyleMode::traditional_native) {
					pixm2 = *PrepareNativePiece(p, j, i);
				}
			}
			p->drawPixmap(QRect((50 + ((8 - i) * (w - 2 * 50) / BaseModel::BoardColPoints))
				- w / BaseModel::BoardColPoints / 2 / 1.5,
				(50 + (9 - j) * (h - 50 - 100) / BaseModel::BoardRowPoints)
				- h / BaseModel::BoardColPoints / 2 / 1.5,
				w / (BaseModel::BoardColPoints) / 1.5,
				h / BaseModel::BoardColPoints / 1.5),
				pixm2);
		}
	}
}
