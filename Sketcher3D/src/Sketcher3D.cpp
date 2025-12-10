#include "stdafx.h"
#include "Sketcher3D.h"

#include "ShapeSlots.h"
#include "Transformations.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStyle>
#include <QDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QColorDialog>
#include <QCheckBox>

Sketcher3D::Sketcher3D(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    resize(1200, 700);
}

Sketcher3D::~Sketcher3D() {}

void Sketcher3D::setupUI()
{
    mCentralWidget = std::make_unique<QWidget>(this);
    mCentralgridWidget = std::make_unique<QGridLayout>(mCentralWidget.get());
    setCentralWidget(mCentralWidget.get());

    menuBarElements();
    toolBarElements();

    glWidget = new OpenGLWidget(this);
    glWidget->setMinimumSize(800, 600);
    mCentralgridWidget->addWidget(glWidget, 0, 0);

    mStatusBar = std::make_unique<QStatusBar>(this);
    setStatusBar(mStatusBar.get());
    mStatusBar->showMessage("Left-drag: rotate | Wheel: zoom | Right-click: select");

    // toolbar
    connect(mCuboidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCuboidToolClicked);
    connect(mCubeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCubeToolClicked);
    connect(mConeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onConeToolClicked);
    connect(mCylinderTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCylinderToolClicked);
    connect(mPyramidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onPyramidClicked);
    connect(mSphereTool.get(), &QToolButton::clicked, this, &Sketcher3D::onSphereToolClicked);

    // transforms
    connect(mTranslate, &QAction::triggered, this, &Sketcher3D::onTranslateActionTriggered);
    connect(mScale, &QAction::triggered, this, &Sketcher3D::onScaleActionTriggered);
    connect(mRotateX, &QAction::triggered, this, &Sketcher3D::onRotateXActionTriggered);
    connect(mRotateY, &QAction::triggered, this, &Sketcher3D::onRotateYActionTriggered);
    connect(mRotateZ, &QAction::triggered, this, &Sketcher3D::onRotateZActionTriggered);

    // file
    connect(mSaveGNUAction, &QAction::triggered, this, &Sketcher3D::onSaveGNUActionTriggered);
    connect(mSaveAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
    connect(mLoadSTLAction, &QAction::triggered, this, &Sketcher3D::onLoadSTLTriggered);
    connect(mSaveSTLAction, &QAction::triggered, this, &Sketcher3D::onSaveSTLTriggered);
    connect(mClearAction, &QAction::triggered, this, &Sketcher3D::onClearTriggered);

    // properties
    connect(mPropertiesAction, &QAction::triggered, this, &Sketcher3D::onPropertiesTriggered);
}

std::unique_ptr<QToolButton> Sketcher3D::createToolButton(
    QToolBar* toolbar, const QString& iconPath,
    const QString& tooltip, const QSize& iconSize)
{
    std::unique_ptr<QToolButton> btn = std::make_unique<QToolButton>(toolbar);
    btn->setIcon(QIcon(iconPath));
    btn->setIconSize(iconSize);
    btn->setToolTip(tooltip);
    toolbar->addWidget(btn.get());
    return btn;
}

void Sketcher3D::toolBarElements()
{
    mToolBar = std::make_unique<QToolBar>(this);
    addToolBar(mToolBar.get());
    mToolBar->setIconSize(QSize(32, 32));

    mCuboidTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/cuboid.png", "Cuboid");
    mCubeTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/cube.png", "Cube");
    mSphereTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/sphere.png", "Sphere");
    mCylinderTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/cylinder.png", "Cylinder");
    mConeTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/cone.png", "Cone");
    mPyramidTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/pyramid.png", "Pyramid");
}

void Sketcher3D::menuBarElements()
{
    mMenuBar = std::make_unique<QMenuBar>(this);
    setMenuBar(mMenuBar.get());

    // File
    mFileMenu = mMenuBar->addMenu("File");
    mSaveMenu = mFileMenu->addMenu("Save");
    mSaveAction = mSaveMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save");
    mSaveAction->setShortcut(QKeySequence::Save);
    mSaveGNUAction = mSaveMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "SaveGNU");
    mSaveSTLAction = mSaveMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "SaveSTL");
    mLoadSTLAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "LoadSTL");

    mNewAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_FileIcon), "New");
    mNewAction->setShortcut(QKeySequence::New);
    mOpenAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DirOpenIcon), "Open");
    mOpenAction->setShortcut(QKeySequence::Open);

    // Edit
    QMenu* editMenu = mMenuBar->addMenu("Edit");
    mClearAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_TrashIcon), "Clear");
    mClearAction->setShortcut(Qt::CTRL | Qt::Key_X);
    QAction* undoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowBack), "Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    QAction* redoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowForward), "Redo");
    redoAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Z);

    // Transformations
    mTransformationMenu = mMenuBar->addMenu("Transformations");
    mTranslate = mTransformationMenu->addAction("Translate");
    mScale = mTransformationMenu->addAction("Scale");
    mRotate = mTransformationMenu->addMenu("Rotate");
    mRotateX = mRotate->addAction("Rotate w.r.t x - Axis");
    mRotateY = mRotate->addAction("Rotate w.r.t y - Axis");
    mRotateZ = mRotate->addAction("Rotate w.r.t z - Axis");

    // Properties
    mPropertiesAction = mMenuBar->addAction("Properties");
}

