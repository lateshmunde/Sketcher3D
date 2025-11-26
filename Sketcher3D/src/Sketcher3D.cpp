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
    
    mMenuBar = std::make_unique<QMenuBar>(this);
    setMenuBar(mMenuBar.get());

    mToolBar = std::make_unique<QToolBar>(this);
    addToolBar(mToolBar.get());

    mStatusBar = std::make_unique<QStatusBar>(this);
    setStatusBar(mStatusBar.get());
    mStatusBar->showMessage("Application Started");

    // File Menu
    QMenu* fileMenu = mMenuBar->addMenu("File");

    QAction* newAction = fileMenu->addAction("New");
    QAction* openAction = fileMenu->addAction("Open");
    QAction* saveAction = fileMenu->addAction("Save");
    //std::unique_ptr<QMenu> fileMenu = mMenuBar->addMenu("File");

}