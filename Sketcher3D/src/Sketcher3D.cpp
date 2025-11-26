#include "stdafx.h"
#include "Sketcher3D.h"

Sketcher3D::Sketcher3D(QWidget *parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);

    setupUI();
    resize(800, 600);

}

Sketcher3D::~Sketcher3D()
{}

void Sketcher3D::setupUI()
{
    // Central widget and layout
    mCentralWidget = new QWidget(this);
    mCentralgridWidget = new QGridLayout(mCentralWidget);

    
}