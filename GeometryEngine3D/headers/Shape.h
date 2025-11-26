#pragma once
#include "Exports.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Point.h"
#include "Constants.h"


class GEOMETRYENGINE3D_API Shape {

private:
    std::string mType;
    std::string mName;

public:
    Shape(const std::string& type,const std::string& name);
    virtual ~Shape() = default;

    // Common method
    const std::string& getName() const;
    const std::string&  getType() const;

    virtual const std::vector<Point> getCoordinates() const = 0;
    virtual void save(std::ostream& os) const = 0;
    virtual void saveForGnu(std::ostream& os) const = 0;

};

 