#ifndef WIRECAMSLICE_H
#define WIRECAMSLICE_H

#include "TopoDS_Face.hxx"
#include "TopoDS_Edge.hxx"
#include "gp_Pnt.hxx"
#include <list>
//using namespace std;

class Splitter;
class Marker;

class Slice
{
public:

  Slice(const TopoDS_Edge& edge, const TopoDS_Face& face,
      const bool performNow=true, const double baseTol=0.05
    );

  std::list<Slice> split(Splitter& splitter);
  void refine(Marker& marker);

  TopoDS_Edge edge;
  TopoDS_Face face;
  std::list<double> params;
  std::list<gp_Pnt> points;
  std::list<gp_Vec> normals;
  std::list<double> alphas;

private:

  void performBaseTessilation(std::list<double>& params,
      std::list<gp_Pnt>& points);

  virtual void calc(const std::list<double>& params,
      std::list<gp_Pnt>& points,
      std::list<gp_Vec>& normals,
      std::list<double>& alphas
    );
  //void calc(const std::list<double>& params, const std::list<gp_Pnt>& points);

  double baseTol;
};

#endif//WIRECAMSLICE_H
