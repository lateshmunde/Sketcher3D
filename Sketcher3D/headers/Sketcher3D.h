#pragma once
#include <QtWidgets/QMainWindow>
#include <QGridLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <memory>

#include "Shape.h"
#include "ShapeCreator.h"
#include "ShapeManager.h"
#include "FileHandle.h"
#include "OpenGLWidget.h"

class Sketcher3D : public QMainWindow
{
    Q_OBJECT

public:
    Sketcher3D(QWidget* parent = nullptr);
    ~Sketcher3D();

private:
    void setupUI();
    void toolBarElements();
    void menuBarElements();

private:
    std::unique_ptr<QWidget> mCentralWidget;
    std::unique_ptr<QGridLayout> mCentralgridWidget;

    std::unique_ptr<QMenuBar> mMenuBar;
    std::unique_ptr<QToolBar> mToolBar;
    std::unique_ptr<QStatusBar> mStatusBar;

    std::unique_ptr<QToolButton> mCubeTool;
    std::unique_ptr<QToolButton> mCuboidTool;
    std::unique_ptr<QToolButton> mCylinderTool;
    std::unique_ptr<QToolButton> mConeTool;
    std::unique_ptr<QToolButton> mSphereTool;
    std::unique_ptr<QToolButton> mPyramidTool;

    QMenu* mFileMenu = nullptr;
    QMenu* mSaveMenu = nullptr;
    QAction* mSaveAction = nullptr;
    QAction* mSaveGNUAction = nullptr;
    QAction* mNewAction = nullptr;
    QAction* mOpenAction = nullptr;

    QMenu* mTransformationMenu = nullptr; // optional mirroring
    QAction* mTranslate = nullptr;
    QAction* mScale = nullptr;
    QMenu* mRotate = nullptr;
    QAction* mRotateX = nullptr;
    QAction* mRotateY = nullptr;
    QAction* mRotateZ = nullptr;

    QMenu* mRotate_arb_z = nullptr;

    ShapeManager shapeManager;

    std::unique_ptr<OpenGLWidget> mGLWidget;

private slots:
    void onCuboidToolClicked();
    void onCubeToolClicked();
    void onConeToolClicked();
    void onCylinderToolClicked();
    void onPyramidClicked();
    void onSphereToolClicked();

    void onSaveActionTriggered();
    void onSaveGNUActionTriggered();

    // Transform handlers
    void onTranslateTriggered();
    void onRotateTriggered();
    void onScaleTriggered();

	void onRotateArbZTriggered();//new
};
