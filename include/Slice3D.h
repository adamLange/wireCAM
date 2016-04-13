#ifndef SLICE3D_H
#define SLICE3D_H

#include "Slice.h"
#include "TopoDS_Face.hxx"
#include "TopoDS_Edge.hxx"
#include "gp_Pnt.hxx"
#include <list>
//using namespace std;

class Splitter;
class Marker;

class Slice3D : public Slice
{
public:

  Slice3D(const TopoDS_Edge& edge, const TopoDS_Face& face,
      const bool performNow=true, const double baseTol=0.05
    );

  virtual gp_Dir surfaceNormal(const double& u);

  virtual TopoDS_Shape shape();

  TopoDS_Edge edge;
  TopoDS_Face face;

private:

  Slice* emptyCopy();

  void performBaseTessilation(std::list<double>& params,
      std::list<gp_Pnt>& points);

  virtual void calc(const std::list<double>& params,
      std::list<gp_Pnt>& points,
      std::list<double>& alphas
    );
  virtual void updateGeometry();

  double baseTol;
};

#endif//SLICE3D_H
