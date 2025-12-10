#pragma once
#include <memory>
#include <string>

#include "Shape.h"
#include "Cube.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Pyramid.h"

class AiShapeGenerator
{
public:
    static std::shared_ptr<Shape> generateShapeFromPrompt(const std::string& prompt);

private:
    static bool contains(const std::string& text, const std::string& word);
};
