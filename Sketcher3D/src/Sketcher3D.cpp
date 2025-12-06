#include "stdafx.h"
#include "Sketcher3D.h"
#include <iostream>
#include <vector>
#include "ShapeSlots.h"
#include "Transformations.h"

Sketcher3D::Sketcher3D(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    resize(1200, 700);  // Increased size to accommodate OpenGL window
}

Sketcher3D::~Sketcher3D()
{
    // glWidget will be deleted automatically by Qt parent-child relationship
}

void Sketcher3D::setupUI()
{
    // Central widget and layout
    mCentralWidget = std::make_unique<QWidget>(this);
    mCentralgridWidget = std::make_unique<QGridLayout>(mCentralWidget.get());
    setCentralWidget(mCentralWidget.get());

    menuBarElements();
    toolBarElements();

    // ============= CREATE OPENGL WIDGET =============
    glWidget = new OpenGLWidget(this);
    glWidget->setMinimumSize(800, 600);

    // Add OpenGL widget to the central grid layout
    // This places it in row 0, column 0 of the grid
    mCentralgridWidget->addWidget(glWidget, 0, 0);
    // ================================================

    mStatusBar = std::make_unique<QStatusBar>(this);
    setStatusBar(mStatusBar.get());
    //mStatusBar->showMessage("Application Started - 3D Viewer Ready");
    mStatusBar->showMessage("X-axis (Red), Y-axis (Green),  Z-axis (Blue)");

    // Connections
    connect(mCuboidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCuboidToolClicked);
    connect(mCubeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCubeToolClicked);
    connect(mConeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onConeToolClicked);
    connect(mCylinderTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCylinderToolClicked);
    connect(mPyramidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onPyramidClicked);
    connect(mSphereTool.get(), &QToolButton::clicked, this, &Sketcher3D::onSphereToolClicked);

    connect(mTranslate, &QAction::triggered, this, &Sketcher3D::onTranslateActionTriggered);
    connect(mScale, &QAction::triggered, this, &Sketcher3D::onScaleActionTriggered);
    connect(mRotateX, &QAction::triggered, this, &Sketcher3D::onRotateXActionTriggered);
    connect(mRotateY, &QAction::triggered, this, &Sketcher3D::onRotateYActionTriggered);
    connect(mRotateZ, &QAction::triggered, this, &Sketcher3D::onRotateZActionTriggered);
    
    connect(mSaveGNUAction, &QAction::triggered, this, &Sketcher3D::onSaveGNUActionTriggered);
    connect(mSaveAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
    connect(mLoadSTLAction, &QAction::triggered, this, &Sketcher3D::onLoadSTLTriggered);
}


std::unique_ptr<QToolButton> Sketcher3D::createToolButton(
    QToolBar* toolbar, const QString& iconPath,
    const QString& tooltip, const QSize& iconSize = QSize(32, 32))
{
    std::unique_ptr<QToolButton> btn = std::make_unique<QToolButton>(toolbar);

    btn->setIcon(QIcon(iconPath));
    btn->setIconSize(iconSize);
    btn->setToolTip(tooltip);

    toolbar->addWidget(btn.get());
    return btn;        // return unique_ptr
}
void Sketcher3D::toolBarElements()
{
    // ============================ TOOL BAR =================================
    mToolBar = std::make_unique<QToolBar>(this);
    addToolBar(mToolBar.get());
    mToolBar->setIconSize(QSize(32, 32));

    //Tool buttons
    mCuboidTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/cuboid.png", "Cuboid");
    mCubeTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/cube.png", "Cube");
    mSphereTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/sphere.png", "Sphere");
    mCylinderTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/cylinder.png", "Cylinder");
    mConeTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/cone.png", "Cone");
    mPyramidTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/pyramid.png", "Pyramid");
    

    //mTransformationTool->setPopupMode(QToolButton::MenuButtonPopup);   // IMPORTANT

    //mToolBar->addWidget(mTransformationTool.get());
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
    mLoadSTLAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "LoadSTL");
    mNewAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_FileIcon), "New");
    mNewAction->setShortcut(QKeySequence::New); // Ctrl+N
    mOpenAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DirOpenIcon), "Open");
    mOpenAction->setShortcut(QKeySequence::Open); // Ctrl+O

    // Edit Menu
    QMenu* editMenu = mMenuBar.get()->addMenu("Edit");
    QAction* cleanAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_TrashIcon), "Clean");
    cleanAction->setShortcut(Qt::CTRL | Qt::Key_X); // Ctrl+X
    QAction* undoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowBack), "Undo");
    undoAction->setShortcut(QKeySequence::Undo); // Ctrl+Z
    QAction* redoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowForward), "Redo");
    redoAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Z); // Ctrl+Shift+Z


    // Transformation Menu
    mTransformationMenu = mMenuBar.get()->addMenu("Transformations");
    mTranslate = mTransformationMenu->addAction("Translate");
    mScale = mTransformationMenu->addAction("Scale");
    mRotate = mTransformationMenu->addMenu("Rotate");
    mRotateX = mRotate->addAction("Rotate w.r.t x - Axis");
    mRotateY = mRotate->addAction("Rotate w.r.t y - Axis");
    mRotateZ = mRotate->addAction("Rotate w.r.t z - Axis");
    
}



