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
    // ui.setupUi(this);

    setupUI();
    resize(800, 600);
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
    // Create dialog
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cuboid");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Name field
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Cuboid1", &dlg);
    layout->addRow("Cuboid Name:", nameEdit.get());

    // Length
    std::unique_ptr<QDoubleSpinBox> lengthSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    lengthSpin->setRange(0.1, 10000);
    lengthSpin->setValue(10.0);
    layout->addRow("Length: ", lengthSpin.get());

    //  Width
    std::unique_ptr<QDoubleSpinBox> widthSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    widthSpin.get()->setRange(0.1, 10000);
    widthSpin.get()->setValue(10.0);
    layout->addRow(" Width:", widthSpin.get());

    // Height
    std::unique_ptr<QDoubleSpinBox> heightSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    heightSpin->setRange(0.1, 10000);
    heightSpin->setValue(10.0);
    layout->addRow(" Height:", heightSpin.get());

    // OK / Cancel buttons
    std::unique_ptr<QDialogButtonBox> buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);

    layout->addRow(buttons.get());

    QObject::connect(buttons.get(), &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons.get(), &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    // Show dialog
    if (dlg.exec() != QDialog::Accepted)
        return;

    // Extract values
    QString name = nameEdit->text();
    double length = lengthSpin->value();
    double width = widthSpin.get()->value();
    double height = heightSpin.get()->value();

    // Create cuboid 
    std::shared_ptr<Shape> cb = std::make_shared<Cuboid>(name.toStdString(), length, width, height);
    shapeManager.addShape(cb);

    QMessageBox::information(this, "Success", "Cuboid created.");
}

void Sketcher3D::onCubeToolClicked()
{

    // Create dialog
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cube");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Name 
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Cube1", &dlg);
    layout.get()->addRow("Cube Name:", nameEdit.get());

    // side
    std::unique_ptr<QDoubleSpinBox> lengthSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    lengthSpin->setRange(0.1, 10000);
    lengthSpin->setValue(10.0);
    layout->addRow("Length: ", lengthSpin.get());
  

    // OK / Cancel buttons
    QDialogButtonBox* buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);

    QObject::connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    // Show dialog
    if (dlg.exec() != QDialog::Accepted)
        return;

    // Get values
    QString name = nameEdit->text();
    double side = lengthSpin->value();

    // Create Cube object
    std::shared_ptr<Shape> c = std::make_shared<Cube>(name.toStdString(), side);
    shapeManager.addShape(c);

    // Inform user
    QMessageBox::information(this, "Success", "cube created");

}

void Sketcher3D::onPyramidClicked()
{
    // Create dialog
    QDialog dlg(this);
    dlg.setWindowTitle("Create Pyramid");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Name
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Pyramid1", &dlg);
    layout.get()->addRow("Pyramid Name:", nameEdit.get());

    // Base Length
    std::unique_ptr<QDoubleSpinBox> lengthSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    lengthSpin->setRange(0.1, 10000);
    lengthSpin->setValue(10.0);
    layout.get()->addRow("Base Length:", lengthSpin.get());

    // Base Width
    std::unique_ptr<QDoubleSpinBox> widthSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    widthSpin.get()->setRange(0.1, 10000);
    widthSpin.get()->setValue(10.0);
    layout->addRow("Base Width:", widthSpin.get());

    // Height
    std::unique_ptr<QDoubleSpinBox> heightSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    heightSpin.get()->setRange(0.1, 10000);
    heightSpin.get()->setValue(10.0);
    layout->addRow("Base Height:", heightSpin.get());

    // OK / Cancel buttons
    std::unique_ptr<QDialogButtonBox> buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);

    layout->addRow(buttons.get());

    QObject::connect(buttons.get(), &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons.get(), &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    // Show dialog
    if (dlg.exec() != QDialog::Accepted)
        return;

    // Get values
    QString name = nameEdit->text();
    double baseLength = lengthSpin->value();
    double baseWidth = widthSpin->value();
    double height = heightSpin->value();

    // Create Pyramid
    std::shared_ptr<Shape> py = std::make_shared<Pyramid>(name.toStdString(), baseLength, baseWidth, height);
    shapeManager.addShape(py);

    // Inform user
    QMessageBox::information(this, "Success", "Pyramid created.");
}

