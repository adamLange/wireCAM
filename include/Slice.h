#ifndef WIRECAMSLICE_H
#define WIRECAMSLICE_H

#include "TopoDS_Face.hxx"
#include "TopoDS_Edge.hxx"
#include "gp_Pnt.hxx"
#include <list>
#include <memory>
#include <utility>
//using namespace std;

class Splitter;
class Marker;

class Slice
{
public:

  Slice();
  std::list<std::unique_ptr<Slice>> split(Splitter& splitter);

  void refine(Marker& marker);

  virtual gp_Dir surfaceNormal(const double& u);

  virtual TopoDS_Shape shape();

  std::list<double> params;
  std::list<gp_Pnt> points;
  std::list<double> alphas;

private:

  virtual Slice* emptyCopy();
  virtual void updateGeometry();
  virtual void calc(const std::list<double>& params, std::list<gp_Pnt>& points,
    std::list<double>& alphas);

};

#endif//WIRECAMSLICE_H
