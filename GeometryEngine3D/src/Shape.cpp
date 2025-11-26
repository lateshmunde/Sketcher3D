#include "pch.h"
#include "Shape.h"

Shape::Shape(const std::string& type, const std::string& name) : mType(type), mName(name){}

const std::string& Shape::getName() const { return mName; }
const std::string& Shape::getType() const { return mType; }

