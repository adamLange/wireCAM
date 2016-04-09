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

  Pocket(TopoDS_Wire& wire);

  void setOffset(double& off);
  void setZOffset(double& zoff);
  void setCornerSquare();
  void setCornerRound();
  void setRadius(double& r);
  void setOrderOut();
  void setOrderIn();

  void calculate();
  std::string postProcess(PostProcessor& pp);

  TopoDS_Compound dumpWires();

private:

  double offset,zOffset,radius;
  bool square,outward;
  TopoDS_Wire& wire;
  PathTree pathTree;

};

#endif//POCKET_H
