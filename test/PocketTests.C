#include "BRepTools.hxx"
#include "BRep_Builder.hxx"
#include "TopoDS_Shape.hxx"
#include "TopoDS_Face.hxx"
#include "TopoDS_Wire.hxx"
#include "Pocket.h"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "PathTree.h"


class PocketTest : public ::testing::Test
{
protected:
  virtual void SetUp(){
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
    std::cout<<wires.size()<<" wires found in pocketFace.brep"<<std::endl;
    pocketWire = wires.front();
  }
  TopoDS_Wire pocketWire;
  BRepTools bt;
};

TEST_F(PocketTest, POCKET_CONST)
{
  Pocket p1(pocketWire);
  p1.calculate();
  bt.Write(p1.dumpWires(),"./PocketTest_POCKET_CONST_Out.brep");
}

TEST_F(PocketTest,PATHTREE)
{
  double offset = -5;
  PathTree pt(pocketWire,offset,true);
  TopoDS_Compound comp = pt.dumpWires();
  bt.Write(comp,"./PocketTest_PATHTREE.brep");
  int numWires = 0;
  TopExp_Explorer exp;
  for (exp.Init(comp,TopAbs_WIRE);exp.More();exp.Next())
  {
    ++numWires;
  }
  std::cout<<numWires<<" wires in pt"<<std::endl;
}
