#ifndef PATHTREE_H
#define PATHTREE_H

#include "TopoDS_TWire.hxx"
#include "TopoDS_Wire.hxx"
#include "TopoDS_Compound.hxx"
#include <list>
#include "Standard_Transient.hxx"

class PathTree
{

public:

  PathTree();
  PathTree(TopoDS_Wire& w,double& offset,bool isOuter);
  void calculate(TopoDS_Wire& w,double& offset,bool isOuter);
  //std::list<TopoDS_Wire> dumpWires();
  TopoDS_Compound dumpWires();

private:

  bool isOuter;
  std::list<PathTree> children;
  //Handle(TopoDS_TWire) wire;
  TopoDS_Wire wire;

};

#endif//PATHTREE_H
