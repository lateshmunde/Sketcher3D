#pragma once
#include "Exports.h"
#include "Shape.h"

class GEOMETRYENGINE3D_API Sphere : public Shape
{
private:
	double mRadius;

public:
	Sphere(const std::string& name, double radius);
	~Sphere() override = default;

	const std::vector<Point> getCoordinates() const override;
	void save(std::ostream& out) const override;
	void saveForGnu(std::ostream& out) const override;

	double getRadius() const;
};