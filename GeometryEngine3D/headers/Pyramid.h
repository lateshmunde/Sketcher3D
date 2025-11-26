#pragma once
#include "Exports.h"
#include "Shape.h"

class GEOMETRYENGINE3D_API Pyramid : public Shape
{
private:
	double mBaseLength;
	double mBaseWidth;
	double mHeight;

public:
	Pyramid(const std::string& name, double baseLength, double baseWidth, double height);
	~Pyramid() override = default;

	const std::vector<Point> getCoordinates() const override;
	void save(std::ostream& out) const override;
	void saveForGnu(std::ostream& out) const override;

	double getLength() const;
	double getWidth() const;
	double getHeight() const;
	double getSlantHeight() const;
};