#ifndef POCKET_H
#define POCKET_H

#include <string>
#include "PathTree.h"
#include "TopoDS_Compound.hxx"
#include <memory>

class PostProcessor;
class TopoDS_Face;

class Pocket
{
public:

  Pocket(TopoDS_Wire& wire, double offset, double zOffset,
    double toolR, double step, bool squareCorners, bool orderOut);

  void calculate();
  std::string postProcess(PostProcessor& pp);

  TopoDS_Compound dumpWires();

  TopoDS_Face face;

  TopoDS_Wire& wireIn,offsetWire;
  double toolR;
  double offset,zOffset,step;

private:

  bool squareCorners,orderOut;
  std::unique_ptr<PathTree> pathTree;

};

#endif//POCKET_H
