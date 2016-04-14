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

TEST_F(PocketTest, POCKET_SQ)
{
  Pocket p1(pocketWire,0.0,0.0,5.0,-5.0,true,true);
  bt.Write(p1.dumpWires(),"./PocketTest_POCKET_SQ.brep");
}

TEST_F(PocketTest, POCKET_RND)
{
  Pocket p1(pocketWire,0.0,0.0,10,-5.0,false,true);
  bt.Write(p1.dumpWires(),"./PocketTest_POCKET_RND.brep");
}
/*
TEST_F(PocketTest,PATHTREE_OUTERCONST)
{
  double offset = -5;
  double step = -4;
  PathTree pt(pocketWire,offset,step,false);
  TopoDS_Compound comp = pt.dumpWires();
  bt.Write(comp,"./PocketTest_PATHTREE_OUTERCONST.brep");
  int numWires = 0;
  TopExp_Explorer exp;
  for (exp.Init(comp,TopAbs_WIRE);exp.More();exp.Next())
  {
    ++numWires;
  }
  std::cout<<numWires<<" wires in pt"<<std::endl;
}
*/
