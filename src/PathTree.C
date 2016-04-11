#include "PathTree.h"
#include "BRepOffsetAPI_MakeOffset.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Builder.hxx"
#include "BRep_Tool.hxx"
#include "Slice.hxx"
#include <iostream>
#include <math>

PathTree::PathTree()
{
}

PathTree::PathTree(TopoDS_Wire& w,double& offset,bool isOuter):
{
  calculate(w,offset,isOuter);
}

void
PathTree::calculate(TopoDS_Wire& w,double& offset,bool isOuter)
{
  //wire = Handle(TopoDS_TWire)::DownCast(w.TShape());
  wire = w;
  //Calc toolpath data
  TopExp_Explorer exp;
  for (exp.Init(wire,TopAbs_EDGE);exp.More();exp.Next())
  {
    PocketSliceEdge* new e(exp.Current());
    slices.push_back(*e);
  }
  //Cut corners
  if (squareCorners)
  {
    std::list<Slice>::iterator head = slices.begin();
    std::list<Slice>::iterator tail = slices.begin();
    ++head;
    while (tail!=slices.end())
    {
      double dalpha std::abs(head.params.alphas()-tail.params.alphas());
      if (dalpha > cornerATol) //need cornerSlice
      {
        PocketCornerSlice* new cs;
        slices.insert(head,*cs);
        ++tail;
      }
  
      ++head;
      ++tail;
      if(head==slices.end())
      {
        head = slices.begin();
      }
    }
  }
  //Do offset
  BRepOffsetAPI_MakeOffset mo(wire);
  mo.Perform(offset);
  TopoDS_Shape result = mo.Shape();
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

TopoDS_Compound
PathTree::dumpWires()
{
  TopoDS_Compound comp;
  TopoDS_Builder builder;
  builder.MakeCompound(comp);
  builder.Add(comp,wire);
  
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
