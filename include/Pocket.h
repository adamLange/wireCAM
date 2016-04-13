#ifndef POCKET_H
#define POCKET_H

#include <string>
#include "PathTree.h"
#include "TopoDS_Compound.hxx"

class PostProcessor;
class TopoDS_Face;

class Pocket
{
public:

  Pocket(TopoDS_Wire& wire, double& offset, double& zOffset,
    double& toolR, double& step, bool squareCorners, bool orderOut);

  void calculate();
  std::string postProcess(PostProcessor& pp);

  TopoDS_Compound dumpWires();

private:

  TopoDS_Wire& wireIn,offsetWire;

  double offset,zOffset,toolR,step;
  bool squareCorners,orderOut;
  PathTree pathTree;

  TopoDS_Face face;

};

#endif//POCKET_H
