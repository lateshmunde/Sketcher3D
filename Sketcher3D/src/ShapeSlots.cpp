#include "stdafx.h"
#include "ShapeSlots.h"

std::unique_ptr<QDoubleSpinBox> ShapeSlots::dimensionField(QDialog& dlg,
    const QString& name, std::unique_ptr<QFormLayout>& layout)
{
    // Dimension
    std::unique_ptr<QDoubleSpinBox> dimensionSpin = std::make_unique<QDoubleSpinBox>(&dlg);
    dimensionSpin->setRange(0.1, 10000);
    dimensionSpin->setValue(10.0);
    layout->addRow(name + " :", dimensionSpin.get());
    return dimensionSpin;
}

void ShapeSlots::dialogButtonBoxFn(QDialog& dlg, std::unique_ptr<QFormLayout>& layout)
{
    // OK / Cancel buttons
    std::unique_ptr<QDialogButtonBox> buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(buttons.get());

    QObject::connect(buttons.get(), &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons.get(), &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    // Show dialog
    if (dlg.exec() != QDialog::Accepted)
        throw std::runtime_error("Dialog cancelled");
}


Pyramid ShapeSlots::pyramidSlot(QWidget* parent)
{
    // Create dialog
    QDialog dlg(parent);
    dlg.setWindowTitle("Create Pyramid");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Name
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Pyramid1", &dlg);
    layout.get()->addRow("Pyramid Name:", nameEdit.get());

    std::unique_ptr<QDoubleSpinBox> lengthSpin = dimensionField(dlg, "Length", layout);
    std::unique_ptr<QDoubleSpinBox> widthSpin = dimensionField(dlg, "Width", layout);
    std::unique_ptr<QDoubleSpinBox> heightSpin = dimensionField(dlg, "Height", layout);

    dialogButtonBoxFn(dlg, layout); // OK / Cancel buttons

    // Get values
    QString name = nameEdit->text();
    double baseLength = lengthSpin->value();
    double baseWidth = widthSpin->value();
    double height = heightSpin->value();

    return Pyramid(name.toStdString(), baseLength, baseWidth, height);
}



Cube ShapeSlots::cubeSlot(QWidget* parent)
{
    // Create dialog
    QDialog dlg(parent);
    dlg.setWindowTitle("Create Cube");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Name 
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Cube1", &dlg);
    layout.get()->addRow("Cube Name:", nameEdit.get());

    // side
    std::unique_ptr<QDoubleSpinBox> lengthSpin = dimensionField(dlg, "Side", layout);

    dialogButtonBoxFn(dlg, layout); // OK / Cancel buttons

    // Get values
    QString name = nameEdit->text();
    double side = lengthSpin->value();

    return Cube(name.toStdString(), side);
}

Cuboid ShapeSlots::cuboidSlot(QWidget* parent)
{
    // Create dialog
    QDialog dlg(parent);
    dlg.setWindowTitle("Create Cuboid");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Name field
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Cuboid1", &dlg);
    layout->addRow("Cuboid Name:", nameEdit.get());

    std::unique_ptr<QDoubleSpinBox> lengthSpin = dimensionField(dlg, "Length", layout);
    std::unique_ptr<QDoubleSpinBox> widthSpin = dimensionField(dlg, "Width", layout);
    std::unique_ptr<QDoubleSpinBox> heightSpin = dimensionField(dlg, "Height", layout);

    dialogButtonBoxFn(dlg, layout); // OK / Cancel buttons
    // Extract name
    QString name = nameEdit->text();
    // Extract value
    double length = lengthSpin->value();
    double width = widthSpin.get()->value();
    double height = heightSpin.get()->value();

    return Cuboid(name.toStdString(), length, width, height);
}

Sphere ShapeSlots::sphereSlot(QWidget* parent)
{
    QDialog dlg(parent);
    dlg.setWindowTitle("Create Sphere");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Sphere Name
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Sphere1", &dlg);
    layout->addRow("Sphere Name:", nameEdit.get());

    std::unique_ptr<QDoubleSpinBox> radiusSpin = dimensionField(dlg, "Radius", layout);

    dialogButtonBoxFn(dlg, layout); // OK / Cancel buttons

    // Get values
    const QString name = nameEdit->text();
    const double radius = radiusSpin->value();

    return Sphere(name.toStdString(), radius);

 }

 Cone ShapeSlots::coneSlot(QWidget* parent)
{
    // Create dialog
    QDialog dlg(parent);
    dlg.setWindowTitle("Create Cone");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Name
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Cone1", &dlg);
    layout->addRow("Cone Name:", nameEdit.get());

    std::unique_ptr<QDoubleSpinBox> radiusSpin = dimensionField(dlg, "Radius", layout);
    std::unique_ptr<QDoubleSpinBox> heightSpin = dimensionField(dlg, "Height", layout);

    dialogButtonBoxFn(dlg, layout); // OK / Cancel buttons

    // Get values
    const QString name = nameEdit->text();
    const double radius = radiusSpin->value();
    const double height = heightSpin->value();

    return Cone(name.toStdString(), radius, height);
 }

 Cylinder ShapeSlots::cylinderSlot(QWidget* parent)
{
    // Create dialog
    QDialog dlg(parent);
    dlg.setWindowTitle("Create Cylinder");

    // Layout
    std::unique_ptr<QFormLayout> layout = std::make_unique<QFormLayout>(&dlg);

    // Cylinder Name
    std::unique_ptr<QLineEdit> nameEdit = std::make_unique<QLineEdit>("Cylinder1", &dlg);
    layout->addRow("Cylinder Name:", nameEdit.get());

    std::unique_ptr<QDoubleSpinBox> radiusSpin = dimensionField(dlg, "Radius", layout);
    std::unique_ptr<QDoubleSpinBox> heightSpin = dimensionField(dlg, "Height", layout);

    dialogButtonBoxFn(dlg, layout); // OK / Cancel buttons

    // Get values
    const QString name = nameEdit->text();
    const double radius = radiusSpin->value();
    const double height = heightSpin->value();

    return Cylinder(name.toStdString(), radius, height);
 }


