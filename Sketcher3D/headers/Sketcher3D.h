#pragma once
#include <QGridLayout>
#include <QToolBar>
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
    QWidget* mCentralWidget;
    QGridLayout* mCentralgridWidget;
    QToolBar* mToolBar;
    QMenuBar* mMenuBar;
    QStatusBar* mStatusBar;
  

};

