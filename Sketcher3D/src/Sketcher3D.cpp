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
    glWidget = new GLWidget(this);
    glWidget->setMinimumSize(800, 600);

    // Add OpenGL widget to the central grid layout
    // This places it in row 0, column 0 of the grid
    mCentralgridWidget->addWidget(glWidget, 0, 0);
    // ================================================

    mStatusBar = std::make_unique<QStatusBar>(this);
    setStatusBar(mStatusBar.get());
    mStatusBar->showMessage("Application Started - 3D Viewer Ready");

    // Connections
    connect(mCuboidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCuboidToolClicked);
    connect(mCubeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCubeToolClicked);
    connect(mConeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onConeToolClicked);
    connect(mCylinderTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCylinderToolClicked);
    connect(mPyramidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onPyramidClicked);
    connect(mSphereTool.get(), &QToolButton::clicked, this, &Sketcher3D::onSphereToolClicked);

    connect(mTransformationTool.get(), &QToolButton::clicked, this, &Sketcher3D::onTransformToolClicked);
    connect(mTranslateTool.get(), &QToolButton::clicked, this, &Sketcher3D::onTranslateToolClicked);
    connect(mScaleTool.get(), &QToolButton::clicked, this, &Sketcher3D::onScaleToolClicked);
    connect(mRotateXTool.get(), &QToolButton::clicked, this, &Sketcher3D::onRotateXToolClicked);
    connect(mRotateYTool.get(), &QToolButton::clicked, this, &Sketcher3D::onRotateYToolClicked);
    connect(mRotateZTool.get(), &QToolButton::clicked, this, &Sketcher3D::onRotateZToolClicked);
    
    connect(mSaveGNUAction, &QAction::triggered, this, &Sketcher3D::onSaveGNUActionTriggered);
    connect(mSaveAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
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
    mTransformationTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/transformation.png", "Transform");
    mTranslateTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/transformation.png", "Translate");
    mScaleTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/transformation.png", "Scale");
    mRotateXTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/transformation.png", "RotateX");
    mRotateYTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/transformation.png", "RotateY");
    mRotateZTool = createToolButton(mToolBar.get(), ":/Sketcher3D/Resources/transformation.png", "RotateZ");

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


    // Transform Menu
    /*mTransformationTool.get()->setMenu(mTransformMenu);

    mTransformMenu = new QMenu(mTransformationTool.get());

    mRotateXAction = new QAction("RotateX", this);
    mRotateYAction = new QAction("RotateY", this);
    mRotateZAction = new QAction("RotateZ", this);
    mScaleAction = new QAction("Scale", this);
    mTranslateAction = new QAction("Translate", this);
  
    mTransformMenu->addAction(mTranslateAction);
    mTransformMenu->addAction(mScaleAction);
    mTransformMenu->addSeparator();
    mTransformMenu->addAction(mRotateXAction);
    mTransformMenu->addAction(mRotateYAction);
    mTransformMenu->addAction(mRotateZAction);*/
    
    

}



void Sketcher3D::onCuboidToolClicked()
{
    // Create cuboid 
    try {
        std::shared_ptr<Shape> cb = std::make_shared<Cuboid>(ShapeSlots::cuboidSlot(this));
        shapeManager.addShape(cb);
        std::vector<Point> vec = cb->getCoordinates();
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
        std::vector<Point> vec = c->getCoordinates();
        glWidget->drawShape(vec);
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
        std::vector<Point> vec = py->getCoordinates();
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
        std::vector<Point> vec = cyl->getCoordinates();
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
        std::vector<Point> vec = cone->getCoordinates();
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
        std::vector<Point> vec = sp->getCoordinates();
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


void Sketcher3D::onTransformToolClicked()
{
    std::shared_ptr<Shape> py = std::make_shared<Pyramid>("py1", 2, 3, 6);
    //shapeManager.addShape(py);
    std::vector<Point> vec = py->getCoordinates();
    vec = Transformations::getPtMatrix(vec);
    glWidget->drawShape(vec);
    mStatusBar->showMessage("Transform Done");
}



void Sketcher3D::onTranslateToolClicked()
{
    QMessageBox::information(this, "WIP", "Work in progress");
}

void Sketcher3D::onScaleToolClicked()
{
    QMessageBox::information(this, "WIP", "Work in progress");
}

void Sketcher3D::onRotateXToolClicked()
{
    QMessageBox::information(this, "WIP", "Work in progress");
}

void Sketcher3D::onRotateYToolClicked()
{
    QMessageBox::information(this, "WIP", "Work in progress");
}

void Sketcher3D::onRotateZToolClicked()
{
    QMessageBox::information(this, "WIP", "Work in progress");
}