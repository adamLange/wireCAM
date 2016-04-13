#ifndef PATHTREE_H
#define PATHTREE_H

#include "TopoDS_TWire.hxx"
#include "TopoDS_Wire.hxx"
#include "TopoDS_Compound.hxx"
#include <list>
#include "Standard_Transient.hxx"
#define _USE_MATH_DEFINES

class Slice;

class PathTree
{

public:

  PathTree(TopoDS_Wire& w, double& offset, double& step,
    bool squareCorners = true, double cornerATol = M_PI/180.);
  PathTree(TopoDS_Wire* outerWire, TopoDS_Wire& w,double& offset,
    double cornerATol = M_PI/180.);
  void calculate();
  //std::list<TopoDS_Wire> dumpWires();
  TopoDS_Compound dumpWires();

private:

  TopoDS_Wire* outerWire;
  double offset;
  std::list<PathTree> children;
  TopoDS_Wire wire;
  std::list<Slice> slices;
  double cornerATol;
  bool squareCorners;

};

#endif//PATHTREE_H
