#pragma once
#include "Exports.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Point.h"
#include "Triangulation.h"
#include "Constants.h"


class GEOMETRYENGINE3D_API Shape {

private:
    std::string mType;
    std::string mName;

public:
    std::vector <Point> mPoints;
    std::vector <Triangle> mTriangles;

protected:
    Triangulation mTriag;

    virtual void build() = 0;

public:
    Shape(const std::string& type,const std::string& name);
    virtual ~Shape() = default;

    // Common method
    const std::string& getName() const;
    const std::string&  getType() const;
    const Triangulation& getTriangulation();

    virtual void save(std::ostream& os) const = 0;
    virtual void saveForGnu(std::ostream& os) const = 0;
};

 