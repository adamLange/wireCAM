#include "PathTree.h"
#include "BRepOffsetAPI_MakeOffset.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Builder.hxx"
#include <iostream>

PathTree::PathTree()
{
}

PathTree::PathTree(TopoDS_Wire& w,double& offset,bool isOuter)
{
  calculate(w,offset,isOuter);
}

void
PathTree::calculate(TopoDS_Wire& w,double& offset,bool isOuter)
{
  //wire = Handle(TopoDS_TWire)::DownCast(w.TShape());
  wire = w;
  //Calc toolpath data
  //Do offset
  BRepOffsetAPI_MakeOffset mo(wire);
  mo.Perform(offset);
  TopoDS_Shape result = mo.Shape();
  TopExp_Explorer exp;
  std::list<TopoDS_Wire> wires;
  for (exp.Init(result,TopAbs_WIRE);
       exp.More();
       exp.Next()
      )
  {
    TopoDS_Wire wi(TopoDS::Wire(exp.Current()));
    children.emplace(children.end(),wi,offset,false);
  }
}

/*
std::list<TopoDS_Wire>
PathTree::dumpWires()
{
  std::list<TopoDS_Wire> wires;
  wires.push_back(wire);
  for (std::list<PathTree>::iterator it = children.begin();
       it != children.end();
       ++it
        )
  {
    wires.splice(wires.end(),it->dumpWires());
  }
  return wires;
}
*/

TopoDS_Compound
PathTree::dumpWires()
{
  TopoDS_Compound comp;
  TopoDS_Builder builder;
  builder.MakeCompound(comp);
  
  std::list<TopoDS_Wire> wires;
  wires.push_back(wire);
  for (std::list<PathTree>::iterator it = children.begin();
       it != children.end();
       ++it
        )
  {
    std::cout<<"iterating over children"<<std::endl;
    TopExp_Explorer exp;
    for (exp.Init(it->dumpWires(),TopAbs_WIRE);
         exp.More();
         exp.Next()
        )
    {
      std::cout<<"iterating over child wires"<<std::endl;
      builder.Add(comp,exp.Current());
    }
  }
  return comp;
}
