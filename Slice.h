#ifndef SLICE_H
#define SLICE_H

#include "TopoDS_Face.hxx"
#include "TopoDS_Edge.hxx"
#include "gp_Pnt.hxx"
#include <list>
#include "Splitter.h"
using namespace std;

class Slice
{
public:

  Slice(const TopoDS_Edge& edge, const TopoDS_Face& face,
      const bool performNow=true, const double baseTol=0.05
    );

  list<Slice> split(Splitter& splitter);

  TopoDS_Edge edge;
  TopoDS_Face face;
  list<double> params;
  list<gp_Pnt> points;
  list<gp_Vec> normals;
  list<double> alphas;

private:

  void performBaseTessilation(list<double>& params,
      list<gp_Pnt>& points);

  void calc(const list<double>& params,
      list<gp_Pnt>& points,
      list<gp_Vec>& normals,
      list<double>& alphas
    );
  //void calc(const list<double>& params, const list<gp_Pnt>& points);

  double baseTol;
};

#endif//SLICE_H
