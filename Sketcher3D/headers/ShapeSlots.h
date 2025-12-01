#pragma once
#include <QtWidgets/QMainWindow>
#include "Shape.h"
#include "Cube.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Pyramid.h"



class ShapeSlots
{
public:
    
    static std::unique_ptr<QDoubleSpinBox> dimensionField(QDialog& dlg,
        const QString& name, std::unique_ptr<QFormLayout>& layout);

    static void dialogButtonBoxFn(QDialog& dlg,
        std::unique_ptr<QFormLayout>& layout);

    static Cube  cubeSlot(QWidget* parent);
    static Cuboid  cuboidSlot(QWidget* parent);
    static Pyramid pyramidSlot(QWidget* parent);
    static Sphere sphereSlot(QWidget* parent);
    static Cone coneSlot(QWidget* parent);
    static Cylinder cylinderSlot(QWidget* parent);
   

};

