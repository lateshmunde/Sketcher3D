#pragma once
#include <QtWidgets/QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include <QGridLayout>
#include <QToolBar>
#include <QToolButton>
#include "Shape.h"

#include <QtWidgets/QMainWindow>
//#include "ui_Sketcher3D.h"

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
    
private slots:
    void onCuboidToolClicked();
    void onCubeToolClicked();
    void onConeToolClicked();
    void onCylinderToolClicked();
    void onPyramidClicked();
    void onSphereToolClicked();

};

