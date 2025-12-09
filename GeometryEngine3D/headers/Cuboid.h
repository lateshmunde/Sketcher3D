#pragma once
#include "Exports.h"
#include "Shape.h"

class GEOMETRYENGINE3D_API Cuboid : public Shape
{
private:
	double mLength;
	double mWidth;
	double mHeight;

protected:
	void build() override;

public:
	Cuboid(const std::string& name, double length, double width, double height);
	~Cuboid() override = default;

	void save(std::ostream& out) const override;
	void saveForGnu(std::ostream& out) const override;

	double getLength() const;
	double getWidth() const;
	double getHeight() const;
};

