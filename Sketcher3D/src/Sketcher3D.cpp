#include "stdafx.h"
#include "Sketcher3D.h"
#include <iostream>
#include <vector>
#include <QStyle>

#include "Cube.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Pyramid.h"

Sketcher3D::Sketcher3D(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    resize(800, 600);
}

Sketcher3D::~Sketcher3D() {}

void Sketcher3D::setupUI()
{
    mCentralWidget = std::make_unique<QWidget>(this);
    mCentralgridWidget = std::make_unique<QGridLayout>(mCentralWidget.get());
    setCentralWidget(mCentralWidget.get());

    menuBarElements();
    toolBarElements();

    mStatusBar = std::make_unique<QStatusBar>(this);
    setStatusBar(mStatusBar.get());
    mStatusBar->showMessage("Application Started");

    connect(mCuboidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCuboidToolClicked);
    connect(mCubeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCubeToolClicked);
    connect(mConeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onConeToolClicked);
    connect(mCylinderTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCylinderToolClicked);
    connect(mPyramidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onPyramidClicked);
    connect(mSphereTool.get(), &QToolButton::clicked, this, &Sketcher3D::onSphereToolClicked);

    connect(mSaveGNUAction, &QAction::triggered, this, &Sketcher3D::onSaveGNUActionTriggered);
    connect(mSaveAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);

    mGLWidget = std::make_unique<OpenGLWidget>(this);
    mCentralgridWidget->addWidget(mGLWidget.get(), 0, 0);
}

void Sketcher3D::menuBarElements()
{
    mMenuBar = std::make_unique<QMenuBar>(this);
    setMenuBar(mMenuBar.get());

    mFileMenu = mMenuBar->addMenu("File");
    mSaveMenu = mFileMenu->addMenu("Save");
    mSaveAction = mSaveMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save");
    mSaveAction->setShortcut(QKeySequence::Save);
    mSaveGNUAction = mSaveMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "SaveGNU");
    mNewAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_FileIcon), "New");
    mNewAction->setShortcut(QKeySequence::New);
    mOpenAction = mFileMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_DirOpenIcon), "Open");
    mOpenAction->setShortcut(QKeySequence::Open);

    QMenu* editMenu = mMenuBar->addMenu("Edit");
    QAction* cleanAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_TrashIcon), "Clean");
    cleanAction->setShortcut(Qt::CTRL | Qt::Key_X);
    QAction* undoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowBack), "Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    QAction* redoAction = editMenu->addAction(mMenuBar->style()->standardIcon(QStyle::SP_ArrowForward), "Redo");
    redoAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Z);

    // Optional mirror in menubar
    mTransformationMenu = mMenuBar->addMenu("Transformations");
    QAction* t1 = mTransformationMenu->addAction("Translation");
    QAction* t2 = mTransformationMenu->addAction("Rotation");
    QAction* t3 = mTransformationMenu->addAction("Scaling");
    connect(t1, &QAction::triggered, this, &Sketcher3D::onTranslateTriggered);
    connect(t2, &QAction::triggered, this, &Sketcher3D::onRotateTriggered);
    connect(t3, &QAction::triggered, this, &Sketcher3D::onScaleTriggered);

    QAction* t4 = mTransformationMenu->addAction("Rot_arb_z");
    connect(t4, &QAction::triggered, this, &Sketcher3D::onRotateArbZTriggered);
}

void Sketcher3D::toolBarElements()
{
    mToolBar = std::make_unique<QToolBar>(this);
    addToolBar(mToolBar.get());
    mToolBar->setIconSize(QSize(32, 32));

    mCuboidTool = std::make_unique<QToolButton>(mToolBar.get());
    mCuboidTool->setIcon(QIcon(":/Sketcher3D/Resources/cuboid.png"));
    mCuboidTool->setIconSize(QSize(32, 32));
    mCuboidTool->setToolTip("Cuboid");
    mToolBar->addWidget(mCuboidTool.get());

    mCubeTool = std::make_unique<QToolButton>(mToolBar.get());
    mCubeTool->setIcon(QIcon(":/Sketcher3D/Resources/cube.png"));
    mCubeTool->setIconSize(QSize(32, 32));
    mCubeTool->setToolTip("Cube");
    mToolBar->addWidget(mCubeTool.get());

    mSphereTool = std::make_unique<QToolButton>(mToolBar.get());
    mSphereTool->setIcon(QIcon(":/Sketcher3D/Resources/sphere.png"));
    mSphereTool->setIconSize(QSize(32, 32));
    mSphereTool->setToolTip("Sphere");
    mToolBar->addWidget(mSphereTool.get());

    mCylinderTool = std::make_unique<QToolButton>(mToolBar.get());
    mCylinderTool->setIcon(QIcon(":/Sketcher3D/Resources/cylinder.png"));
    mCylinderTool->setIconSize(QSize(32, 32));
    mCylinderTool->setToolTip("Cylinder");
    mToolBar->addWidget(mCylinderTool.get());

    mConeTool = std::make_unique<QToolButton>(mToolBar.get());
    mConeTool->setIcon(QIcon(":/Sketcher3D/Resources/cone.png"));
    mConeTool->setIconSize(QSize(32, 32));
    mConeTool->setToolTip("Cone");
    mToolBar->addWidget(mConeTool.get());

    mPyramidTool = std::make_unique<QToolButton>(mToolBar.get());
    mPyramidTool->setIcon(QIcon(":/Sketcher3D/Resources/pyramid.png"));
    mPyramidTool->setIconSize(QSize(32, 32));
    mPyramidTool->setToolTip("Pyramid");
    mToolBar->addWidget(mPyramidTool.get());

}

