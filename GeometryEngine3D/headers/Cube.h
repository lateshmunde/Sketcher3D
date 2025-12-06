#pragma once
#include "Exports.h"
#include "Shape.h"

class GEOMETRYENGINE3D_API Cube : public Shape
{
private:
	double mSide;
public:
	Cube(const std::string& name, double side);
	~Cube() override = default;

	const std::vector<Point> getCoordinates() const override;
	const std::vector<Point> coodinatesForGLTriangle() const override;
	void save(std::ostream& fout) const override;
	void saveForGnu(std::ostream& fout) const override;

	double getSide() const;
	Triangulation makeCube() const override;
};