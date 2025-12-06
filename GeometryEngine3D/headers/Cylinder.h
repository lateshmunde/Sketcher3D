#pragma once
#include "Exports.h"
#include "Shape.h"

class GEOMETRYENGINE3D_API Cylinder : public Shape
{
private:
	double mRadius;
	double mHeight;

public:
	Cylinder(const std::string& name, double radius, double height);
	~Cylinder() override = default;


	const std::vector<Point> getCoordinates() const override;
	const std::vector<Point> coodinatesForGLTriangle() const override;
	void save(std::ostream& out) const override;
	void saveForGnu(std::ostream& out) const override;

	double getRadius() const;
	double getHeight() const;

	Triangulation makeCube() const override;
};