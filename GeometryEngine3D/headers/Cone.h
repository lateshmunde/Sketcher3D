#pragma once
#include "Exports.h"
#include "Shape.h"

class GEOMETRYENGINE3D_API Cone : public Shape
{
private:
	double mRadius;
	double mHeight;

protected:
	void build() override;

public:
	Cone(const std::string& name, double radius, double height);
	~Cone() override = default;

	 void save(std::ostream& out) const override;
	 void saveForGnu(std::ostream& out) const override;

	 double getRadius() const;
	 double getHeight() const;
	 double getSlantHeight() const;
};

