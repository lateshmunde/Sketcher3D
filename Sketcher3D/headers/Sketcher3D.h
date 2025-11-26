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

private:
    std::unique_ptr<QWidget> mCentralWidget;
    std::unique_ptr<QGridLayout> mCentralgridWidget;

    std::unique_ptr<QToolBar> mToolBar;
    std::unique_ptr<QMenuBar> mMenuBar;
    std::unique_ptr<QStatusBar> mStatusBar;

    std::unique_ptr<QToolButton> mCuboidTool;
    std::unique_ptr<QToolButton> mCubeTool;
    std::unique_ptr<QToolButton> mConeTool;
    std::unique_ptr<QToolButton> mPyramidTool;
    std::unique_ptr<QToolButton> mCylinderTool;
    std::unique_ptr<QToolButton> mSphereTool;
    
};

