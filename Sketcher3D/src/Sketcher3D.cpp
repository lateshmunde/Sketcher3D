#include "stdafx.h"
#include "Sketcher3D.h"

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
    mCentralWidget = new QWidget(this);
    mCentralgridWidget = new QGridLayout(mCentralWidget);
    
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
   


}

void Sketcher3D::menuBarElements()
{
    // File Menu
    QMenu* fileMenu = mMenuBar->addMenu("File");
    QAction* newAction = fileMenu->addAction("New");
    QAction* openAction = fileMenu->addAction("Open");
    QAction* saveAction = fileMenu->addAction("Save");
}

void Sketcher3D::toolBarElements()
{
    mToolBar = new QToolBar(this);
    addToolBar(mToolBar);

    //Cuboid Tool
    mCuboidTool = new QToolButton(mToolBar);
    mCuboidTool->setIcon(QIcon("Cuboid"));
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

}

void Sketcher3D::onCubeToolClicked()
{

}

void Sketcher3D::onConeToolClicked()
{

}

void Sketcher3D::onCylinderToolClicked()
{

}

void Sketcher3D::onPyramidClicked()
{

}

void Sketcher3D::onSphereToolClicked()
{

}