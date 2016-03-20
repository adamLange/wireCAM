#include <string>
#include <list>
#include "TopoDS_Edge.hxx"
#include "TopoDS_Face.hxx"

class gp_Pnt;
class gp_Dir;
class gp_Vec;
//class TopoDS_Face;

#ifndef TARGETSURFACE_H
#define TARGETSURFACE_H

class TargetSurface
{
public:

  TargetSurface();
  TargetSurface(const Standard_CString &filepath);
  TargetSurface(TopoDS_Face &face);

  std::list<TopoDS_Edge> intersectWithPlane(
      const gp_Pnt& pnt,
      const gp_Dir& dir
    );

  std::list<TopoDS_Edge> intersectWithPlanes(
    const gp_Pnt& pnt,
    const gp_Dir& dir,
    const gp_Vec& vec,
    int n
  );

  std::list<TopoDS_Edge> slice(
    const gp_Dir& dir,
    const float& spacing,
    const float& boundingBoxPadding=5,
    const float& tol=0.1,
    const bool& performIncMesh=true
  );

private:
  TopoDS_Face face;
};

#endif //TARGETSURFACE_H
