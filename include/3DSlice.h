#ifndef 3DSLICE_H
#define 3DSLICE_H

#include "Slice.h"
#include "TopoDS_Face.hxx"
#include "TopoDS_Edge.hxx"
#include "gp_Pnt.hxx"
#include <list>
//using namespace std;

class Splitter;
class Marker;

class 3DSlice : public Slice
{
public:

  3DSlice(const TopoDS_Edge& edge, const TopoDS_Face& face,
      const bool performNow=true, const double baseTol=0.05
    );

  std::list<Slice> split(Splitter& splitter);
  void refine(Marker& marker);

  virtual gp_Vec surfaceNormal(const double& u);

  TopoDS_Edge edge;
  TopoDS_Face face;

private:

  virtual void emptyCopy(std::unique_ptr<Slice>& ptr);

  void performBaseTessilation(std::list<double>& params,
      std::list<gp_Pnt>& points);

  void calc(const std::list<double>& params,
      std::list<gp_Pnt>& points,
      std::list<gp_Vec>& normals,
      std::list<double>& alphas
    );
  //void calc(const std::list<double>& params, const std::list<gp_Pnt>& points);

  double baseTol;
};

#endif//3DSLICE_H
