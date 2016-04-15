#include "PathTree.h"
#include "Pocket.h"
#include "BRepOffsetAPI_MakeOffset.hxx"
#include "BRepOffsetAPI_NormalProjection.hxx"
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

#include "BRepGProp.hxx"
#include "GProp_GProps.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "Slice.h"

#include <iostream>
#include <sstream>

PathTree::PathTree(Pocket& p,TopoDS_Wire& w,
  bool squareCorners,double cornerATol):
  pocket(p),
  wire(w),
  //step(step),
  squareCorners(squareCorners)
  //cornerATol(cornerATol)
{
  //offset w
  /*
  BRepOffsetAPI_MakeOffset mo(w);
  mo.Perform(step);
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
  this->step = step;
  this->cornerATol = cornerATol;
  */
  calculate();
}

/*
PathTree::PathTree(TopoDS_Wire* outerWire, TopoDS_Wire& w,
    double& offset, double cornerATol
  ):
  pocket(p),
  outerWire(outerWire),
  offset(offset),
  cornerATol(cornerATol)
{
  calculate();
}
*/

void
PathTree::calculate()
{

  //GProp_GProps props;
  //BRepGProp gprop;
  //gprop.LinearProperties(wire,props);
  //BRepGPropLinearProperties(wire,props);
  //gp_Pnt com = props.CentreOfMass();
  
  //BRepBuilderAPI_MakeFace mf();

  TopExp_Explorer exp;
  for (exp.Init(wire,TopAbs_EDGE);exp.More();exp.Next())
  {
    Slice* e = new PocketEdgeSlice(TopoDS::Edge(exp.Current()),true,1e-3);
    slices.emplace_back(e);
  }
  //Cut corners
  if (squareCorners)
  {
    std::list<std::unique_ptr<Slice>>::iterator head = slices.begin();
    std::list<std::unique_ptr<Slice>>::iterator tail = slices.begin();
    ++head;
    bool flag = true;
    while (flag)
    {
      double dalpha = std::fmod(std::fabs(((*head)->alphas.front())-((*tail)->alphas.back())),(2*M_PI));
      if (dalpha > M_PI/180.) //need cornerSlice
      {
        gp_Pnt pnt = (*head)->points.front();
        double alpha0 = (*tail)->alphas.back();
        double alpha1 = (*head)->alphas.front();
        Slice* cs = new PocketCornerSlice(pocket,pnt,alpha0,alpha1);
        slices.emplace(head,cs);
        //++tail;
        --head;
      }

      ++head;
      ++tail;
      if(head==slices.end())
      {
        head = slices.begin();
      }
    flag = (tail!=slices.end());
    }
  }
  //Do offset
  BRepOffsetAPI_MakeOffset mo(wire);
  mo.Perform(pocket.step);
  TopoDS_Shape result = mo.Shape();
  std::list<TopoDS_Wire> wires;
  for (exp.Init(result,TopAbs_WIRE);
       exp.More();
       exp.Next()
      )
  {
    TopoDS_Wire wi(TopoDS::Wire(exp.Current()));
    /*
    BRepOffsetAPI_NormalProjection np(pocket.face);
    np.Add(wi);
    TopTools_ListOfShape los;
    if (!np.BuildWire(los))
    {
      throw std::runtime_error("failed to project wire");
    }
    wi = TopoDS::Wire(los.First());
    los.RemoveFirst();
    if (!los.IsEmpty())
    {
      throw std::runtime_error("projection resulted in multiple wires");
    }
    */
    children.emplace_back(pocket,wi,false);
  }
}

TopoDS_Compound
PathTree::dumpWires()
{
  TopoDS_Compound comp;
  TopoDS_Builder builder;
  builder.MakeCompound(comp);
  for (std::list<std::unique_ptr<Slice>>::iterator slice = 
    slices.begin(); slice != slices.end();++slice
  )
  {
    builder.Add(comp,(*slice)->shape());
  }
  
  std::list<TopoDS_Wire> wires;
  wires.push_back(wire);
  for (std::list<PathTree>::iterator it = children.begin();
       it != children.end();
       ++it
        )
  {
    std::cout<<"iterating over children"<<std::endl;
    TopExp_Explorer exp;
    for (exp.Init(it->dumpWires(),TopAbs_COMPOUND);
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

std::string
PathTree::postProcess(PostProcessor& p)
{
  std::stringstream str;
  /*
  for (std::list<std::unique_ptr<Slice>>::iterator slice =
        slices.begin(); slice != slices.end(); ++slice
    )
  {
    str<<p.postProcess(**slice);
  }
  */
  str<<p.postProcess(slices);
  for (std::list<PathTree>::iterator pt = children.begin();
       pt != children.end(); ++pt)
  {
    str<<pt->postProcess(p);
  }
  return str.str();
}
