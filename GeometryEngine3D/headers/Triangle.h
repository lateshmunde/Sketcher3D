#pragma once
#include "Exports.h"
#include <iostream>

class GEOMETRYENGINE3D_API Triangle
{
public:
	int m1; // index of points
	int m2;
	int m3; 


public:
	Triangle(int m1, int m2, int m3);
	~Triangle();
};
