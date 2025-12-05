#pragma once
#include <QtWidgets/QMainWindow> // For mCentralWidget
#include <QGridLayout> // For mCentralgridWidget
#include <QMenuBar> // For mMenuBar
#include <QToolBar> // For mToolBar
#include <QStatusBar> // For mStatusBar
#include <memory> // For unique_ptr
#include <QToolButton>
#include <fstream>
#include <QMessageBox>
#include <QFileDialog>
#include "Shape.h"
#include "ShapeCreator.h"
#include "ShapeManager.h"
#include "FileHandle.h"
#include "OpenGLWidget.h"

class Sketcher3D : public QMainWindow
{
    Q_OBJECT

public:
    Sketcher3D(QWidget *parent = nullptr);
    ~Sketcher3D();
    

private:
    //Ui::Sketcher3DClass ui;
    void setupUI() ;
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

    QMenu* mFileMenu;
    QMenu* mSaveMenu;
    QAction* mSaveAction;
    QAction* mSaveGNUAction;
    QAction* mNewAction;
    QAction* mOpenAction;

    QMenu* mTransformationMenu;
    QAction* mTranslate;
    QAction* mScale;
    QMenu* mRotate;
    QAction* mRotateX;
    QAction* mRotateY;
    QAction* mRotateZ;

    ShapeManager shapeManager; // handles container(vector) of shapes
    std::unique_ptr<OpenGLWidget> mGLWidget;
    
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
    void onSaveGNUActionTriggered(); // For GNU plot

    // Transformation
    void onTranslateActionTriggered();
    void onScaleActionTriggered();
    void onRotateXActionTriggered();
    void onRotateYActionTriggered();
    void onRotateZActionTriggered();
};

