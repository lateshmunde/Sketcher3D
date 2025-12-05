#include "stdafx.h"
#include "Sketcher3D.h"
#include <iostream>
#include <vector>
#include "ShapeSlots.h"

Sketcher3D::Sketcher3D(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    resize(800, 800);
}

Sketcher3D::~Sketcher3D()
{
}

void Sketcher3D::setupUI()
{
    // Central widget and layout
    mCentralWidget = std::make_unique<QWidget>(this);
    mCentralgridWidget = std::make_unique<QGridLayout>(mCentralWidget.get());
    setCentralWidget(mCentralWidget.get());

    menuBarElements();
    toolBarElements();

    mStatusBar = std::make_unique<QStatusBar>(this);
    setStatusBar(mStatusBar.get());
    mStatusBar->showMessage("Application Started");

    // Connections
    connect(mCuboidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCuboidToolClicked);
    connect(mCubeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCubeToolClicked);
    connect(mConeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onConeToolClicked);
    connect(mCylinderTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCylinderToolClicked);
    connect(mPyramidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onPyramidClicked);
    connect(mSphereTool.get(), &QToolButton::clicked, this, &Sketcher3D::onSphereToolClicked);


    connect(mSaveGNUAction, &QAction::triggered, this, &Sketcher3D::onSaveGNUActionTriggered);
    connect(mSaveAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
    //connect(mNewAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
    //connect(mOpenAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
    //connect(cleanAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
    //connect(undoAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
    //connect(redoAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);

    connect(mTranslate, &QAction::triggered, this, &Sketcher3D::onTranslateActionTriggered);
    connect(mScale, &QAction::triggered, this, &Sketcher3D::onScaleActionTriggered);
    connect(mRotateX, &QAction::triggered, this, &Sketcher3D::onRotateXActionTriggered);
    connect(mRotateY, &QAction::triggered, this, &Sketcher3D::onRotateYActionTriggered);
    connect(mRotateZ, &QAction::triggered, this, &Sketcher3D::onRotateZActionTriggered);


    mGLWidget = std::make_unique<OpenGLWidget>(this);
    mCentralgridWidget->addWidget(mGLWidget.get(), 0, 0);

    std::vector<Point> triangle = {
    Point(0.0, 0.0, 0.0),
    Point(0.0, 0.75, 0.0),
    Point(0.75, 0.0, 0.0)
    };
    mGLWidget->setVertices(triangle);
}

void Sketcher3D::menuBarElements()
{
    // ============================ MENU BAR =================================
    mMenuBar = std::make_unique<QMenuBar>(this);
    setMenuBar(mMenuBar.get());

    
    // File Menu
    mFileMenu = mMenuBar.get()->addMenu("File");
    mSaveMenu = mFileMenu->addMenu("Save");
    mSaveAction = mSaveMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save");
    mSaveAction->setShortcut(QKeySequence::Save); // Ctrl+S
    mSaveGNUAction = mSaveMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "SaveGNU");
    mNewAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_FileIcon), "New");
    mNewAction->setShortcut(QKeySequence::New); // Ctrl+N
    mOpenAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DirOpenIcon), "Open");
    mOpenAction->setShortcut(QKeySequence::Open); // Ctrl+O

    // Edit Menu
    QMenu *editMenu = mMenuBar.get()->addMenu("Edit"); // Local pointer editMenu, no need to delete
    QAction *cleanAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_TrashIcon), "Clean");
    cleanAction->setShortcut(Qt::CTRL | Qt::Key_X); // Ctrl+X
    QAction *undoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowBack), "Undo");
    undoAction->setShortcut(QKeySequence::Undo); // Ctrl+Z
    QAction *redoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowForward), "Redo");
    redoAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Z); // Ctrl+Shift+Z/

    // Transformation Menu
    mTransformationMenu = mMenuBar.get()->addMenu("Transformations");
    mTranslate = mTransformationMenu->addAction("Translate");
    mScale = mTransformationMenu->addAction("Scale");
    mRotate = mTransformationMenu->addMenu("Rotate");
    mRotateX = mRotate->addAction("Rotate w.r.t x - Axis");
    mRotateY = mRotate->addAction("Rotate w.r.t y - Axis");
    mRotateZ = mRotate->addAction("Rotate w.r.t z - Axis");
}

