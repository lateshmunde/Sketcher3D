#pragma once

#include <QtWidgets/QMainWindow>
#include <QGridLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <memory>
#include <vector>

#include "Shape.h"
#include "ShapeManager.h"
#include "FileHandle.h"
#include "OpenGLWidget.h"
#include "TransformManager.h"

class Sketcher3D : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sketcher3D(QWidget* parent = nullptr);
    ~Sketcher3D();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();

    void addShapeWithTransform(const std::shared_ptr<Shape>& shape);
    void updateGLScene();

private:
    std::unique_ptr<QWidget>      mCentralWidget;
    std::unique_ptr<QGridLayout>  mCentralLayout;

    std::unique_ptr<QMenuBar>  mMenuBar;
    std::unique_ptr<QToolBar>  mToolBar;
    std::unique_ptr<QStatusBar> mStatusBar;

    std::unique_ptr<QToolButton> mCubeTool;
    std::unique_ptr<QToolButton> mCuboidTool;
    std::unique_ptr<QToolButton> mCylinderTool;
    std::unique_ptr<QToolButton> mConeTool;
    std::unique_ptr<QToolButton> mSphereTool;
    std::unique_ptr<QToolButton> mPyramidTool;

    // Menus / actions
    QMenu* mFileMenu = nullptr;
    QMenu* mSaveMenu = nullptr;
    QAction* mSaveAction = nullptr;
    QAction* mSaveGNUAction = nullptr;
    QAction* mNewAction = nullptr;
    QAction* mOpenAction = nullptr;

    QMenu* mTransformMenu = nullptr;
    QAction* mTranslateAction = nullptr;
    QAction* mScaleAction = nullptr;
    QMenu* mRotateMenu = nullptr;
    QAction* mRotateXAction = nullptr;
    QAction* mRotateYAction = nullptr;
    QAction* mRotateZAction = nullptr;

    ShapeManager     mShapeManager;
    TransformManager mTransformManager;

    std::unique_ptr<OpenGLWidget> mGLWidget;

private slots:
    // Shape creation
    void onCubeToolClicked();
    void onCuboidToolClicked();
    void onCylinderToolClicked();
    void onConeToolClicked();
    void onSphereToolClicked();
    void onPyramidToolClicked();

    // File
    void onSaveActionTriggered();
    void onSaveGNUActionTriggered();

    // Transformations
    void onTranslateTriggered();
    void onScaleTriggered();
    void onRotateXTriggered();
    void onRotateYTriggered();
    void onRotateZTriggered();
};
