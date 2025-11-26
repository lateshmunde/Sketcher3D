#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "Sketcher3D.h"
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
    // for add shapes
    // Central widget and layout
    mCentralWidget = new QWidget(this);
    mCentralgridWidget = new QGridLayout(mCentralWidget);
    setCentralWidget(mCentralWidget);

    menuBarElements();
    toolBarElements();

    mStatusBar = new QStatusBar(this);
    setStatusBar(mStatusBar);
    mStatusBar->showMessage("Application Started");


    // Connections
    connect(mCuboidTool, &QToolButton::clicked, this, &Sketcher3D::onCuboidToolClicked);
    connect(mCubeTool, &QToolButton::clicked, this, &Sketcher3D::onCubeToolClicked);
    connect(mConeTool, &QToolButton::clicked, this, &Sketcher3D::onConeToolClicked);
    connect(mCylinderTool, &QToolButton::clicked, this, &Sketcher3D::onCylinderToolClicked);
    connect(mPyramidTool, &QToolButton::clicked, this, &Sketcher3D::onPyramidClicked);
    connect(mSphereTool, &QToolButton::clicked, this, &Sketcher3D::onSphereToolClicked);
    connect(mSaveAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
   
}

void Sketcher3D::menuBarElements()
{
    mMenuBar = new QMenuBar(this);
    setMenuBar(mMenuBar);

    // File Menu
    mFileMenu = mMenuBar->addMenu("File");
    mNewAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_FileIcon), "New");
    mNewAction->setShortcut(QKeySequence::New);   // Ctrl+N
    mOpenAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DirOpenIcon), "Open");
    mOpenAction->setShortcut(QKeySequence::Open);   // Ctrl+O
    mSaveAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save");
    mSaveAction->setShortcut(QKeySequence::Save);   // Ctrl+S

}

void Sketcher3D::toolBarElements()
{
    mToolBar = new QToolBar(this);
    addToolBar(mToolBar);

    //Cuboid Tool
    mCuboidTool = new QToolButton(mToolBar);
    mCuboidTool->setIcon(QIcon("Cuboid"));
    mCuboidTool->setText("Cuboid");
    mCuboidTool->setIconSize(QSize(32, 32));
    mCuboidTool->setIconSize(QSize(32, 32));
    mCuboidTool->setToolTip("Cuboid");
    mToolBar->addWidget(mCuboidTool);

    //Cube Tool
    mCubeTool = new QToolButton(mToolBar);
    mCubeTool->setIcon(QIcon("Cube"));
    mCubeTool->setIconSize(QSize(32, 32));
    mCubeTool->setToolTip("Cube");
    mToolBar->addWidget(mCubeTool);

    //Sphere Tool
    mSphereTool = new QToolButton(mToolBar);
    mSphereTool->setIcon(QIcon("Sphere"));
    mSphereTool->setIconSize(QSize(32, 32));
    mSphereTool->setToolTip("Sphere");
    mToolBar->addWidget(mSphereTool);

    //Cylinder Tool
    mCylinderTool = new QToolButton(mToolBar);
    mCylinderTool->setIcon(QIcon("Cylinder"));
    mCylinderTool->setIconSize(QSize(32, 32));
    mCylinderTool->setToolTip("Cylinder");
    mToolBar->addWidget(mCylinderTool);

    //Cone Tool
    mConeTool = new QToolButton(mToolBar);
    mConeTool->setIcon(QIcon("Cone"));
    mConeTool->setIconSize(QSize(32, 32));
    mConeTool->setToolTip("Cone");
    mToolBar->addWidget(mConeTool);

    //Pyramid Tool
    mPyramidTool = new QToolButton(mToolBar);
    mPyramidTool->setIcon(QIcon("Pyramid"));
    mPyramidTool->setIconSize(QSize(32, 32));
    mPyramidTool->setToolTip("Pyramid");
    mToolBar->addWidget(mPyramidTool);
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