void Sketcher3D::onCuboidToolClicked()
{
    // Create cuboid 
    try {
        std::shared_ptr<Shape> cb = std::make_shared<Cuboid>(ShapeSlots::cuboidSlot(this));
        shapeManager.addShape(cb);
        //std::vector<Point> vec = cb->getCoordinates();
        std::vector<Point> vec = cb->coodinatesForGLTriangle();
        glWidget->drawShape(vec);
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
        //std::vector<Point> vec = c->getCoordinates();
        //std::vector<Point> vec = c->coodinatesForGLTriangle();
        /*Triangulation cube = c->makeCube();
        std::vector <Point> pts = cube.getPoints();
        std::vector <Triangle> tris = cube.getTriangles();*/
        std::vector <Point> pts = c->mPoints;
        std::vector <Triangle> tris = c->mTriangles;
        glWidget->drawShapeCube(pts, tris);
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
        std::vector<Point> vec = py->coodinatesForGLTriangle();
        glWidget->drawShape(vec);
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
        std::vector<Point> vec = cyl->coodinatesForGLTriangle();
        glWidget->drawShape(vec);
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
        std::vector<Point> vec = cone->coodinatesForGLTriangle();
        glWidget->drawShape(vec);
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
        std::vector<Point> vec = sp->coodinatesForGLTriangle();
        glWidget->drawShape(vec);
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
        QMessageBox::information(this, "Saved", "Shapes saved and rendered in 3D viewer!");
    }
    else
    {
        QMessageBox::warning(this, "Not Saved", "Shapes not saved!");
    }
}


void Sketcher3D::onLoadSTLTriggered()
{
   /**QString qFileName = QFileDialog::getSaveFileName(
       this, "Save Shapes", "", ".stl");*/

       // load stl file
    std::string fileName = "../cube.stl";
    std::vector <Point> pts = FileHandle::readSTL(fileName);

    if (!pts.empty())
    {
        glWidget->drawShape(pts);
        QMessageBox::information(this, "load", "Shapes loaded and rendered in 3D viewer!");
    }
    else
    {
        QMessageBox::warning(this, "Not loaded", "Shapes not loaded!");
    }
}


void Sketcher3D::onSaveActionTriggered()
{
    /*QString qFileName = QFileDialog::getSaveFileName(
        this, "Save Shapes", "", ".skt");*/

    // Save shapes to file
    std::string fileName = "../Shapes.skt";

    std::vector<std::shared_ptr<Shape>> shapesVec = shapeManager.getShapesVec();
    if (FileHandle::saveToFile(fileName, shapesVec))
    {
        QMessageBox::information(this, "Saved", "Shapes saved and rendered in 3D viewer!");
    }
    else
    {
        QMessageBox::warning(this, "Not Saved", "Shapes not saved!");
    }
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



    std::shared_ptr<Shape> shape = shapeManager.getLastShape();
    //shapeManager.addShape(shape);
    std::vector<Point> vec = shape->coodinatesForGLTriangle();

    std::vector<Point> transformed = Transformations::translate(vec, tx, ty, tz);

    glWidget->drawShape(transformed);
    //mGLWidget->setVertices(currentVertices);
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

    std::shared_ptr<Shape> shape = shapeManager.getLastShape();
    std::vector<Point> vec = shape->coodinatesForGLTriangle();

    std::vector<Point> transformed = Transformations::scale(vec, sx, sy, sz);

    glWidget->drawShape(transformed);
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

    double angleX = ((MathConstants::PI * angleSpin->value()) / 180);

    std::shared_ptr<Shape> shape = shapeManager.getLastShape();
    std::vector<Point> vec = shape->coodinatesForGLTriangle();

    std::vector<Point> transformed = Transformations::rotationX(vec, angleX);

    glWidget->drawShape(transformed);
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

    double angleY = ((MathConstants::PI * angleSpin->value()) / 180);

    std::shared_ptr<Shape> shape = shapeManager.getLastShape();
    std::vector<Point> vec = shape->coodinatesForGLTriangle();

    std::vector<Point> transformed = Transformations::rotationY(vec, angleY);

    glWidget->drawShape(transformed);
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

    double angleZ = ((MathConstants::PI * angleSpin->value()) / 180);

    std::shared_ptr<Shape> shape = shapeManager.getLastShape();
    std::vector<Point> vec = shape->coodinatesForGLTriangle();

    std::vector<Point> transformed = Transformations::rotationZ(vec, angleZ);

    glWidget->drawShape(transformed);
}