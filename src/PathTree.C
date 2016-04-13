#include "PathTree.h"
#include "BRepOffsetAPI_MakeOffset.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Builder.hxx"
#include "BRep_Tool.hxx"
#include "TopoDS.hxx"
//#include "Slice.h"
#include "PocketEdgeSlice.h"
#include "PocketCornerSlice.h"
#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <exception>
#include <stdexcept>

PathTree::PathTree(TopoDS_Wire& w, double& offset, double& step,
  bool squareCorners,double cornerATol)
{
  //offset w
  BRepOffsetAPI_MakeOffset mo(w);
  mo.Perform(offset);
  TopoDS_Shape result = mo.Shape();
  std::list<TopoDS_Wire> wires;
  TopExp_Explorer exp;
  for (exp.Init(result,TopAbs_WIRE); exp.More(); exp.Next())
  {
    TopoDS_Wire wi(TopoDS::Wire(exp.Current()));
  }
  if (wires.size()>1)
  {
    throw std::runtime_error("PathTree::PathTree initial offset resulted\
      in more than one wire.");
  }
  wire = wires.front();
  outerWire = &w;
  this->offset = step;
  this->cornerATol = cornerATol;
  calculate();
}

PathTree::PathTree(TopoDS_Wire* outerWire, TopoDS_Wire& w,
    double& offset, double cornerATol
  ):
  outerWire(outerWire),
  offset(offset),
  cornerATol(cornerATol)
{
  calculate();
}

void
PathTree::calculate()
{
  TopExp_Explorer exp;
  for (exp.Init(wire,TopAbs_EDGE);exp.More();exp.Next())
  {
    PocketEdgeSlice* e = new PocketEdgeSlice(TopoDS::Edge(exp.Current()),true,1e-3);
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
      double dalpha = std::abs(head->alphas.back()-tail->alphas.front());
      if ((outerWire)&&(dalpha > cornerATol)) //need cornerSlice
      {
        gp_Pnt pnt = head->points.back();
        double alpha0 = tail->alphas.back();
        double alpha1 = head->alphas.front();
        PocketCornerSlice* cs = new PocketCornerSlice(pnt,alpha0,alpha1,
          *outerWire,offset);
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
    TopoDS_Wire* ptrw;
    children.emplace_back(ptrw,wi,offset);
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