// ---------- toolbar shape slots ----------
void Sketcher3D::onCuboidToolClicked()
{
    try {
        std::shared_ptr<Shape> cb = std::make_shared<Cuboid>(ShapeSlots::cuboidSlot(this));
        shapeManager.addShape(cb);
        glWidget->drawShape(cb->getTriangulation().getDataForOpenGl(),
            cb->getTriangulation().getNormalForOpenGl());
        mStatusBar->showMessage("Cuboid created");
    }
    catch (const std::runtime_error& e) {
        QMessageBox::information(this, "Info", e.what());
    }
}

void Sketcher3D::onCubeToolClicked()
{
    try {
        std::shared_ptr<Shape> c = std::make_shared<Cube>(ShapeSlots::cubeSlot(this));
        shapeManager.addShape(c);
        glWidget->drawShape(c->getTriangulation().getDataForOpenGl(),
            c->getTriangulation().getNormalForOpenGl());
        mStatusBar->showMessage("Cube created");
    }
    catch (const std::runtime_error& e) {
        QMessageBox::information(this, "Info", e.what());
    }
}

void Sketcher3D::onPyramidClicked()
{
    try {
        std::shared_ptr<Shape> py = std::make_shared<Pyramid>(ShapeSlots::pyramidSlot(this));
        shapeManager.addShape(py);
        glWidget->drawShape(py->getTriangulation().getDataForOpenGl(),
            py->getTriangulation().getNormalForOpenGl());
        mStatusBar->showMessage("Pyramid created");
    }
    catch (const std::runtime_error& e) {
        QMessageBox::information(this, "Info", e.what());
    }
}

void Sketcher3D::onCylinderToolClicked()
{
    try {
        std::shared_ptr<Shape> cyl = std::make_shared<Cylinder>(ShapeSlots::cylinderSlot(this));
        shapeManager.addShape(cyl);
        glWidget->drawShape(cyl->getTriangulation().getDataForOpenGl(),
            cyl->getTriangulation().getNormalForOpenGl());
        mStatusBar->showMessage("Cylinder created");
    }
    catch (const std::runtime_error& e) {
        QMessageBox::information(this, "Info", e.what());
    }
}

void Sketcher3D::onConeToolClicked()
{
    try {
        std::shared_ptr<Shape> cone = std::make_shared<Cone>(ShapeSlots::coneSlot(this));
        shapeManager.addShape(cone);
        glWidget->drawShape(cone->getTriangulation().getDataForOpenGl(),
            cone->getTriangulation().getNormalForOpenGl());
        mStatusBar->showMessage("Cone created");
    }
    catch (const std::runtime_error& e) {
        QMessageBox::information(this, "Info", e.what());
    }
}

void Sketcher3D::onSphereToolClicked()
{
    try {
        std::shared_ptr<Shape> sp = std::make_shared<Sphere>(ShapeSlots::sphereSlot(this));
        shapeManager.addShape(sp);
        glWidget->drawShape(sp->getTriangulation().getDataForOpenGl(),
            sp->getTriangulation().getNormalForOpenGl());
        mStatusBar->showMessage("Sphere created");
    }
    catch (const std::runtime_error& e) {
        QMessageBox::information(this, "Info", e.what());
    }
}

