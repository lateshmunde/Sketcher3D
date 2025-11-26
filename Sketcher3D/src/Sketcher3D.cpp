#include "stdafx.h"
#include "Sketcher3D.h"
#include <iostream>
#include <vector>
#include "Cube.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Pyramid.h"

Sketcher3D::Sketcher3D(QWidget *parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);

    setupUI();
    resize(800, 600);


}

Sketcher3D::~Sketcher3D()
{}

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
    connect(saveAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
   
}

void Sketcher3D::menuBarElements()
{
    // ============================ MENU BAR =================================
    mMenuBar = std::make_unique<QMenuBar>(this);
    setMenuBar(mMenuBar.get());

    // File Menu
    QMenu* fileMenu = mMenuBar.get()->addMenu("File");
    QAction* newAction = fileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_FileIcon), "New");
    newAction->setShortcut(QKeySequence::New);   // Ctrl+N
    QAction* openAction = fileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DirOpenIcon), "Open");
    openAction->setShortcut(QKeySequence::Open);   // Ctrl+O
    QAction* saveAction = fileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save");
    saveAction->setShortcut(QKeySequence::Save);   // Ctrl+S

    // Edit Menu
    QMenu* editMenu = mMenuBar.get()->addMenu("Edit"); // Local pointer editMenu, no need to delete
    QAction* cleanAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_TrashIcon), "Clean");
    cleanAction->setShortcut(Qt::CTRL | Qt::Key_X); // Ctrl+X
    QAction* undoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowBack), "Undo");
    undoAction->setShortcut(QKeySequence::Undo);   // Ctrl+Z
    QAction* redoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowForward), "Redo");
    redoAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Z);   // Ctrl+Shift+Z/
}

void Sketcher3D::toolBarElements()
{
    // ============================ TOOL BAR =================================
    mToolBar = std::make_unique<QToolBar>(this);
    addToolBar(mToolBar.get());
    mToolBar->setIconSize(QSize(48, 48));

    //Cuboid Tool
    mCuboidTool = std::make_unique<QToolButton>(mToolBar.get());
    mCuboidTool.get()->setIcon(QIcon(":/Sketcher3D/Rec/cuboid.png"));
    mCuboidTool.get()->setIconSize(QSize(48, 48));
    mCuboidTool.get()->setToolTip("Cuboid");
    mToolBar.get()->addWidget(mCuboidTool.get());

    //Cube Tool
    mCubeTool = std::make_unique<QToolButton>(mToolBar.get());
    mCubeTool.get()->setIcon(QIcon(":/Sketcher3D/Rec/cube.png"));
    mCubeTool.get()->setIconSize(QSize(48, 48));
    mCubeTool.get()->setToolTip("Cube");
    mToolBar.get()->addWidget(mCubeTool.get());

    //Sphere Tool
    mSphereTool = std::make_unique<QToolButton>(mToolBar.get());
    mSphereTool.get()->setIcon(QIcon(":/Sketcher3D/Rec/sphere.png"));
    mSphereTool.get()->setIconSize(QSize(48, 48));
    mSphereTool.get()->setToolTip("Sphere");
    mToolBar.get()->addWidget(mSphereTool.get());

    //Cylinder Tool
    mCylinderTool = std::make_unique<QToolButton>(mToolBar.get());
    mCylinderTool.get()->setIcon(QIcon(":/Sketcher3D/Rec/cylinder.png"));
    mCylinderTool.get()->setIconSize(QSize(48, 48));
    mCylinderTool.get()->setToolTip("Cylinder");
    mToolBar.get()->addWidget(mCylinderTool.get());

    //Cone Tool
    mConeTool = std::make_unique<QToolButton>(mToolBar.get());
    mConeTool.get()->setIcon(QIcon(":/Sketcher3D/Rec/cone.png"));
    mConeTool.get()->setIconSize(QSize(48, 48));
    mConeTool.get()->setToolTip("Cone");
    mToolBar.get()->addWidget(mConeTool.get());

    //Pyramid Tool
    mPyramidTool = std::make_unique<QToolButton>(mToolBar.get());
    mPyramidTool.get()->setIcon(QIcon(":/Sketcher3D/Rec/pyramid.png"));
    mPyramidTool.get()->setIconSize(QSize(48, 48));
    mPyramidTool.get()->setToolTip("Pyramid");
    mToolBar.get()->addWidget(mPyramidTool.get());
}

void Sketcher3D::onCuboidToolClicked()
{
    QMessageBox::information(this, "Done", "sphere.dat created!");
}

void Sketcher3D::onCubeToolClicked()
{
    QMessageBox::information(this, "Done", "sphere.dat created!");
}

void Sketcher3D::onConeToolClicked()
{
    QMessageBox::information(this, "Done", "sphere.dat created!");
}

void Sketcher3D::onCylinderToolClicked()
{
    QMessageBox::information(this, "Done", "sphere.dat created!");
}

void Sketcher3D::onPyramidClicked()
{
    QMessageBox::information(this, "Done", "sphere.dat created!");
}

void Sketcher3D::onSphereToolClicked()
{
    bool okName = false;
    QString qname = QInputDialog::getText(
        this,
        "Sphere Name",
        "Enter sphere name:",
        QLineEdit::Normal,
        "Sphere1",
        &okName);

    if (!okName || qname.isEmpty())
        return;

    std::string name = qname.toStdString();

    bool ok = false;
    double radius = QInputDialog::getDouble(
        this,
        "Sphere Radius",
        "Enter radius:",
        5.0,      // default value
        1.0,      // min
        100.0,  // max
        2,
        &ok);

    if (!ok) return;

     
    std::shared_ptr<Shape> s = std::make_shared<Sphere>(name, radius);
    mgr.addShape(s);
    //Create shapes using ShapeCreator
    /*std::shared_ptr<Shape> S
        = std::make_shared<Shape>(ShapeCreator::createSphere(name, radius));*/
    
}

void Sketcher3D::onSaveActionTriggered()
{
    /*QString qFileName = QFileDialog::getSaveFileName(
        this, "Save Shapes", "", ".dat");*/
  
    //Save shapes to file (GNU Plot)
    std::string fileName = "../ShapesGNU.dat";

    std::vector<std::shared_ptr<Shape>> shapesVec = mgr.getShapesVec();
    if (FileHandle::saveToFileGNUPlot(fileName, shapesVec)) {
        QMessageBox::information(this, "Saved", "Shapes saved successfully!");
    }
    else
        QMessageBox::information(this, "Not Saved", "Shapes not saved!");

}