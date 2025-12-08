#pragma once
#include <QtWidgets/QMainWindow>
#include <QGridLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <memory>
#include <QToolButton>
#include <fstream>
#include <QMessageBox>
#include <QFileDialog>
#include "Shape.h"
#include "ShapeCreator.h"
#include "ShapeManager.h"
#include "FileHandle.h"
#include "OpenGLWidget.h"  // OpenGL Widget

class Sketcher3D : public QMainWindow
{
    Q_OBJECT
public:
    Sketcher3D(QWidget* parent = nullptr);
    ~Sketcher3D();


    std::unique_ptr <QToolButton> createToolButton(
        QToolBar* toolbar,const QString& iconPath,
        const QString& toolTip,const QSize& iconSize);

  
private:
    void setupUI();
    void toolBarElements();
    void menuBarElements();
   
private:
    // OpenGL Widget for 3D rendering
    OpenGLWidget* glWidget;

    // UI Components
    std::unique_ptr<QWidget> mCentralWidget;
    std::unique_ptr<QGridLayout> mCentralgridWidget;
    std::unique_ptr<QMenuBar> mMenuBar;
    std::unique_ptr<QToolBar> mToolBar;
    std::unique_ptr<QStatusBar> mStatusBar;

    // Tool Buttons
    std::unique_ptr<QToolButton> mCubeTool;
    std::unique_ptr<QToolButton> mCuboidTool;
    std::unique_ptr<QToolButton> mCylinderTool;
    std::unique_ptr<QToolButton> mConeTool;
    std::unique_ptr<QToolButton> mSphereTool;
    std::unique_ptr<QToolButton> mPyramidTool;
  
    // Menu Items
    QMenu* mFileMenu;
    QMenu* mSaveMenu;
    QAction* mSaveAction;
    QAction* mSaveGNUAction;
    QAction* mLoadSTLAction;
    QAction* mSaveSTLAction;

    QAction* mNewAction;
    QAction* mOpenAction;

    QMenu* mTransformationMenu;
    QAction* mTranslate;
    QAction* mScale;
    QMenu* mRotate;
    QAction* mRotateX;
    QAction* mRotateY;
    QAction* mRotateZ;

    // Shape Manager
    ShapeManager shapeManager;

private slots:
    void onCuboidToolClicked();
    void onCubeToolClicked();
    void onConeToolClicked();
    void onCylinderToolClicked();
    void onPyramidClicked();
    void onSphereToolClicked();

    //void onNewActionTriggered();
    //void onOpenActionTriggered();
    void onSaveActionTriggered();
    void onSaveGNUActionTriggered();
    void onLoadSTLTriggered();
    void onSaveSTLTriggered();


    // Transformation
    /*void onTranslateActionTriggered();
    void onScaleActionTriggered();
    void onRotateXActionTriggered();
    void onRotateYActionTriggered();
    void onRotateZActionTriggered();*/
};
