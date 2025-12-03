#include "stdafx.h"
#include "Sketcher3D.h"

#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QStyle>
#include <QMessageBox>

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

Sketcher3D::~Sketcher3D() = default;

void Sketcher3D::setupUI()
{
    mCentralWidget = std::make_unique<QWidget>(this);
    mCentralLayout = std::make_unique<QGridLayout>(mCentralWidget.get());
    setCentralWidget(mCentralWidget.get());

    setupMenuBar();
    setupToolBar();

    mStatusBar = std::make_unique<QStatusBar>(this);
    setStatusBar(mStatusBar.get());
    mStatusBar->showMessage("Ready");

    // OpenGL widget
    mGLWidget = std::make_unique<OpenGLWidget>(this);
    mCentralLayout->addWidget(mGLWidget.get(), 0, 0);

    // Connect shape buttons
    connect(mCubeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCubeToolClicked);
    connect(mCuboidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCuboidToolClicked);
    connect(mCylinderTool.get(), &QToolButton::clicked, this, &Sketcher3D::onCylinderToolClicked);
    connect(mConeTool.get(), &QToolButton::clicked, this, &Sketcher3D::onConeToolClicked);
    connect(mSphereTool.get(), &QToolButton::clicked, this, &Sketcher3D::onSphereToolClicked);
    connect(mPyramidTool.get(), &QToolButton::clicked, this, &Sketcher3D::onPyramidToolClicked);

    // File actions
    connect(mSaveAction, &QAction::triggered, this, &Sketcher3D::onSaveActionTriggered);
    connect(mSaveGNUAction, &QAction::triggered, this, &Sketcher3D::onSaveGNUActionTriggered);

    // Transformation actions
    connect(mTranslateAction, &QAction::triggered, this, &Sketcher3D::onTranslateTriggered);
    connect(mScaleAction, &QAction::triggered, this, &Sketcher3D::onScaleTriggered);
    connect(mRotateXAction, &QAction::triggered, this, &Sketcher3D::onRotateXTriggered);
    connect(mRotateYAction, &QAction::triggered, this, &Sketcher3D::onRotateYTriggered);
    connect(mRotateZAction, &QAction::triggered, this, &Sketcher3D::onRotateZTriggered);
}

