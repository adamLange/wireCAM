#ifndef PATHTREE_H
#define PATHTREE_H

#include "TopoDS_Face.hxx"
#include "TopoDS_Wire.hxx"
#include "TopoDS_Compound.hxx"
#include <list>
#include <memory>
#include <string>
#include "Standard_Transient.hxx"
#include "PostProcessor.h"
#define _USE_MATH_DEFINES

class Pocket;

//class Slice;
#include "Slice.h"

class PathTree
{

public:

  PathTree(Pocket& p,TopoDS_Wire& w, bool squareCorners = true,
    double cornerATol = M_PI/180.);
  //PathTree(Pocket& p, TopoDS_Wire& w,
  //  double& offset, double cornerATol = M_PI/180.);
  void calculate();
  //std::list<TopoDS_Wire> dumpWires();
  TopoDS_Compound dumpWires();
  std::string postProcess(PostProcessor& p);

private:

  Pocket& pocket;
  //TopoDS_Wire* outerWire;
  //double step;
  std::list<PathTree> children;
  TopoDS_Wire wire;
  std::list<std::unique_ptr<Slice>> slices;
  //double cornerATol;
  bool squareCorners;

};

#endif//PATHTREE_H