void Sketcher3D::onCuboidToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cuboid");
    auto layout = new QFormLayout(&dlg);

    auto nameEdit = new QLineEdit("Cuboid1", &dlg);
    layout->addRow("Cuboid Name:", nameEdit);

    auto lengthSpin = new QDoubleSpinBox(&dlg);
    lengthSpin->setRange(0.1, 10000);
    lengthSpin->setValue(10.0);
    layout->addRow("Length:", lengthSpin);

    auto widthSpin = new QDoubleSpinBox(&dlg);
    widthSpin->setRange(0.1, 10000);
    widthSpin->setValue(10.0);
    layout->addRow("Width:", widthSpin);

    auto heightSpin = new QDoubleSpinBox(&dlg);
    heightSpin->setRange(0.1, 10000);
    heightSpin->setValue(10.0);
    layout->addRow("Height:", heightSpin);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    auto cb = std::make_shared<Cuboid>(nameEdit->text().toStdString(),
        lengthSpin->value(), widthSpin->value(), heightSpin->value());
    shapeManager.addShape(cb);
    QMessageBox::information(this, "Success", "Cuboid created.");
}

void Sketcher3D::onCubeToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cube");
    auto layout = new QFormLayout(&dlg);

    auto nameEdit = new QLineEdit("Cube1", &dlg);
    layout->addRow("Cube Name:", nameEdit);

    auto sideSpin = new QDoubleSpinBox(&dlg);
    sideSpin->setRange(0.1, 10000);
    sideSpin->setValue(10.0);
    layout->addRow("Side:", sideSpin);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    auto c = std::make_shared<Cube>(nameEdit->text().toStdString(), sideSpin->value());
    shapeManager.addShape(c);
    QMessageBox::information(this, "Success", "Cube created.");
}

void Sketcher3D::onPyramidClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Pyramid");
    auto layout = new QFormLayout(&dlg);

    auto nameEdit = new QLineEdit("Pyramid1", &dlg);
    layout->addRow("Pyramid Name:", nameEdit);

    auto lengthSpin = new QDoubleSpinBox(&dlg);
    lengthSpin->setRange(0.1, 10000);
    lengthSpin->setValue(10.0);
    layout->addRow("Base Length:", lengthSpin);

    auto widthSpin = new QDoubleSpinBox(&dlg);
    widthSpin->setRange(0.1, 10000);
    widthSpin->setValue(10.0);
    layout->addRow("Base Width:", widthSpin);

    auto heightSpin = new QDoubleSpinBox(&dlg);
    heightSpin->setRange(0.1, 10000);
    heightSpin->setValue(10.0);
    layout->addRow("Height:", heightSpin);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    auto py = std::make_shared<Pyramid>(nameEdit->text().toStdString(),
        lengthSpin->value(), widthSpin->value(), heightSpin->value());
    shapeManager.addShape(py);
    QMessageBox::information(this, "Success", "Pyramid created.");
}

void Sketcher3D::onCylinderToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cylinder");
    auto layout = new QFormLayout(&dlg);

    auto nameEdit = new QLineEdit("Cylinder1", &dlg);
    layout->addRow("Cylinder Name:", nameEdit);

    auto radiusSpin = new QDoubleSpinBox(&dlg);
    radiusSpin->setRange(0.1, 10000);
    radiusSpin->setValue(10.0);
    layout->addRow("Radius:", radiusSpin);

    auto heightSpin = new QDoubleSpinBox(&dlg);
    heightSpin->setRange(0.1, 10000);
    heightSpin->setValue(10.0);
    layout->addRow("Height:", heightSpin);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    auto cyl = std::make_shared<Cylinder>(nameEdit->text().toStdString(),
        radiusSpin->value(), heightSpin->value());
    shapeManager.addShape(cyl);
    QMessageBox::information(this, "Success", "Cylinder created.");
}

