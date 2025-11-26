#pragma once
#include "Exports.h"
#include "Shape.h"


class GEOMETRYENGINE3D_API Cuboid : public Shape
{
private:
	double mLength;
	double mWidth;
	double mHeight;

public:
	Cuboid(const std::string& name, double length, double width, double height);
	~Cuboid() override = default;

	const std::vector<Point> getCoordinates() const override;
	void save(std::ostream& out) const override;
	void saveForGnu(std::ostream& out) const override;

	double getLength() const;
	double getWidth() const;
	double getHeight() const;
};

//GEOMETRY3D_API std::ostream& operator<<(std::ostream& out, const Cuboid& c);

