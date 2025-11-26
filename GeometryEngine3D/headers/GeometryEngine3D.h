#include "Exports.h"
// This class is exported from the dll
class GEOMETRYENGINE3D_API CGeometryEngine3D {
public:
	CGeometryEngine3D(void);
	// TODO: add your methods here.
};

extern GEOMETRYENGINE3D_API int nGeometryEngine3D;

GEOMETRYENGINE3D_API int fnGeometryEngine3D(void);
