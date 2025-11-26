#include "stdafx.h"
#include "Sketcher3D.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Sketcher3D window;
    window.show();
    return app.exec();
}
