#include "BRepTools.hxx"
#include "BRep_Builder.hxx"
#include "TopoDS_Shape.hxx"
#include "TopoDS_Face.hxx"
#include "Pocket.h"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"

TEST(POCKET, CONSTRUCTCONFIG)
{
  BRepTools bt;
  BRep_Builder bb;
  TopoDS_Shape shape;
  bt.Read(shape,"./pocketFace.brep",bb);

  TopoDS_Face face;
  TopExp_Explorer exp;
  list<TopoDS_Wire> wires;
  for (exp.Init(shape,TopAbs_WIRE);exp.More();exp.Next())
  {
    wires.push_back(TopoDS::Wire(exp.Current()));
  }
  std::cout<<wires.size()<<"wires found in pocketFace.brep"<<std::endl;
  Pocket p1(wires.front());
  p1.calculate();
  bt.Write(p1.dumpWires(),"./pocketWiresOut.brep");
}