void Sketcher3D::toolBarElements()
{
    // ============================ TOOL BAR =================================
    mToolBar = std::make_unique<QToolBar>(this);
    addToolBar(mToolBar.get());
    mToolBar->setIconSize(QSize(32, 32));

    // Cuboid Tool
    mCuboidTool = std::make_unique<QToolButton>(mToolBar.get());
    mCuboidTool.get()->setIcon(QIcon(":/Sketcher3D/Resources/cuboid.png"));
    mCuboidTool.get()->setIconSize(QSize(32, 32));
    mCuboidTool.get()->setToolTip("Cuboid");
    mToolBar.get()->addWidget(mCuboidTool.get());

    // Cube Tool
    mCubeTool = std::make_unique<QToolButton>(mToolBar.get());
    mCubeTool.get()->setIcon(QIcon(":/Sketcher3D/Resources/cube.png"));
    mCubeTool.get()->setIconSize(QSize(32, 32));
    mCubeTool.get()->setToolTip("Cube");
    mToolBar.get()->addWidget(mCubeTool.get());

    // Sphere Tool
    mSphereTool = std::make_unique<QToolButton>(mToolBar.get());
    mSphereTool.get()->setIcon(QIcon(":/Sketcher3D/Resources/sphere.png"));
    mSphereTool.get()->setIconSize(QSize(32, 32));
    mSphereTool.get()->setToolTip("Sphere");
    mToolBar.get()->addWidget(mSphereTool.get());

    // Cylinder Tool
    mCylinderTool = std::make_unique<QToolButton>(mToolBar.get());
    mCylinderTool.get()->setIcon(QIcon(":/Sketcher3D/Resources/cylinder.png"));
    mCylinderTool.get()->setIconSize(QSize(32, 32));
    mCylinderTool.get()->setToolTip("Cylinder");
    mToolBar.get()->addWidget(mCylinderTool.get());

    // Cone Tool
    mConeTool = std::make_unique<QToolButton>(mToolBar.get());
    mConeTool.get()->setIcon(QIcon(":/Sketcher3D/Resources/cone.png"));
    mConeTool.get()->setIconSize(QSize(32, 32));
    mConeTool.get()->setToolTip("Cone");
    mToolBar.get()->addWidget(mConeTool.get());

    // Pyramid Tool
    mPyramidTool = std::make_unique<QToolButton>(mToolBar.get());
    mPyramidTool.get()->setIcon(QIcon(":/Sketcher3D/Resources/pyramid.png"));
    mPyramidTool.get()->setIconSize(QSize(32, 32));
    mPyramidTool.get()->setToolTip("Pyramid");
    mToolBar.get()->addWidget(mPyramidTool.get());
}

void Sketcher3D::onCuboidToolClicked()
{
    // Create cuboid
    try {
        std::shared_ptr<Shape> cb = std::make_shared<Cuboid>(ShapeSlots::cuboidSlot(this));
        shapeManager.addShape(cb);
        mGLWidget->setVertices(cb->coodinatesForGLTriangle());
        mStatusBar->showMessage("Cuboid created");
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::information(nullptr, "Info", e.what());
    }
}

void Sketcher3D::onCubeToolClicked()
{
    // Create Cube object
    try {
        std::shared_ptr<Shape> c = std::make_shared<Cube>(ShapeSlots::cubeSlot(this));
        shapeManager.addShape(c);
        mGLWidget->setVertices(c->coodinatesForGLTriangle());
        mStatusBar->showMessage("Cube created");
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::information(nullptr, "Info", e.what());
    }
}

void Sketcher3D::onPyramidClicked()
{
    // Create Pyramid
    try {
        std::shared_ptr<Shape> py = std::make_shared<Pyramid>(ShapeSlots::pyramidSlot(this));
        shapeManager.addShape(py);
        mGLWidget->setVertices(py->coodinatesForGLTriangle());
        mStatusBar->showMessage("Pyramid created");
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::information(nullptr, "Info", e.what());
    }
}