void Sketcher3D::setupMenuBar()
{
    mMenuBar = std::make_unique<QMenuBar>(this);
    setMenuBar(mMenuBar.get());

    // File menu
    mFileMenu = mMenuBar->addMenu("File");
    mSaveMenu = mFileMenu->addMenu("Save");

    mSaveAction = mSaveMenu->addAction(
        mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save");
    mSaveAction->setShortcut(QKeySequence::Save);

    mSaveGNUAction = mSaveMenu->addAction(
        mMenuBar->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save GNU");

    mNewAction = mFileMenu->addAction(
        mMenuBar->style()->standardIcon(QStyle::SP_FileIcon), "New");
    mNewAction->setShortcut(QKeySequence::New);

    mOpenAction = mFileMenu->addAction(
        mMenuBar->style()->standardIcon(QStyle::SP_DirOpenIcon), "Open");
    mOpenAction->setShortcut(QKeySequence::Open);

    // Transform menu
    mTransformMenu = mMenuBar->addMenu("Transformations");
    mTranslateAction = mTransformMenu->addAction("Translate");
    mScaleAction = mTransformMenu->addAction("Scale");

    mRotateMenu = mTransformMenu->addMenu("Rotate");
    mRotateXAction = mRotateMenu->addAction("Around X-axis");
    mRotateYAction = mRotateMenu->addAction("Around Y-axis");
    mRotateZAction = mRotateMenu->addAction("Around Z-axis");
}

void Sketcher3D::setupToolBar()
{
    mToolBar = std::make_unique<QToolBar>(this);
    addToolBar(mToolBar.get());
    mToolBar->setIconSize(QSize(32, 32));

    mCubeTool = std::make_unique<QToolButton>(mToolBar.get());
    mCubeTool->setIcon(QIcon(":/Sketcher3D/Resources/cube.png"));
    mCubeTool->setToolTip("Cube");
    mToolBar->addWidget(mCubeTool.get());

    mCuboidTool = std::make_unique<QToolButton>(mToolBar.get());
    mCuboidTool->setIcon(QIcon(":/Sketcher3D/Resources/cuboid.png"));
    mCuboidTool->setToolTip("Cuboid");
    mToolBar->addWidget(mCuboidTool.get());

    mSphereTool = std::make_unique<QToolButton>(mToolBar.get());
    mSphereTool->setIcon(QIcon(":/Sketcher3D/Resources/sphere.png"));
    mSphereTool->setToolTip("Sphere");
    mToolBar->addWidget(mSphereTool.get());

    mCylinderTool = std::make_unique<QToolButton>(mToolBar.get());
    mCylinderTool->setIcon(QIcon(":/Sketcher3D/Resources/cylinder.png"));
    mCylinderTool->setToolTip("Cylinder");
    mToolBar->addWidget(mCylinderTool.get());

    mConeTool = std::make_unique<QToolButton>(mToolBar.get());
    mConeTool->setIcon(QIcon(":/Sketcher3D/Resources/cone.png"));
    mConeTool->setToolTip("Cone");
    mToolBar->addWidget(mConeTool.get());

    mPyramidTool = std::make_unique<QToolButton>(mToolBar.get());
    mPyramidTool->setIcon(QIcon(":/Sketcher3D/Resources/pyramid.png"));
    mPyramidTool->setToolTip("Pyramid");
    mToolBar->addWidget(mPyramidTool.get());
}

void Sketcher3D::addShapeWithTransform(const std::shared_ptr<Shape>& shape)
{
    mShapeManager.addShape(shape);
    mTransformManager.addIdentityTransform();
    updateGLScene();
}

void Sketcher3D::updateGLScene()
{
    const auto shapes = mShapeManager.getShapesVec();
    const auto& transforms = mTransformManager.getTransforms();

    if (mGLWidget)
        mGLWidget->setScene(shapes, transforms);
}

// ===================== Shape Creation Slots =====================

void Sketcher3D::onCubeToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cube");
    QFormLayout layout(&dlg);

    QLineEdit nameEdit("Cube1", &dlg);
    layout.addRow("Name:", &nameEdit);

    QDoubleSpinBox sideSpin(&dlg);
    sideSpin.setRange(0.1, 10000.0);
    sideSpin.setValue(10.0);
    layout.addRow("Side:", &sideSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    auto cube = std::make_shared<Cube>(nameEdit.text().toStdString(), sideSpin.value());
    addShapeWithTransform(cube);
}

void Sketcher3D::onCuboidToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cuboid");
    QFormLayout layout(&dlg);

    QLineEdit nameEdit("Cuboid1", &dlg);
    layout.addRow("Name:", &nameEdit);

    QDoubleSpinBox lSpin(&dlg), wSpin(&dlg), hSpin(&dlg);
    lSpin.setRange(0.1, 10000.0); wSpin.setRange(0.1, 10000.0); hSpin.setRange(0.1, 10000.0);
    lSpin.setValue(10.0); wSpin.setValue(10.0); hSpin.setValue(10.0);
    layout.addRow("Length:", &lSpin);
    layout.addRow("Width:", &wSpin);
    layout.addRow("Height:", &hSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    auto cuboid = std::make_shared<Cuboid>(nameEdit.text().toStdString(),
        lSpin.value(), wSpin.value(), hSpin.value());
    addShapeWithTransform(cuboid);
}

void Sketcher3D::onCylinderToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cylinder");
    QFormLayout layout(&dlg);

    QLineEdit nameEdit("Cylinder1", &dlg);
    layout.addRow("Name:", &nameEdit);

    QDoubleSpinBox rSpin(&dlg), hSpin(&dlg);
    rSpin.setRange(0.1, 10000.0); rSpin.setValue(10.0);
    hSpin.setRange(0.1, 10000.0); hSpin.setValue(10.0);
    layout.addRow("Radius:", &rSpin);
    layout.addRow("Height:", &hSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    auto cyl = std::make_shared<Cylinder>(nameEdit.text().toStdString(),
        rSpin.value(), hSpin.value());
    addShapeWithTransform(cyl);
}

void Sketcher3D::onConeToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Cone");
    QFormLayout layout(&dlg);

    QLineEdit nameEdit("Cone1", &dlg);
    layout.addRow("Name:", &nameEdit);

    QDoubleSpinBox rSpin(&dlg), hSpin(&dlg);
    rSpin.setRange(0.1, 10000.0); rSpin.setValue(10.0);
    hSpin.setRange(0.1, 10000.0); hSpin.setValue(10.0);
    layout.addRow("Radius:", &rSpin);
    layout.addRow("Height:", &hSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    auto cone = std::make_shared<Cone>(nameEdit.text().toStdString(),
        rSpin.value(), hSpin.value());
    addShapeWithTransform(cone);
}

void Sketcher3D::onSphereToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Sphere");
    QFormLayout layout(&dlg);

    QLineEdit nameEdit("Sphere1", &dlg);
    layout.addRow("Name:", &nameEdit);

    QDoubleSpinBox rSpin(&dlg);
    rSpin.setRange(0.1, 10000.0);
    rSpin.setValue(10.0);
    layout.addRow("Radius:", &rSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    auto sphere = std::make_shared<Sphere>(nameEdit.text().toStdString(), rSpin.value());
    addShapeWithTransform(sphere);
}

void Sketcher3D::onPyramidToolClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Create Pyramid");
    QFormLayout layout(&dlg);

    QLineEdit nameEdit("Pyramid1", &dlg);
    layout.addRow("Name:", &nameEdit);

    QDoubleSpinBox lSpin(&dlg), wSpin(&dlg), hSpin(&dlg);
    lSpin.setRange(0.1, 10000.0); lSpin.setValue(10.0);
    wSpin.setRange(0.1, 10000.0); wSpin.setValue(10.0);
    hSpin.setRange(0.1, 10000.0); hSpin.setValue(10.0);
    layout.addRow("Base Length:", &lSpin);
    layout.addRow("Base Width:", &wSpin);
    layout.addRow("Height:", &hSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    auto pyr = std::make_shared<Pyramid>(nameEdit.text().toStdString(),
        lSpin.value(), wSpin.value(), hSpin.value());
    addShapeWithTransform(pyr);
}

// ===================== File Save =====================

void Sketcher3D::onSaveActionTriggered()
{
    const std::string fileName = "../Shapes.skt";
    auto shapes = mShapeManager.getShapesVec();

    if (FileHandle::saveToFile(fileName, shapes))
        QMessageBox::information(this, "Saved", "Shapes saved successfully!");
    else
        QMessageBox::warning(this, "Error", "Failed to save shapes.");
}

void Sketcher3D::onSaveGNUActionTriggered()
{
    const std::string fileName = "../ShapesGNU.dat";
    auto shapes = mShapeManager.getShapesVec();

    // NOTE: still saving original coordinates.
    // To save transformed coordinates, extend FileHandle to take transforms too.
    if (FileHandle::saveToFileGNUPlot(fileName, shapes))
        QMessageBox::information(this, "Saved", "GNU data saved successfully!");
    else
        QMessageBox::warning(this, "Error", "Failed to save GNU data.");
}

// ===================== Transformation Slots =====================

void Sketcher3D::onTranslateTriggered()
{
    auto shapes = mShapeManager.getShapesVec();
    if (shapes.empty()) {
        QMessageBox::warning(this, "No shapes", "There are no shapes to transform.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Translate Shape");
    QFormLayout layout(&dlg);

    QSpinBox indexSpin(&dlg);
    indexSpin.setRange(0, static_cast<int>(shapes.size()) - 1);
    layout.addRow("Shape index:", &indexSpin);

    QDoubleSpinBox txSpin(&dlg), tySpin(&dlg), tzSpin(&dlg);
    txSpin.setRange(-1000, 1000); tySpin.setRange(-1000, 1000); tzSpin.setRange(-1000, 1000);
    layout.addRow("Translate X:", &txSpin);
    layout.addRow("Translate Y:", &tySpin);
    layout.addRow("Translate Z:", &tzSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    const int idx = indexSpin.value();
    QMatrix4x4& M = mTransformManager.transformAt(idx);
    M.translate(static_cast<float>(txSpin.value()),
        static_cast<float>(tySpin.value()),
        static_cast<float>(tzSpin.value()));

    updateGLScene();
}

void Sketcher3D::onScaleTriggered()
{
    auto shapes = mShapeManager.getShapesVec();
    if (shapes.empty()) {
        QMessageBox::warning(this, "No shapes", "There are no shapes to transform.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Scale Shape");
    QFormLayout layout(&dlg);

    QSpinBox indexSpin(&dlg);
    indexSpin.setRange(0, static_cast<int>(shapes.size()) - 1);
    layout.addRow("Shape index:", &indexSpin);

    QDoubleSpinBox sxSpin(&dlg), sySpin(&dlg), szSpin(&dlg);
    sxSpin.setRange(0.001, 1000); sySpin.setRange(0.001, 1000); szSpin.setRange(0.001, 1000);
    sxSpin.setValue(1.0); sySpin.setValue(1.0); szSpin.setValue(1.0);
    layout.addRow("Scale X:", &sxSpin);
    layout.addRow("Scale Y:", &sySpin);
    layout.addRow("Scale Z:", &szSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    const int idx = indexSpin.value();
    QMatrix4x4& M = mTransformManager.transformAt(idx);
    M.scale(static_cast<float>(sxSpin.value()),
        static_cast<float>(sySpin.value()),
        static_cast<float>(szSpin.value()));

    updateGLScene();
}

void Sketcher3D::onRotateXTriggered()
{
    auto shapes = mShapeManager.getShapesVec();
    if (shapes.empty()) {
        QMessageBox::warning(this, "No shapes", "There are no shapes to transform.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Rotate Around X-axis");
    QFormLayout layout(&dlg);

    QSpinBox indexSpin(&dlg);
    indexSpin.setRange(0, static_cast<int>(shapes.size()) - 1);
    layout.addRow("Shape index:", &indexSpin);

    QDoubleSpinBox angleSpin(&dlg);
    angleSpin.setRange(-360, 360);
    layout.addRow("Angle (degrees):", &angleSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    const int idx = indexSpin.value();
    QMatrix4x4& M = mTransformManager.transformAt(idx);
    M.rotate(static_cast<float>(angleSpin.value()), 1.0f, 0.0f, 0.0f);

    updateGLScene();
}

void Sketcher3D::onRotateYTriggered()
{
    auto shapes = mShapeManager.getShapesVec();
    if (shapes.empty()) {
        QMessageBox::warning(this, "No shapes", "There are no shapes to transform.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Rotate Around Y-axis");
    QFormLayout layout(&dlg);

    QSpinBox indexSpin(&dlg);
    indexSpin.setRange(0, static_cast<int>(shapes.size()) - 1);
    layout.addRow("Shape index:", &indexSpin);

    QDoubleSpinBox angleSpin(&dlg);
    angleSpin.setRange(-360, 360);
    layout.addRow("Angle (degrees):", &angleSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    const int idx = indexSpin.value();
    QMatrix4x4& M = mTransformManager.transformAt(idx);
    M.rotate(static_cast<float>(angleSpin.value()), 0.0f, 1.0f, 0.0f);

    updateGLScene();
}

void Sketcher3D::onRotateZTriggered()
{
    auto shapes = mShapeManager.getShapesVec();
    if (shapes.empty()) {
        QMessageBox::warning(this, "No shapes", "There are no shapes to transform.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Rotate Around Z-axis");
    QFormLayout layout(&dlg);

    QSpinBox indexSpin(&dlg);
    indexSpin.setRange(0, static_cast<int>(shapes.size()) - 1);
    layout.addRow("Shape index:", &indexSpin);

    QDoubleSpinBox angleSpin(&dlg);
    angleSpin.setRange(-360, 360);
    layout.addRow("Angle (degrees):", &angleSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    const int idx = indexSpin.value();
    QMatrix4x4& M = mTransformManager.transformAt(idx);
    M.rotate(static_cast<float>(angleSpin.value()), 0.0f, 0.0f, 1.0f);

    updateGLScene();
}