// ---------- file ----------
void Sketcher3D::onSaveGNUActionTriggered()
{
    QString qFileName = QFileDialog::getSaveFileName(this, "Save Shapes", "", "*.dat");
    auto shapesVec = shapeManager.getShapesVec();
    if (FileHandle::saveToFileGNUPlot(qFileName.toStdString(), shapesVec))
        QMessageBox::information(this, "Save for GNU", "Shapes saved for GNU.");
    else
        QMessageBox::warning(this, "Not Saved", "Shapes not saved!");
}

void Sketcher3D::onLoadSTLTriggered()
{
    QString qFileName = QFileDialog::getOpenFileName(this, "Open STL", "", "*.STL");
    FileHandle::readSTL(qFileName.toStdString(), tri);

    if (!tri.getDataForOpenGl().empty()) {
        glWidget->drawShape(tri.getDataForOpenGl(), tri.getNormalForOpenGl());
        QMessageBox::information(this, "Load", "Shapes loaded and rendered in 3D viewer!");
    }
    else {
        QMessageBox::warning(this, "Not loaded", "Shapes not loaded!");
    }
}

void Sketcher3D::onSaveSTLTriggered()
{
    QString qFileName = QFileDialog::getSaveFileName(this, "Save Shapes", "", "*.STL");
    auto shapesVec = shapeManager.getShapesVec();
    if (FileHandle::writeSTL(qFileName.toStdString(), shapesVec))
        QMessageBox::information(this, "Save STL", "Shapes saved in STL.");
    else
        QMessageBox::warning(this, "Not Saved", "Shapes not saved!");
}

void Sketcher3D::onSaveActionTriggered()
{
    QString qFileName = QFileDialog::getSaveFileName(this, "Save Shapes", "", "*.skt");
    auto shapesVec = shapeManager.getShapesVec();
    if (FileHandle::saveToFile(qFileName.toStdString(), shapesVec))
        QMessageBox::information(this, "Save .skt", "Shapes saved in .skt format.");
    else
        QMessageBox::warning(this, "Not Saved", "Shapes not saved!");
}

void Sketcher3D::onClearTriggered()
{
    shapeManager.clearShape();
    glWidget->clearShape();
}

// ---------- transforms ----------
void Sketcher3D::onTranslateActionTriggered()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Translate Object");
    auto* xL = new QLabel("Translate X"), * yL = new QLabel("Translate Y"), * zL = new QLabel("Translate Z");
    auto* x = new QDoubleSpinBox(), * y = new QDoubleSpinBox(), * z = new QDoubleSpinBox();
    for (auto* s : { x,y,z }) { s->setRange(-10000, 10000); s->setDecimals(2); s->setValue(0.0); }
    auto* ok = new QPushButton("Apply"), * cancel = new QPushButton("Cancel");
    auto* grid = new QGridLayout(); grid->addWidget(xL, 0, 0); grid->addWidget(x, 0, 1);
    grid->addWidget(yL, 1, 0); grid->addWidget(y, 1, 1);
    grid->addWidget(zL, 2, 0); grid->addWidget(z, 2, 1);
    auto* hl = new QHBoxLayout(); hl->addWidget(ok); hl->addWidget(cancel);
    grid->addLayout(hl, 3, 0, 1, 2); dlg.setLayout(grid);
    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted) return;

    auto shape = shapeManager.getLastShape(); if (!shape) return;
    auto vec = shape->getTriangulation().getDataForOpenGl();
    auto transformed = Transformations::translate(vec, x->value(), y->value(), z->value());
    glWidget->drawShape(transformed);
}

