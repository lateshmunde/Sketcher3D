#include "AiShapeGenerator.h"
#include <algorithm>

bool AiShapeGenerator::contains(const std::string& text, const std::string& word)
{
    std::string t = text, w = word;
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    std::transform(w.begin(), w.end(), w.begin(), ::tolower);
    return t.find(w) != std::string::npos;
}

std::shared_ptr<Shape> AiShapeGenerator::generateShapeFromPrompt(const std::string& prompt)
{
    // === Simple NLP heuristics ===
    double size1 = 10, size2 = 20, size3 = 30;

    if (contains(prompt, "small"))  size1 = 5;
    if (contains(prompt, "medium")) size1 = 10;
    if (contains(prompt, "large"))  size1 = 20;

    // === Shape Detection ===
    if (contains(prompt, "cube"))
        return std::make_shared<Cube>("AICube", size1);

    if (contains(prompt, "sphere"))
        return std::make_shared<Sphere>("AISphere", size1);

    if (contains(prompt, "cylinder"))
        return std::make_shared<Cylinder>("AICylinder", size1, size2);

    if (contains(prompt, "cone"))
        return std::make_shared<Cone>("AICone", size1, size2);

    if (contains(prompt, "pyramid"))
        return std::make_shared<Pyramid>("AIPyramid", size2, size2, size3);

    if (contains(prompt, "cuboid") || contains(prompt, "box"))
        return std::make_shared<Cuboid>("AICuboid", size1, size2, size3);

    // Default
    return nullptr;
}
