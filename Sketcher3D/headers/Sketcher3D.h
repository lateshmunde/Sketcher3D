#pragma once
#include <QtWidgets/QMainWindow>
#include <QGridLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <memory>
#include <QToolButton>

#include "Shape.h"
#include "ShapeCreator.h"
#include "ShapeManager.h"
#include "FileHandle.h"
#include "OpenGLWidget.h"

class Sketcher3D : public QMainWindow
{
    Q_OBJECT
public:
    explicit Sketcher3D(QWidget* parent = nullptr);
    ~Sketcher3D();

    std::unique_ptr<QToolButton> createToolButton(
        QToolBar* toolbar, const QString& iconPath,
        const QString& toolTip, const QSize& iconSize = QSize(32, 32));

private:
    void setupUI();
    void toolBarElements();
    void menuBarElements();

private:
    OpenGLWidget* glWidget = nullptr;

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
    QAction* mLoadSTLAction = nullptr;
    QAction* mSaveSTLAction = nullptr;

    QAction* mNewAction = nullptr;
    QAction* mOpenAction = nullptr;
    QAction* mClearAction = nullptr;

    QMenu* mTransformationMenu = nullptr;
    QAction* mTranslate = nullptr;
    QAction* mScale = nullptr;
    QMenu* mRotate = nullptr;
    QAction* mRotateX = nullptr;
    QAction* mRotateY = nullptr;
    QAction* mRotateZ = nullptr;

    QAction* mPropertiesAction = nullptr;

    ShapeManager   shapeManager;
    Triangulation  tri;

private slots:
    void onCuboidToolClicked();
    void onCubeToolClicked();
    void onConeToolClicked();
    void onCylinderToolClicked();
    void onPyramidClicked();
    void onSphereToolClicked();

    void onSaveActionTriggered();
    void onSaveGNUActionTriggered();
    void onLoadSTLTriggered();
    void onSaveSTLTriggered();
    void onClearTriggered();

    void onTranslateActionTriggered();
    void onScaleActionTriggered();
    void onRotateXActionTriggered();
    void onRotateYActionTriggered();
    void onRotateZActionTriggered();

    void onPropertiesTriggered();
};
