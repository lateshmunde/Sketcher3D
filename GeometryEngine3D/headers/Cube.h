#pragma once
#include "Exports.h"
#include "Shape.h"

class GEOMETRYENGINE3D_API Cube : public Shape
{
private:
	double mSide;

protected:
	void build() override;

public:
	Cube(const std::string& name, double side);
	~Cube() override = default;

	void save(std::ostream& fout) const override;
	void saveForGnu(std::ostream& fout) const override;

	double getSide() const;
};