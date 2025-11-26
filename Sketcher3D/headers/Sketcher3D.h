#pragma once
#include <QGridLayout>
#include <QToolBar>
#include <memory>
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
    //QWidget* mCentralWidget;
    std::unique_ptr<QWidget> mCentralWidget;
    //QGridLayout* mCentralgridWidget;
    std::unique_ptr<QGridLayout> mCentralgridWidget;

    /*QToolBar* mToolBar;
    QMenuBar* mMenuBar;
    QStatusBar* mStatusBar;*/
  

};

