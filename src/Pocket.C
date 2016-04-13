#include "Pocket.h"
#include "TopoDS.hxx"
#include "TopoDS_Wire.hxx"
#include "BRepOffsetAPI_MakeOffset.hxx"
#include "TopExp_Explorer.hxx"

#include "gp_Vec.hxx"
#include "gp_Trsf.hxx"
#include "BRepBuilderAPI_Transform.hxx"

#include <exception>
#include <stdexcept>


Pocket::Pocket(TopoDS_Wire& wire, double& offset, double& zOffset,
    double& toolR, double& step, bool squareCorners, bool orderOut)
  wireIn(wire),
  offset(offset),
  zOffset(zOffset),
  toolR(toolR),
  step(step),
  squareCorners(squareCorners),
  orderOut(orderOut)
{
  calculate();
}


void
Pocket::calculate()
{

  BRepOffsetAPI_MakeOffset mo(wireIn);
  mo.Perform(offset);
  TopoDS_Shape result = mo.Shape();
  TopExp_Explorer exp;
  std::list<TopoDS_Wire> wires;
  for (exp.Init(result,TopAbs_WIRE);
       exp.More();
       exp.Next()
      )
  {
    wires.push_back(TopoDS::Wire(exp.Current()));
  }
  if (wires.size() != 1)
  {
    throw std::runtime_error("In Pocket::calculate the main offset\
       resulted in other that one wire!");
  }

  offsetWire = wires.front();
  gp_Trsf trsf;
  trsf.SetTranslation(gp_Vec(0,0,zOffset));
  BRepBuilerAPI_Transform mt(offsetWire,trsf);
  wires.clear();
  for (exp.Init(result,TopAbs_WIRE);
       exp.More();
       exp.Next()
      )
  {
    wires.push_back(TopoDS::Wire(exp.Current()));
  }
  if (wires.size() != 1)
  {
    throw std::runtime_error("In Pocket::calculate the zOffset transform\
       resulted in more than one wire!");
  }
  offsetWire = wires.front();

  //pathTree.calculate(offsetWire,offset,true);

  return;
}

std::string
Pocket::postProcess(PostProcessor& pp)
{
  return "poof";
}

TopoDS_Compound
Pocket::dumpWires()
{
  return pathTree.dumpWires();
}