void Sketcher3D::onCylinderToolClicked()
{
    // Create Cylinder object
    try {
        std::shared_ptr<Shape> cyl = std::make_shared<Cylinder>(ShapeSlots::cylinderSlot(this));
        shapeManager.addShape(cyl);
        mGLWidget->setVertices(cyl->coodinatesForGLTriangle());
        mStatusBar->showMessage("Cylinder created");
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::information(nullptr, "Info", e.what());
    }
}

void Sketcher3D::onConeToolClicked()
{
    // Create Cone object
    try {
        std::shared_ptr<Shape> cone = std::make_shared<Cone>(ShapeSlots::coneSlot(this));
        shapeManager.addShape(cone);
        mGLWidget->setVertices(cone->coodinatesForGLTriangle());
        mStatusBar->showMessage("Cone created");
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::information(nullptr, "Info", e.what());
    }
}

void Sketcher3D::onSphereToolClicked()
{
    // Create Sphere object
    try {
        std::shared_ptr<Shape> sp = std::make_shared<Sphere>(ShapeSlots::sphereSlot(this));
        shapeManager.addShape(sp);
        mGLWidget->setVertices(sp->coodinatesForGLTriangle());
        mStatusBar->showMessage("Sphere created");
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::information(nullptr, "Info", e.what());
    }
}

void Sketcher3D::onSaveGNUActionTriggered()
{
    /*QString qFileName = QFileDialog::getSaveFileName(
        this, "Save Shapes", "", ".dat");*/

    // Save shapes to file (GNU Plot)
    std::string fileName = "../ShapesGNU.dat";

    std::vector<std::shared_ptr<Shape>> shapesVec = shapeManager.getShapesVec();
    if (FileHandle::saveToFileGNUPlot(fileName, shapesVec))
    {
        QMessageBox::information(this, "Saved", "Shapes saved successfully!");
    }
    else
        QMessageBox::information(this, "Not Saved", "Shapes not saved!");
}

void Sketcher3D::onSaveActionTriggered()
{
    /*QString qFileName = QFileDialog::getSaveFileName(
        this, "Save Shapes", "", ".dat");*/

        // Save shapes to file
    std::string fileName = "../Shapes.skt";

    std::vector<std::shared_ptr<Shape>> shapesVec = shapeManager.getShapesVec();
    if (FileHandle::saveToFile(fileName, shapesVec))
    {
        QMessageBox::information(this, "Saved", "Shapes saved successfully!");
    }
    else
        QMessageBox::information(this, "Not Saved", "Shapes not saved!");
}

void Sketcher3D::onTranslateActionTriggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Translate Object");

    QLabel* xLabel = new QLabel("Translate X");
    QLabel* yLabel = new QLabel("Translate Y");
    QLabel* zLabel = new QLabel("Translate Z");

    QDoubleSpinBox* xSpin = new QDoubleSpinBox();
    QDoubleSpinBox* ySpin = new QDoubleSpinBox();
    QDoubleSpinBox* zSpin = new QDoubleSpinBox();

    xSpin->setRange(-10000, 10000);
    ySpin->setRange(-10000, 10000);
    zSpin->setRange(-10000, 10000);

    xSpin->setDecimals(2);
    ySpin->setDecimals(2);
    zSpin->setDecimals(2);

    xSpin->setValue(0.0);
    ySpin->setValue(0.0);
    zSpin->setValue(0.0);

    QPushButton* okButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(xLabel, 0, 0);
    layout->addWidget(xSpin, 0, 1);

    layout->addWidget(yLabel, 1, 0);
    layout->addWidget(ySpin, 1, 1);

    layout->addWidget(zLabel, 2, 0);
    layout->addWidget(zSpin, 2, 1);

    layout->addLayout(buttonLayout, 3, 0, 1, 2);

    dialog.setLayout(layout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
        return;

    double tx = xSpin->value();
    double ty = ySpin->value();
    double tz = zSpin->value();

    std::vector<Point> currentVertices = mGLWidget->getVertices();
    Transformation::Translate(currentVertices, tx, ty, tz);

    mGLWidget->setVertices(currentVertices);
}