void Sketcher3D::onConeToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cone");
    auto layout = new QFormLayout(&dlg);

    auto nameEdit = new QLineEdit("Cone1", &dlg);
    layout->addRow("Cone Name:", nameEdit);

    auto radiusSpin = new QDoubleSpinBox(&dlg);
    radiusSpin->setRange(0.1, 10000.0);
    radiusSpin->setValue(10.0);
    layout->addRow("Radius:", radiusSpin);

    auto heightSpin = new QDoubleSpinBox(&dlg);
    heightSpin->setRange(0.1, 10000.0);
    heightSpin->setValue(10.0);
    layout->addRow("Height:", heightSpin);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    auto cone = std::make_shared<Cone>(nameEdit->text().toStdString(),
        radiusSpin->value(), heightSpin->value());
    shapeManager.addShape(cone);
    QMessageBox::information(this, "Success", "Cone Created.");
}

void Sketcher3D::onSphereToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Sphere");
    auto layout = new QFormLayout(&dlg);

    auto nameEdit = new QLineEdit("Sphere1", &dlg);
    layout->addRow("Sphere Name:", nameEdit);

    auto radiusSpin = new QDoubleSpinBox(&dlg);
    radiusSpin->setRange(0.1, 10000);
    radiusSpin->setValue(10.0);
    layout->addRow("Radius:", radiusSpin);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    auto sp = std::make_shared<Sphere>(nameEdit->text().toStdString(), radiusSpin->value());
    shapeManager.addShape(sp);
    QMessageBox::information(this, "Success", "Sphere created.");
}

void Sketcher3D::onSaveGNUActionTriggered()
{
    std::string fileName = "../ShapesGNU.dat";
    const auto shapesVec = shapeManager.getShapesVec();
    if (FileHandle::saveToFileGNUPlot(fileName, shapesVec))
        QMessageBox::information(this, "Saved", "Shapes saved successfully!");
    else
        QMessageBox::information(this, "Not Saved", "Shapes not saved!");
}

void Sketcher3D::onSaveActionTriggered()
{
    std::string fileName = "../Shapes.skt";
    const auto shapesVec = shapeManager.getShapesVec();
    if (FileHandle::saveToFile(fileName, shapesVec))
        QMessageBox::information(this, "Saved", "Shapes saved successfully!");
    else
        QMessageBox::information(this, "Not Saved", "Shapes not saved!");
}

// ========= Transform Actions (use 4x4 under the hood) =========

void Sketcher3D::onTranslateTriggered()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Translation");

    auto layout = new QFormLayout(&dlg);
    auto dx = new QDoubleSpinBox(&dlg);
    auto dy = new QDoubleSpinBox(&dlg);
    dx->setRange(-10000, 10000); dy->setRange(-10000, 10000);
    dx->setDecimals(3); dy->setDecimals(3);
    layout->addRow("dx:", dx);
    layout->addRow("dy:", dy);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted)
        mGLWidget->applyTranslation(static_cast<float>(dx->value()),
            static_cast<float>(dy->value()));
}

void Sketcher3D::onRotateTriggered()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Rotation");

    auto layout = new QFormLayout(&dlg);
    auto deg = new QDoubleSpinBox(&dlg);
    deg->setRange(-36000, 36000);
    deg->setDecimals(3);
    layout->addRow("Angle (degrees, about Z):", deg);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted)
        mGLWidget->applyRotation(static_cast<float>(deg->value()));
}

void Sketcher3D::onScaleTriggered()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Scaling");

    auto layout = new QFormLayout(&dlg);
    auto sx = new QDoubleSpinBox(&dlg);
    auto sy = new QDoubleSpinBox(&dlg);
    sx->setRange(-1000, 1000); sy->setRange(-1000, 1000);
    sx->setDecimals(3); sy->setDecimals(3);
    sx->setValue(1.0);  sy->setValue(1.0);
    layout->addRow("sx:", sx);
    layout->addRow("sy:", sy);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted)
        mGLWidget->applyScale(static_cast<float>(sx->value()),
            static_cast<float>(sy->value()));
}