void Sketcher3D::onCylinderToolClicked()
{
    // Create dialog
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cylinder");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Cylinder  Name
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Cylinder1", &dlg);
    layout->addRow("Cylinder Name:", nameEdit.get());

    // Radius
    std::unique_ptr<QDoubleSpinBox> radiusSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    radiusSpin->setRange(0.1, 10000);
    radiusSpin->setValue(10.0);
    layout->addRow("Radius:", radiusSpin.get());

    // Height
    std::unique_ptr<QDoubleSpinBox> heightSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    heightSpin->setRange(0.1, 10000);
    heightSpin->setValue(10.0);
    layout->addRow("Height:", heightSpin.get());

    // OK / Cancel buttons
    std::unique_ptr<QDialogButtonBox> buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);

    layout->addRow(buttons.get());

    QObject::connect(buttons.get(), &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons.get(), &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    // Show dialog
    if (dlg.exec() != QDialog::Accepted)
        return;

    // Get values
    const QString name = nameEdit->text();
    const double radius = radiusSpin->value();
    const double height = heightSpin->value();

    // Create Cylinder object (from DLL)
    std::shared_ptr<Shape> cyl = std::make_shared<Cylinder>(name.toStdString(), radius, height);
    shapeManager.addShape(cyl);

    // Done
    QMessageBox::information(this, "Success", "Cylinder created.");
}

void Sketcher3D::onConeToolClicked()
{
    // Create dialog
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cone");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Name
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Cone1", &dlg);
    layout->addRow("Cone Name:", nameEdit.get());

    // Radius
    std::unique_ptr<QDoubleSpinBox> radiusSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    radiusSpin->setRange(0.1, 10000.0);
    radiusSpin->setValue(10.0);
    layout->addRow("Radius:", radiusSpin.get());

    // Height
    std::unique_ptr<QDoubleSpinBox> heightSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    heightSpin->setRange(0.1, 10000.0);
    heightSpin->setValue(10.0);
    layout->addRow("Height:", heightSpin.get());

    // OK / Cancel buttons
    std::unique_ptr<QDialogButtonBox> buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);

    layout->addRow(buttons.get());

    QObject::connect(buttons.get(), &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons.get(), &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    // Show dialog
    if (dlg.exec() != QDialog::Accepted)
        return;

    // Get values
    const QString name = nameEdit->text();
    const double radius = radiusSpin->value();
    const double height = heightSpin->value();

    // Create Cone object (from geometry library)
    std::shared_ptr<Shape> cone = std::make_shared<Cone>(name.toStdString(), radius, height);
    shapeManager.addShape(cone);

    // Inform user
    QMessageBox::information(this, "Success", "Cone Created.");
}

void Sketcher3D::onSphereToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Sphere");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Sphere  Name
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Sphere1", &dlg);
    layout->addRow("Sphere Name:", nameEdit.get());

    // Radius
    std::unique_ptr<QDoubleSpinBox> radiusSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    radiusSpin->setRange(0.1, 10000);
    radiusSpin->setValue(10.0);
    layout->addRow("Radius:", radiusSpin.get());


    // OK / Cancel buttons
    std::unique_ptr<QDialogButtonBox> buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);

    layout->addRow(buttons.get());

    QObject::connect(buttons.get(), &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons.get(), &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    // Show dialog
    if (dlg.exec() != QDialog::Accepted)
        return;

    // Get values
    const QString name = nameEdit->text();
    const double radius = radiusSpin->value();

    // Create Sphere object (from DLL)
    std::shared_ptr<Shape> sp = std::make_shared<Sphere>(name.toStdString(), radius);
    shapeManager.addShape(sp);

    // Done
    QMessageBox::information(this, "Success", "Sphere created.");

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