void Sketcher3D::onScaleActionTriggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Scale Object");

    QLabel* xLabel = new QLabel("Scale X");
    QLabel* yLabel = new QLabel("Scale Y");
    QLabel* zLabel = new QLabel("Scale Z");

    QDoubleSpinBox* xSpin = new QDoubleSpinBox();
    QDoubleSpinBox* ySpin = new QDoubleSpinBox();
    QDoubleSpinBox* zSpin = new QDoubleSpinBox();

    xSpin->setRange(0.01, 1000);
    ySpin->setRange(0.01, 1000);
    zSpin->setRange(0.01, 1000);

    xSpin->setValue(1.0);
    ySpin->setValue(1.0);
    zSpin->setValue(1.0);

    QPushButton* okButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(xLabel, 0, 0);
    layout->addWidget(xSpin, 0, 1);

    layout->addWidget(yLabel, 1, 0);
    layout->addWidget(ySpin, 1, 1);

    layout->addWidget(zLabel, 2, 0);
    layout->addWidget(zSpin, 2, 1);

    layout->addLayout(buttonLayout, 3, 0, 1, 2);

    dialog.setLayout(layout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
        return;

    double sx = xSpin->value();
    double sy = ySpin->value();
    double sz = zSpin->value();

    std::vector<Point> currentVertices = mGLWidget->getVertices();

    Transformation::Scale(currentVertices, sx, sy, sz);

    mGLWidget->setVertices(currentVertices);
    mGLWidget->update();
}

void Sketcher3D::onRotateXActionTriggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Rotate X");

    QLabel* angleLabel = new QLabel("Angle (degrees)");

    QDoubleSpinBox* angleSpin = new QDoubleSpinBox();
    angleSpin->setRange(-360, 360);
    angleSpin->setValue(0.0);
    angleSpin->setDecimals(2);

    QPushButton* okButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(angleLabel);
    layout->addWidget(angleSpin);
    layout->addLayout(buttonLayout);

    dialog.setLayout(layout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
        return;

    double angle = ((MathConstants::PI * angleSpin->value()) / 180);

    std::vector<Point> currentVertices = mGLWidget->getVertices();

    Transformation::RotateX(currentVertices, angle);

    mGLWidget->setVertices(currentVertices);
}

void Sketcher3D::onRotateYActionTriggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Rotate Y");

    QLabel* angleLabel = new QLabel("Angle (degrees)");

    QDoubleSpinBox* angleSpin = new QDoubleSpinBox();
    angleSpin->setRange(-360, 360);
    angleSpin->setValue(0.0);
    angleSpin->setDecimals(2);

    QPushButton* okButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(angleLabel);
    layout->addWidget(angleSpin);
    layout->addLayout(buttonLayout);

    dialog.setLayout(layout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
        return;

    double angle = ((MathConstants::PI * angleSpin->value()) / 180);

    std::vector<Point> currentVertices = mGLWidget->getVertices();

    Transformation::RotateY(currentVertices, angle);

    mGLWidget->setVertices(currentVertices);
}

void Sketcher3D::onRotateZActionTriggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Rotate Z");

    QLabel* angleLabel = new QLabel("Angle (degrees)");

    QDoubleSpinBox* angleSpin = new QDoubleSpinBox();
    angleSpin->setRange(-360, 360);
    angleSpin->setValue(0.0);
    angleSpin->setDecimals(2);

    QPushButton* okButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(angleLabel);
    layout->addWidget(angleSpin);
    layout->addLayout(buttonLayout);

    dialog.setLayout(layout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
        return;

    double angle = ((MathConstants::PI*angleSpin->value())/180);

    std::vector<Point> currentVertices = mGLWidget->getVertices();

    Transformation::RotateZ(currentVertices, angle);

    mGLWidget->setVertices(currentVertices);
}
