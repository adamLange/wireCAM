#include "Pocket.h"
#include "TopoDS.hxx"
#include "TopoDS_Wire.hxx"
#include "BRepOffsetAPI_MakeOffset.hxx"
#include "TopExp_Explorer.hxx"

#include <exception>
#include <stdexcept>

Pocket::Pocket(TopoDS_Wire& wire):
  offset(-1),
  zOffset(0),
  radius(10),
  square(true),
  outward(true),
  wire(wire)
{
}

void
Pocket::setOffset(double& off)
{
  offset=off;
}

void
Pocket::setZOffset(double& zoff)
{
   zOffset = zoff;
}

void
Pocket::setCornerSquare()
{
  square = true;
}

void
Pocket::setCornerRound()
{
  square = false;
}

void
Pocket::setRadius(double& r)
{
  radius = r;
}

void
Pocket::setOrderOut()
{
  outward = true;
}

void
Pocket::setOrderIn()
{
  outward = false;
}

void
Pocket::calculate()
{

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
    wires.push_back(TopoDS::Wire(exp.Current()));
  }
  if (wires.size() != 1)
  {
    throw std::runtime_error("In Pocket::calculate the main offset\
       resulted in other that one wire!");
  }

  pathTree.calculate(wires.front(),offset,true);

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
