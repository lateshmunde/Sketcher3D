#include "stdafx.h"
#include "Sketcher3D.h"
#include <iostream>
#include <vector>
#include "ShapeSlots.h"

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



std::unique_ptr<QDoubleSpinBox> Sketcher3D::dimensionField(QDialog& dlg,
    const QString& name, std::unique_ptr<QFormLayout>& layout)
{
    // Dimension
    std::unique_ptr<QDoubleSpinBox> dimensionSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    dimensionSpin->setRange(0.1, 10000);
    dimensionSpin->setValue(10.0);
    layout->addRow(name + " :", dimensionSpin.get());
    return dimensionSpin;
}

void Sketcher3D::dialogButtonBoxFn(QDialog& dlg, std::unique_ptr<QFormLayout>& layout)
{
    // OK / Cancel buttons
    std::unique_ptr<QDialogButtonBox> buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons.get());

    QObject::connect(buttons.get(), &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons.get(), &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    // Show dialog
    if (dlg.exec() != QDialog::Accepted)
        return;
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
}

void Sketcher3D::onCuboidToolClicked()
{
    // Create cuboid 
    std::shared_ptr<Shape> cb = std::make_shared<Cuboid>(ShapeSlots::cuboidSlot(this));
    shapeManager.addShape(cb);
    glWidget->drawShape(cb);
    mStatusBar->showMessage("Cuboid created");
}

void Sketcher3D::onCubeToolClicked()
{
    // Create Cube object
    std::shared_ptr<Shape> c = std::make_shared<Cube>(ShapeSlots::cubeSlot(this));
    shapeManager.addShape(c);
    glWidget->drawShape(c);
    mStatusBar->showMessage("Cube created");
}

void Sketcher3D::onPyramidClicked()
{
    // Create Pyramid
    std::shared_ptr<Shape> py= std::make_shared<Pyramid>(ShapeSlots::pyramidSlot(this));
    shapeManager.addShape(py);
    glWidget->drawShape(py);
    mStatusBar->showMessage("Pyramid created");
}

void Sketcher3D::onCylinderToolClicked()
{
    // Create Cylinder object
    std::shared_ptr<Shape> cyl = std::make_shared<Cylinder>(ShapeSlots::cylinderSlot(this));
    shapeManager.addShape(cyl);
    glWidget->drawShape(cyl);
    mStatusBar->showMessage("Cylinder created");
}

void Sketcher3D::onConeToolClicked()
{
    // Create Cone object
    std::shared_ptr<Shape> cone = std::make_shared<Cone>(ShapeSlots::coneSlot(this));
    shapeManager.addShape(cone);
    glWidget->drawShape(cone);
    mStatusBar->showMessage("Cone created");
}

void Sketcher3D::onSphereToolClicked()
{
    // Create Sphere object
    try {
        std::shared_ptr<Shape> sp = std::make_shared<Sphere>(ShapeSlots::sphereSlot(this));
        shapeManager.addShape(sp);
        glWidget->drawShape(sp);
        mStatusBar->showMessage("Sphere created");
    }
    catch (...) {
        // Do nothing — user cancelled
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