void Sketcher3D::onScaleActionTriggered()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Scale Object");
    auto* xL = new QLabel("Scale X"), * yL = new QLabel("Scale Y"), * zL = new QLabel("Scale Z");
    auto* x = new QDoubleSpinBox(), * y = new QDoubleSpinBox(), * z = new QDoubleSpinBox();
    for (auto* s : { x,y,z }) { s->setRange(0.01, 1000); s->setValue(1.0); }
    auto* ok = new QPushButton("Apply"), * cancel = new QPushButton("Cancel");
    auto* grid = new QGridLayout(); grid->addWidget(xL, 0, 0); grid->addWidget(x, 0, 1);
    grid->addWidget(yL, 1, 0); grid->addWidget(y, 1, 1);
    grid->addWidget(zL, 2, 0); grid->addWidget(z, 2, 1);
    auto* hl = new QHBoxLayout(); hl->addWidget(ok); hl->addWidget(cancel);
    grid->addLayout(hl, 3, 0, 1, 2); dlg.setLayout(grid);
    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted) return;

    auto shape = shapeManager.getLastShape(); if (!shape) return;
    auto vec = shape->getTriangulation().getDataForOpenGl();
    auto transformed = Transformations::scale(vec, x->value(), y->value(), z->value());
    glWidget->drawShape(transformed);
}

void Sketcher3D::onRotateXActionTriggered()
{
    QDialog dlg(this); dlg.setWindowTitle("Rotate X");
    auto* lab = new QLabel("Angle (degrees)"); auto* ang = new QDoubleSpinBox();
    ang->setRange(-360, 360); ang->setDecimals(2); ang->setValue(0.0);
    auto* ok = new QPushButton("Apply"), * cancel = new QPushButton("Cancel");
    auto* v = new QVBoxLayout(); v->addWidget(lab); v->addWidget(ang);
    auto* hl = new QHBoxLayout(); hl->addWidget(ok); hl->addWidget(cancel); v->addLayout(hl);
    dlg.setLayout(v);
    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted) return;

    double angleX = ((MathConstants::PI * ang->value()) / 180);
    auto shape = shapeManager.getLastShape(); if (!shape) return;
    auto vec = shape->getTriangulation().getDataForOpenGl();
    glWidget->drawShape(Transformations::rotationX(vec, angleX));
}

void Sketcher3D::onRotateYActionTriggered()
{
    QDialog dlg(this); dlg.setWindowTitle("Rotate Y");
    auto* lab = new QLabel("Angle (degrees)"); auto* ang = new QDoubleSpinBox();
    ang->setRange(-360, 360); ang->setDecimals(2); ang->setValue(0.0);
    auto* ok = new QPushButton("Apply"), * cancel = new QPushButton("Cancel");
    auto* v = new QVBoxLayout(); v->addWidget(lab); v->addWidget(ang);
    auto* hl = new QHBoxLayout(); hl->addWidget(ok); hl->addWidget(cancel); v->addLayout(hl);
    dlg.setLayout(v);
    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted) return;

    double angleY = ((MathConstants::PI * ang->value()) / 180);
    auto shape = shapeManager.getLastShape(); if (!shape) return;
    auto vec = shape->getTriangulation().getDataForOpenGl();
    glWidget->drawShape(Transformations::rotationY(vec, angleY));
}

void Sketcher3D::onRotateZActionTriggered()
{
    QDialog dlg(this); dlg.setWindowTitle("Rotate Z");
    auto* lab = new QLabel("Angle (degrees)"); auto* ang = new QDoubleSpinBox();
    ang->setRange(-360, 360); ang->setDecimals(2); ang->setValue(0.0);
    auto* ok = new QPushButton("Apply"), * cancel = new QPushButton("Cancel");
    auto* v = new QVBoxLayout(); v->addWidget(lab); v->addWidget(ang);
    auto* hl = new QHBoxLayout(); hl->addWidget(ok); hl->addWidget(cancel); v->addLayout(hl);
    dlg.setLayout(v);
    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted) return;

    double angleZ = ((MathConstants::PI * ang->value()) / 180);
    auto shape = shapeManager.getLastShape(); if (!shape) return;
    auto vec = shape->getTriangulation().getDataForOpenGl();
    glWidget->drawShape(Transformations::rotationZ(vec, angleZ));
}

