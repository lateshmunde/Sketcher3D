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
   


}

void Sketcher3D::menuBarElements()
{
    mMenuBar = std::make_unique<QMenuBar>(this);
    setMenuBar(mMenuBar.get());

    // File Menu
    //QMenu* fileMenu = mMenuBar->addMenu("File");
    std::unique_ptr<QMenu> fileMenu;
    fileMenu = std::make_unique<QMenu>(mMenuBar->addMenu("File"));

    std::unique_ptr<QAction> newAction;
    newAction = std::make_unique<QAction>(fileMenu->addAction("New"));
    std::unique_ptr<QAction> openAction;
    openAction = std::make_unique<QAction>(fileMenu->addAction("Open"));
    std::unique_ptr<QAction> saveAction;
    saveAction = std::make_unique<QAction>(fileMenu->addAction("Save"));

    /*QMenu* fileMenu = mMenuBar->addMenu("File");
    QAction* newAction = fileMenu->addAction("New");
    QAction* openAction = fileMenu->addAction("Open");
    QAction* saveAction = fileMenu->addAction("Save");*/
}

void Sketcher3D::toolBarElements()
{
    mToolBar = std::make_unique<QToolBar>(this);
    addToolBar(mToolBar.get());

    //Cuboid Tool
    mCuboidTool = std::make_unique<QToolButton>(this);
    mCuboidTool->setIcon(QIcon("Cuboid"));
    mCuboidTool->setIconSize(QSize(32, 32));
    mCuboidTool->setToolTip("Cuboid");
    mToolBar->addWidget(mCuboidTool.get());

    //Cube Tool
    mCubeTool = std::make_unique<QToolButton>(this);
    mCubeTool->setIcon(QIcon("Cube"));
    mCubeTool->setIconSize(QSize(32, 32));
    mCubeTool->setToolTip("Cube");
    mToolBar->addWidget(mCubeTool.get());

    //Sphere Tool
    mSphereTool = std::make_unique<QToolButton>(this);
    mSphereTool->setIcon(QIcon("Sphere"));
    mSphereTool->setIconSize(QSize(32, 32));
    mSphereTool->setToolTip("Sphere");
    mToolBar->addWidget(mSphereTool.get());

    //Cylinder Tool
    mCylinderTool = std::make_unique<QToolButton>(this);
    mCylinderTool->setIcon(QIcon("Cylinder"));
    mCylinderTool->setIconSize(QSize(32, 32));
    mCylinderTool->setToolTip("Cylinder");
    mToolBar->addWidget(mCylinderTool.get());

    //Cone Tool
    mConeTool = std::make_unique<QToolButton>(this);
    mConeTool->setIcon(QIcon("Cone"));
    mConeTool->setIconSize(QSize(32, 32));
    mConeTool->setToolTip("Cone");
    mToolBar->addWidget(mConeTool.get());

    //Pyramid Tool
    mPyramidTool = std::make_unique<QToolButton>(this);
    mPyramidTool->setIcon(QIcon("Pyramid"));
    mPyramidTool->setIconSize(QSize(32, 32));
    mPyramidTool->setToolTip("Pyramid");
    mToolBar->addWidget(mPyramidTool.get());
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