#ifndef WIRECAMSLICE_H
#define WIRECAMSLICE_H

#include "TopoDS_Face.hxx"
#include "TopoDS_Edge.hxx"
#include "gp_Pnt.hxx"
#include <list>
#include <memory>
//using namespace std;

class Splitter;
class Marker;

class Slice
{
public:

  Slice();

  std::list<std::unique_ptr<Slice>> split(Splitter& splitter);

  void refine(Marker& marker);

  virtual gp_Vec surfaceNormal(const double& u);

  std::list<double> params;
  std::list<gp_Pnt> points;
  std::list<double> alphas;

private:

  virtual void emptyCopy(std::unique_ptr<Slice>& ptr);

};

#endif//WIRECAMSLICE_H