// ---------- Properties dialog ----------
void Sketcher3D::onPropertiesTriggered()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Properties");

    auto* form = new QFormLayout(&dlg);

    // Color (solid)
    auto* colorBtn = new QPushButton("Pick Colour…", &dlg);
    auto* colorPreview = new QLabel("      ");
    colorPreview->setAutoFillBackground(true);
    QPalette pal = colorPreview->palette();
    pal.setColor(QPalette::Window, QColor(0, 179, 255));
    colorPreview->setPalette(pal);
    colorPreview->setFrameShape(QFrame::Box);

    auto* colorRow = new QWidget(&dlg);
    auto* colorRowLayout = new QHBoxLayout(colorRow);
    colorRowLayout->setContentsMargins(0, 0, 0, 0);
    colorRowLayout->addWidget(colorBtn);
    colorRowLayout->addWidget(colorPreview, 1);
    form->addRow("Selected Object Solid Colour:", colorRow);

    // Per-vertex color controls
    auto* pvRow = new QWidget(&dlg);
    auto* pvLay = new QHBoxLayout(pvRow);
    pvLay->setContentsMargins(0, 0, 0, 0);
    auto* btnRandomPV = new QPushButton("Random Per-Vertex Colors", &dlg);
    auto* btnClearPV = new QPushButton("Clear Per-Vertex Colors", &dlg);
    pvLay->addWidget(btnRandomPV);
    pvLay->addWidget(btnClearPV);
    form->addRow("Per-vertex Colors:", pvRow);

    // Grid / wireframe / lighting / bbox
    auto* chkGrid = new QCheckBox("Show Grid", &dlg);
    auto* chkWire = new QCheckBox("Wireframe", &dlg);
    auto* chkLight = new QCheckBox("Lighting Enabled", &dlg); chkLight->setChecked(true);
    auto* chkBBox = new QCheckBox("Show Bounding Box (selected)", &dlg); chkBBox->setChecked(true);
    form->addRow(chkGrid);
    form->addRow(chkWire);
    form->addRow(chkLight);
    form->addRow(chkBBox);

    // Material (Ka/Kd/Ks + shininess)
    auto* kaBtn = new QPushButton("Ambient (Ka)…", &dlg);
    auto* kdBtn = new QPushButton("Diffuse (Kd)…", &dlg);
    auto* ksBtn = new QPushButton("Specular (Ks)…", &dlg);
    auto* shin = new QDoubleSpinBox(&dlg);
    shin->setRange(1.0, 256.0); shin->setValue(32.0);

    auto* matRow1 = new QWidget(&dlg);
    auto* matLay1 = new QHBoxLayout(matRow1);
    matLay1->setContentsMargins(0, 0, 0, 0);
    matLay1->addWidget(kaBtn); matLay1->addWidget(kdBtn); matLay1->addWidget(ksBtn);

    form->addRow("Material:", matRow1);
    form->addRow("Shininess:", shin);

    // Buttons
    auto* btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btns);

    // Color selections defaults
    QColor solid(0, 179, 255), ka(26, 26, 26), kd(204, 204, 204), ks(51, 51, 51);

    QObject::connect(colorBtn, &QPushButton::clicked, [&]() {
        QColor c = QColorDialog::getColor(solid, this, "Pick Solid Colour");
        if (c.isValid()) {
            solid = c;
            QPalette p = colorPreview->palette(); p.setColor(QPalette::Window, solid); colorPreview->setPalette(p);
        }
        });
    QObject::connect(kaBtn, &QPushButton::clicked, [&]() {
        QColor c = QColorDialog::getColor(ka, this, "Pick Ambient (Ka)");
        if (c.isValid()) ka = c;
        });
    QObject::connect(kdBtn, &QPushButton::clicked, [&]() {
        QColor c = QColorDialog::getColor(kd, this, "Pick Diffuse (Kd)");
        if (c.isValid()) kd = c;
        });
    QObject::connect(ksBtn, &QPushButton::clicked, [&]() {
        QColor c = QColorDialog::getColor(ks, this, "Pick Specular (Ks)");
        if (c.isValid()) ks = c;
        });

    QObject::connect(btnRandomPV, &QPushButton::clicked, [&]() {
        glWidget->setRandomVertexColorsForSelected();
        });
    QObject::connect(btnClearPV, &QPushButton::clicked, [&]() {
        glWidget->clearVertexColorsForSelected();
        });

    QObject::connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    // Apply
    glWidget->setColorOfSelected(solid);
    glWidget->setMaterialForSelected(ka, kd, ks, float(shin->value()));
    glWidget->setGridVisible(chkGrid->isChecked());
    glWidget->setWireframe(chkWire->isChecked());
    glWidget->setLightingEnabled(chkLight->isChecked());
    glWidget->setShowBoundingBox(chkBBox->isChecked());

    mStatusBar->showMessage("Properties applied");
}
