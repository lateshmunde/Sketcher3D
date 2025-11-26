// GeometryEngine3D.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "GeometryEngine3D.h"


// This is an example of an exported variable
GEOMETRYENGINE3D_API int nGeometryEngine3D=0;

// This is an example of an exported function.
GEOMETRYENGINE3D_API int fnGeometryEngine3D(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CGeometryEngine3D::CGeometryEngine3D()
{
    return;
}
