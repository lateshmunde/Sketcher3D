#pragma once
#include <QtWidgets/QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include <QGridLayout>
#include <QToolBar>
#include <QToolButton>
//#include <QMenu>
//#include <QAction>
#include "Shape.h"
#include "ShapeCreator.h"
#include "ShapeManager.h"
#include "FileHandle.h"

#include <QtWidgets/QMainWindow>
//#include "ui_Sketcher3D.h"

class Sketcher3D : public QMainWindow
{
    Q_OBJECT

public:
    Sketcher3D(QWidget *parent = nullptr);
    ~Sketcher3D();
    void toolBarElements();
    void menuBarElements();

private:
    //Ui::Sketcher3DClass ui;
    void setupUI() ;
    

private:
    QWidget* mCentralWidget;
    QGridLayout* mCentralgridWidget;

    QToolBar* mToolBar;
    QMenuBar* mMenuBar;
    QStatusBar* mStatusBar;

    QToolButton* mCuboidTool;
    QToolButton* mCubeTool;
    QToolButton* mConeTool;
    QToolButton* mCylinderTool;
    QToolButton* mPyramidTool;
    QToolButton* mSphereTool;

    QMenu* mFileMenu;
    QAction* mNewAction;
    QAction* mOpenAction;
    QAction* mSaveAction;

    ShapeManager mgr;
    
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
    
};

